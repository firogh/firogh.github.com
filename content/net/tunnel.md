---
date: 2014-12-28
title:  tunnel
category: net
---

#class
network layer over network layer: gre
link layer over network layer: l2tp, pptp

# Proxy
[透明代理、匿名代理、混淆代理、高匿代理有什么区别？](http://www.coder4.com/archives/4434)
## 正向代理forward proxy
shadowsocks goagent

## Reverse proxy
负载均衡的服务器, 缓存的静态内容的server

## 透明代理
hiwifi router kproxy nginx, kproxy 是更细粒度的iptables.
所谓透明代理就是正向代理且client零配置.

## DMZ
Demilitarized zone, 解除军事武装区 or 非军事区!
这个跨领域的名字听上去, 好叼叼的, 现实中南极洲和三八线都是非军事区.
为什么叫这个名字, 暗指firewall没有设置太多限制规则的一部分网络地址空间.
内网和外网是争端的主角, 而DMZ是个缓冲区.

### simplest sample
iptables -A INPUT -p tcp -d 10.42.0.52 --dport 53 -j ACCEPT
iptables -A FORWARD -p tcp -d 192.168.1.2 --dport 53 -j ACCEPT
iptabltes -t nat -A PREOUTTING -p -tcp 10.42.0.52 --dport 53 -j DNAT --to 192.168.1.2:53
我们就知道了dmz有整体有两部分构成ACCEPT and DNAT
同时涉及到一个dmz地址空间{dmzwan10.42.0.52:53, dmzlan:192.168.1.2:53}


#Basic concepts
* VPN is created by establishing a virtual point-to-point connection through the use of
1. dedicated connections(permanent)
2. virtual tunneling protocols
traffic encryptions and authentication
+ connection(tunnel) + secure

#Tunnel proctocol
* pptp
data-link header | PPP | IP ...
data-link header | IP | GRE | PPP | IP...
rfc2637
http://pptpclient.sourceforge.net/diagrams.phtml 
* pppd -> /dev/ppp
/dev/ppp (see ppp_init) 下发密码, 加密之类的.
ppp_netdev_ops (see ppp_create_interface)
ppp_wirte->ppp_xmit_process->ppp_send_frame->ppp_push->|pch->chan->ops->start_xmit()=pppoe_xmit->__pppoe_xmit

* init
pptp -> pptp server -> generate PPP connection and PPP0(interface)
* input
eth -> ppptp(remove gre header)->PPP header-> pty(device) -> PPP0(interface) -> 
* output
-> PPP0(interface) -> PPP header -> pty -> PPPT(add gre header) -> raw socket -> eth


* shadowsocks
user -> iptables -> shadowsocks(redir.c (struct server)->(struct remote)) -> server

ssh redsocks
user -> iptables -> redsocks -> ssh -> server

* pppoe
pppoe 的sk->sk_backlog_rcv是在pppoe_create中确定的, 这点却别与tcp socket
pppoe_rcv_core ->　netif_rx
从sk_receive_skb来看pppoe 和tcp都对应一个session or sock!
对比inet_create 和 pppoe_create, 而pppoe很屌是在链路层的sock厉害!
pppoe报文会经过两次sk_filter()
http://blog.csdn.net/osnetdev/article/details/8958058
&pppoe_chan_ops;

+ppp header 
is added in ppp_start_xmit to ppp_send_frame NO ppp_hdr struct!
ppp-wan->ndo_start_xmit()=ppp_start_xmit()->ppp_xmit_process()->ppp_send_frame()
->
{
	__skb_push(skb, sizeof(*ph));
    skb_reset_network_header(skb);
	//add pppoe_hdr no header???
    dev_queue_xmit(skb); 
}


#PPP 
http://www.cnitblog.com/liaoqingshan/archive/2013/06/13/52906.html


#GRE
include/net/gre.h

#L2tp
IP -> UDP -> L2TP -> PPP

* source files
net/l2tp/
├── Kconfig
├── l2tp_core.c
├── l2tp_core.h
├── l2tp_debugfs.c
├── l2tp_eth.c
├── l2tp_ip.c
├── l2tp_netlink.c
├── l2tp_ppp.c
└── Makefile

include/uapi/linux/ppp_defs.h

* Functions
l2tp_recv_common ...recv_skb=pppol2tp_recv()
l2tp_xmit_core

#PPTP
##Route ip-list to pptp-vpn
11: pptp-vpn: <POINTOPOINT,MULTICAST,NOARP,UP,LOWER_UP> mtu 1482 qdisc pfifo_fast state UNKNOWN qlen 3
    link/ppp 
    inet 192.168.1.60 peer 192.168.1.1/32 scope global pptp-vpn
root@Hiwifi:~# route -n
Kernel IP routing table
Destination     Gateway         Genmask         Flags Metric Ref    Use Iface
0.0.0.0         192.168.1.1     0.0.0.0         UG    0      0        0 pptp-vpn
123.121.94.19   192.168.10.254  255.255.255.255 UGH   0      0        0 eth2.2
192.168.1.1     0.0.0.0         255.255.255.255 UH    0      0        0 pptp-vpn
192.168.10.0    0.0.0.0         255.255.255.0   U     0      0        0 eth2.2
192.168.199.0   0.0.0.0         255.255.255.0   U     0      0        0 br-lan

##pptp header
IP | GRE | pptp_gre_header | PPP | IP

##input
ip -> gre -> ppp -> ip
ip_local_deliver_finish()(netfilter NF LOCAL IN smartqos local in) ->inet_protos.gre_rcv -> gre_proto[GREPROTO_PPTP]=pptp_rcv{pptp_gre_header} -> sk_receive_skb ->sk_backlog_rcv -> pptp_rcv_core -> ppp_input -> ppp_do_recv 
-> ppp_receive_frame -> ppp_receive_nonmp_frame -> netif_rx -> netif_rx_internal -> enqueue_to_backlog -> ____napi_schedule(sd, &sd->backlog);
-> netif_receive_skb...

##output
ip_output ...-> ppp_start_xmit -> ppp_xmit_process -> ppp_send_frame -> add ppp header -> ppp_push 
->pch->chan->ops->start_xmit=pptp_xmit -> add gre and ip header. -> ip_local_out ->
+another send skb functions is ip_build_and_send_pkt()
{ this functions hook is init in pptp_connect() -> ppp_register_channel(&po->chan) }
{ppp_connect()'s init functions is pptp_create, see pptp_init_module -> 
register_pppox_proto(PX_PROTO_PPTP, &pppox_pptp_proto) pptp_create.}
when invoke pptp_create? socket()!

if layer 4 protocol is gre then pass.

##PPTP & NAT
* Forward package
br-lan(PREROUTING orginal packet NAT old) 
(route -> pptp-vpn) = ip_forward()->NF_INET_FORWARD->ip_forward_finish()->dst_output()
pptp-vpn -> ndo_start_xmit -> add some headers -> local_out NAT new -> tc dequeue_qdisc(this is maybe eth2.2's routing result! see top)
so we need to do is just find original skb in PPTP packet header!

##PPTP & GRE & Conntrack
As a pure IP protocol GRE uses only IP addresses but no port numbers giving the router's NAT a tough time to track such a connection. In its base configuration OpenWrt Backfire is able to NAT a single PPTP connections but not multiple such connections concurrently. It is also unreliable when trying to establish consecutive single PPTP connections from different LAN clients in rapid succession. 
=
kernel vpn just one session.

##ppp-> pptp code.
ppp_send_frame()
{

	//below is ok 132 is output!
	printk("ct %d, %d, %d, %d\n", HWF_CT_EXT(ct)->user_id, user_id, HWF_CT_EXT(ct)->is_marked, proto);
	switch(proto) //0x21, 33
	{
	
		//Below two print is not show, so we did pass through these codes.
        if (cp == skb->data + 2) {
            /* didn't compress */
            printk("NO compreess %x\n", ppp->flags);
            kfree_skb(new_skb);
        } else {
            if (cp[0] & SL_TYPE_COMPRESSED_TCP) {
                proto = PPP_VJC_COMP;
                cp[0] &= ~SL_TYPE_COMPRESSED_TCP;
            } else {
                proto = PPP_VJC_UNCOMP;
                cp[0] = skb->data[2];
            }
            kfree_skb(skb);
            skb = new_skb;
            printk("I am here %d ,%d, %x\n", cp[0], proto, ppp->flags);


	}
	printk("hit b %d\n", skb->qosmark);
	skb = pad_compress_skb(ppp, skb);//this functions skb will be replaced with a new_skab by alloc_skb();
	printk("hit a %d\n", skb->qosmark);
	//...
    printk("befor push %d %d\n", skb->qosmark, user_id);
    skb->qosmark = user_id;

    ppp->xmit_pending = skb;
    ppp_push(ppp);
//dmesg
[  304.600000] ct 132, 132, 0, 33
[  304.610000] hit b 132
[  304.610000] hit a 0
[  304.610000] befor push 0 132
[  304.610000] out chan 132
[  304.620000] chan ok132
[  304.620000] qosmark132 132



	so we need backup qosmark be for invoking this function then restore it.

}


ppp_push(struct ppp *ppp)
{
    struct list_head *list;
    struct channel *pch;
    struct sk_buff *skb = ppp->xmit_pending;

    if (!skb)
        return;

    list = &ppp->channels;
    if (list_empty(list)) {
        /* nowhere to send the packet, just drop it */
        ppp->xmit_pending = NULL;
        kfree_skb(skb);
        return;
    }

    if ((ppp->flags & SC_MULTILINK) == 0) {
        /* not doing multilink: send it down the first channel */
        list = list->next;
        pch = list_entry(list, struct channel, clist);

        printk("out chan %d\n", skb->qosmark);
        spin_lock_bh(&pch->downl);
        if (pch->chan) {
            printk("chan ok%d\n", skb->qosmark);
            if (pch->chan->ops->start_xmit(pch->chan, skb))
                ppp->xmit_pending = NULL;
        } else {
            /* channel got unregistered */
            printk("no chan\n", skb->qosmark);
            kfree_skb(skb);
            ppp->xmit_pending = NULL;


