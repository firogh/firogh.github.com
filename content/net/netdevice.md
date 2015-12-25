---
tags: net
title: Linux net device
date: 2015-02-27 15:46:13
category: net
---

# Common Interface felling
## No driver tg3
I remove all kernel 4.0  moduls that ip ad just only output lo interface info.

## Hiwifi
* Disable network(netifd) in hiwifi openwrt
root@Hiwifi:/# ip ad
1: lo: <LOOPBACK> mtu 16436 qdisc noop state DOWN
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
2: eth2: <BROADCAST,MULTICAST> mtu 1500 qdisc noop state DOWN qlen 1000
    link/ether d4:ee:07:07:75:c2 brd ff:ff:ff:ff:ff:ff
3: ra0: <BROADCAST,MULTICAST> mtu 1500 qdisc noop state DOWN qlen 1000
    link/ether 00:00:00:00:00:00 brd ff:ff:ff:ff:ff:ff
4: sit0: <NOARP> mtu 1480 qdisc noop state DOWN
    link/sit 0.0.0.0 brd 0.0.0.0
5: gre0: <NOARP> mtu 1476 qdisc noop state DOWN 
    link/gre 0.0.0.0 brd 0.0.0.0

* Enable network(netifd) in hiwifi openwrt
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 16436 qdisc noqueue state UNKNOWN
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
    inet6 ::1/128 scope host 
       valid_lft forever preferred_lft forever
2: eth2: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast state UNKNOWN qlen 1000
    link/ether d4:ee:07:07:75:c2 brd ff:ff:ff:ff:ff:ff
    inet6 fe80::d6ee:7ff:fe07:75c2/64 scope link 
       valid_lft forever preferred_lft forever
3: ra0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast master br-lan state UNKNOWN qlen 1000
    link/ether d4:ee:07:07:75:c2 brd ff:ff:ff:ff:ff:ff
4: sit0: <NOARP> mtu 1480 qdisc noop state DOWN 
    link/sit 0.0.0.0 brd 0.0.0.0
5: gre0: <NOARP> mtu 1476 qdisc noop state DOWN 
    link/gre 0.0.0.0 brd 0.0.0.0
6: br-lan: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UP 
    link/ether d4:ee:07:07:75:c2 brd ff:ff:ff:ff:ff:ff
    inet 10.1.1.1/24 brd 10.1.1.255 scope global br-lan
    inet6 fe80::d6ee:7ff:fe07:75c2/64 scope link 
       valid_lft forever preferred_lft forever
7: eth2.1@eth2: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue master br-lan state UP 
    link/ether d4:ee:07:07:75:c2 brd ff:ff:ff:ff:ff:ff
8: eth2.2@eth2: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UP 
    link/ether d4:ee:07:07:75:c3 brd ff:ff:ff:ff:ff:ff
    inet 192.168.199.223/24 brd 192.168.199.255 scope global eth2.2
    inet6 fe80::d6ee:7ff:fe07:75c3/64 scope link 
       valid_lft forever preferred_lft forever
9: ra1: <BROADCAST,MULTICAST> mtu 1500 qdisc noop state DOWN qlen 1000
    link/ether d6:ee:07:04:75:c2 brd ff:ff:ff:ff:ff:ff
10: apcli0: <BROADCAST,MULTICAST> mtu 1500 qdisc noop state DOWN qlen 1000
    link/ether d6:ee:07:05:75:c2 brd ff:ff:ff:ff:ff:ff

#Bridging
* A bridge behaves like a virtual network switch, 一样.
Bridging is a router using MAC address at L2 in essence.
A bridge transparently relays traffic between multiple network interfaces. 
In plain English this means that a bridge connects two or more physical Ethernets together to form one bigger (logical) Ethernet.

## Spanning tree protocol(Rapid STP,Multiple STP)
the algorithm is not executed on a single host that later distributes the result to all the others; 
instead, this is a distributed protocol.

## BPUD Bridge protocol data units
* Configuration BPDU
Used to define the loop-free topology. 
Topology Change Notification (TCN) BPDU
Used by a bridge to notify the root bridge about a detected topology change. 

* BPDU Aging
On a stable network, the time depends mainly on how loaded the bridges are and how fast they can process BPDUs.

* Root Bridge
The root bridge is the only bridge that generates BPDUs
The root bridge makes sure each bridge in the network comes to know about a topology change when one occurs

* Designated Bridges
While each tree has only one root bridge, there is one designated bridge for each LAN, 
which becomes the bridge all hosts and bridges on the LAN use to reach the root.
The designated bridge is chosen by determining which bridge on the LAN has the lowest path cost to the root bridge.

* Bridge Port
While root ports lead toward the root of the tree (i.e., the root bridge), designated ports lead toward the leaves.

## Function
Rassive learning
Flooding
Aging
Bridging Loops
Switch:

## Details
newe_bridge_dev() create net_device and net_bridge.

# vlan
Vlans are a way to split up a layer2 broadcasting domain, VLANs allow you to create multiple separated networks with only a single switch.
In a vlan-capable network there are 2 types of connections : "access" connections and "trunk" connections
Vlan packet

## Vlan packet
Preamble 56 alternating bits | SFD10101011 | dst mac | src mac | TPID 0x8100 | TCI:PCP DEI VID| Ether type 0x86DD ipv6 ...|CRC FCS

## 802.1Q/Vlan header
TPID is the same as 0x86DD just a Ether type 0x8100  | PCP 3bis  DEI 1bit VID 12bis

## Access connection
An access connection looks like a normal connection to an ethernet switch, 
only that switch will only forward your packets within the same vlan, so they will not be able to reach ports that are in a different vlan.
For access ports, the switch will add (or overwrite) this tag value on any incoming(it means transfer out of host) packet before forwarding

## Trunk connnection
"Trunk" ports can communicate with multiple vlans, but you need to send special packets that contain 
both the packet and an indication in what vlan they are to be forwarded.
For trunk ports, the value is supposed to be present. If it is not, the value of the "native vlan" will be added.

## Split up a layer2 broadcasting domain
vlan 什么都别想, 你总的有个vlan吧. 对上来就创建一个vlan-device.
vconfig add eth0 vid
for trunk routing between the different vlans we need ip_forward

## Details of implemention
net/8021q/
### initialize
vlan_proto_init()
这个函数最重要的是映射了iocctl函数， 因为接下来的所有操作都要用到ioctl。

### application
vconfig eth0 1
vlan_ioctl_handler()->register_vlan_device():
alloc net_device.
vlan_setup()函数非常重要，设置dev->priv_flags 为 802.1q！tx queue 为0.
设置open函数为vlan_dev_open;
register_vlan_dev(）把这个设备注册到内核。

