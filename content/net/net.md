---
tags: net
title: Linux Network Stack
date: 2015-02-27 15:46:13
category: net
---

#Reference
[TCP/IP Reference Page](http://www.protocols.com/pbook/tcpip1.htm)

# Network
## 为什么Internet产生?
就好比问, 色情片为什么出现一样. 人类的发明都是对自身需求的回应.
Internet 解决信息传递的问题.信息传递只是手段, 古已有之, 比如信鸽, 狼烟等.

## Internet 都能做什么?
看片等...
 
## 什么是Internet
英文[network](http://keithbriggs.info/network.html), 其中work, 构造之意.
etymonline 给出结缔成网之意, net-like arrangement of threads, wires, etc.
Network -> Telecommunications network -> Computer network -> Internet

The Internet is a global system of interconnected computer networks that use 
the standard Internet protocol suite (TCP/IP) to link several billion devices worldwide.
Internet protocol suite,是结网的策略方法核心.

# Internet protocol suite
The Internet protocol suite is the computer networking model and set of 
communications protocols used on the Internet and similar computer networks.

Network stack严格的表述是network protocol stack.
通常简称[protocol stack](http://en.wikipedia.org/wiki/Protocol_stack), 即协议栈
The protocol stack is an implementation of a computer networking protocol suite. 
[Internet protocol suite -- TCP/IP](http://en.wikipedia.org/wiki/Internet_protocol_suite)就是一种[protocol stack.](http://en.wikipedia.org/wiki/List_of_network_protocol_stacks)

## 为什么协议要分层? 怎么分?
这是非常有深度的问题! 可以说本文价值所在.
这里讨论的是如何设计一套网络协议.
抛开TCP/IP, 如果要让我门自己设计一套 network protocol, 有什么思路?
发送方, 接收方, 信息. 这是所有信息传递的要素, 无论是, 狼烟, 信鸽, 还是书信.
不同的是, 信息传递的方式.狼烟这种大范围视觉方式, 关心的最少, 敌人来犯点着烟就行了.
那么信鸽了, 信鸽的英文叫做Homing pigeon, 信鸽的归巢本能, 为我指定了信息的接收房.
抛开隐私, 我们能把信放大到天空那么大, 收信人抬下头也能收到信息. 
对于书信的方式, 在信息不能广泛的传递给每个人的时候, 我们需要把信送到特定的人手里.  
信的地址就成了必选. 信息传输的方法决定了, 接收者的特征集合.
protocol暗指communications protocol, protocol词源上便指diplomatic rules of etiquette
一种合适的交流手段. 而The Internet protocol suite的定义中明确指明了computer networking model.
也就是源于方法决定对象的经验结论, wikipedia上的定义是非常有见地的.
所以回到计算机框架下的信息传输设计. 受限于计算机数据的交互方法, 我们必须指明信息的接收方.
所以我来设计一套network protocol, 第一点, 就是如何表示每个计算机收信方, 也就是地址.
地址成了网络协议的本体!有了地址后, 如何寻址就是个问题了, 这属于衍生的问题, 没什么意思了.
我们假装, 已经设计出一套惊天地泣鬼神的寻路方法, 还是叫寻址更好, 寻找目的地址.
我们只是道出了, 网络协议的基本的要素接收方的本质属性, 也就是model. 那么, 我们怎么定义方法呢?
传输无外乎, 你有消息, 给我发个信, 再有消息, 再发, 当然我也可以给你发. 结合到实际的计算机领域,
假如, 我们只有地址和寻址方法的寒酸网络协议, 在一个小函数里搞定了.
为什么OSI和TCP/IP都搞得那么复杂?
下面才是为什么要[分层](http://en.wikipedia.org/wiki/Abstraction_layer), 和怎么分的问题.
是什么导致了分层? 哲学上, 分乃是不同的存在.也就是说存在我们寒酸的网络协议不同的存在.
我们现在把linux内核的协议栈替换称我们的poor network protocol, (你现在, 应该知道为什么osi中
network layer的名字来源了, 正因为他代表了整个网络协议的实质, 所以名字逼格才这么高!)
显然, 还是不能运行, 为什么?我们缺少和应用层以及底层硬件的交互.也就是空中楼阁! 
加一个poor bsd socket,  poor application layer来了.加一个和底层硬件交互, poor linker layer也来了. 
我靠, 我的协议也有3层了, 拿去用吧.
我现在把传输层也意淫进来. 显然, 传输层不是这么随便的, 他的存在肯定有着合理的理由.
事物存在的理由不在于自身! 我认为汉语传输层(transport)是一个被严重误读了的名字, 这不同于人的名字, 
人类的名字是一个标示系统, 比如你叫马机霸, 你就一定要长得像马jb. 而学术领域的, 名字则具有
另外一个重要的属性就是:

	概念性or推理性的认知.
即从名字中可以推理出相关属性; 在汉语当中, 我第一次看谢希仁《计算机网络第五版》教材时,
就感觉这个名字好晕啊!他和传输有半毛钱关系, 传输不是链路层网卡网线的事吗? 
记得老师说了句, 这是一种抽象, 问题旧搁置了.当年未能深入问题, 非常遗憾. 今天补上!
TCP中的T是transmission和transport中的都被翻译成了汉语的传输! 
有很长一段时间, 我都认为传输层,应该叫做传输控制层, 因为他, 看上去真的更像控制啊!
那么transmission和transport的区别到底事什么呢?
看wikipedia的解释, 立马明白了, 这分明说的是传输层的本质啊:
Transport or transportation is the movement of people, animals and goods from one location to another.
相信不用看etymonline你也知道transport是怎么来的trans + port, 这里的port是名词港口之意.
现在, 就明白了为什么port端口号, 虽然实质是地址的含义, 却不属于network层.
传输层有卸货的含义, 干脆就叫转港层吧!
transport layer 的巨大意义, 就被显示出来了, 他是必须的.
实质上我们也应该看到无论是port还是ip address都是地址的含义, 这也是协议栈模型的本质.
下面我们来讨论, 信息发送的方法问题.
网络协议栈的每一层都有着不同协议, 也就是不同方法.即便是一个协议自身也是众多方法的集合.
理解这些协议程度, 就成为network 工程师能力差异, 下面我们会逐层意淫, 绝不是什么分析理解.
回到最初的问题, 实际上我们已经解决了怎么分层的问题了.
我们现在还差一个, 为什么分层.
简单说这是个设计问题. 设计的好会影响的设计本身与实现.这里是模块化的设计思路.
优点如维基所说:
Because each protocol module usually communicates with two others, 
they are commonly imagined as layers in a stack of protocols. 
[更完整的陈述](http://en.wikipedia.org/wiki/Communications_protocol#Layering)

# Design of [Communications protocol](http://en.wikipedia.org/wiki/Communications_protocol)
[The Design Philosophy of the DARPA Internet Protocols](http://ccr.sigcomm.org/archive/1995/jan95/ccr-9501-clark.pdf)
[Reviews from RICE edu](http://www.cs.rice.edu/~eugeneng/teaching/s04/comp629/reviews/Cla88.txt)
[Reviews of Michael S. Liu Yale](http://zoo.cs.yale.edu/classes/cs633/Reviews/Cla88.msl38.html)
我们在这里探讨的是通用的信息交换协议, 也就是说UDP, TCP, IP共有的性质, 之后我们会结合Stevens的
书籍和linux 内核的实现, 去探讨这些协议具体的差异, 当然是偏重为什么导致这种差异.
方法还是思考(意淫).
我们还是从地址开始，
Address formate: 有了地址我们就要有地址的具体格式如哪个街道，哪个小区几号楼几室。
Route: 找到收信人, 计算机网络包括linker, network 和transportlayer
Data formate: 接下来就是信封格式，对应头部。
Reliability:发的信可能丢了，要在写一封吗？
Detection of transmission errors:如信被人篡改了,如[浔阳楼宋江题反诗，梁山泊戴宗传假信](http://www.iqiyi.com/dianshiju/20110608/3773df277f51d210.html)
这些都是信息交换常见的问题场景.还有一些和计算机相关的问题.
Connection-oriented communication: in order and connect-wared
Flow control
Congestion control
信息交换协议, 基本上就是解决这些问题.TCP, UDP, IP这些协议也全是为了解决这些问题.
另外, 用户态用stream socket指代了connection-oriented, realiability等问题. 
是一个高度复杂的概念, 用户态可以通过sock的类型安排协议, 传输层和网络层都不是必须.

# Kernel network infrastructure
## [skb](http://vger.kernel.org/~davem/skb.html)
[Basic functions for sk_buff](http://www.skbuff.net/skbbasic.html)
[SKB data area handling](http://vger.kernel.org/~davem/skb_data.html)
[Things that need to get done in the Linux kernel networking](http://vger.kernel.org/~davem/net_todo.html)
现在poor network stack 基本功能已经实现了, 用户要在twitter上分享一个心机婊的自拍照.
另外, 我认为心机婊还是非常可爱的一群人.协议栈接到用户态的数据后,
内核要用一个buff存储照片, 好了把照片copy进去.我们走道了transport layer.
我们要encapuslate一个poor transport header. 靠忘记在照片数据前面留点空间给头部了.
可以遇见的往下走的网络层和linker也会遇到这问题, 留大点.
每encapuslate都应该有一个pointer指向这个header, 便于下次加头部时候能
推算((char *)old header pointer - new header len)出写入到buff的起始地址.
同样我们也应该知道这个buffer的起始地址.
当我们穿过协议栈时, 我们需要一个list成员, 把这个数据放到相应协议层处理队列上去.
实际内存区域buff, 指向buff的各种表明位置的指针, 还有一个关联buff的list成员. 
如何组织他们?这里把实际的报文数据和管理相关的数据分开管理最合适了.
就有了内核sk_buff这个结构了.

我们再来看看发包的过程, 
在申请发送数据的缓存区, 我们也要为管理数据sk_buff申请空间.
我们已经设计好了, 不是吗? alloc_skb跟我们的想法一致.
接下来, 我们为了不在吃苦头, 我们需要为各层留个header的空间.
我们先来看看skb_reverve 和skb_put 这个两个函数都修改了tail这个成员
那么这个data 和tail代表什么涵义呢? 代表实际有效数据的起始和结尾!
这个语义非常重要, 必须理解记忆!
我们如何知道改刘多少呢? 另外, 实际上我们先加头和先加数据都是可以的.
读了上面的design philosophy你就应该知道, transport和network层原来
是一个东西.对于头部大小的确定, 显然, 如果不是固定的大小,
由不同协议自身来定使最好的, 像tcp 和ip这种时不时还毛出个option, 显然
你是无法在最开始旧reserve出来空间大小的. 那copy用户态数据也要延迟了....
ip_ufo_append_data函数中我们可以看到skb_reverve是为linker header留了空间.
而udp和ip是通过skb_put来计算tail的偏移.
skb_reserve的comment上面写明只能用在没有有效数据sk buff.
现在, 我们看看skb_push与skb_put的比较语义研究.
skb的data和tai标志有效数据的起始位置.
skb_push关注data, skb_put关注tail. push和put都有往里放的涵义.
push有promote 提升含义. 隐含扩大data的涵义.
put是常见的放在什么上的涵义.是建立在已有数据的接触上, 显然skb_put就是
做这样的事情, 放一个在旧的上之后增加tail
好吧这算是意淫.我只想让你知道data和tail的语义非常重要.
如果我们要是按照先添加报文header再添加数据, 就是一个个skb_put, copy操作.
如果你是事前reserve了那就是skb_push.
往前加是push, 后加是put, easy.
真实的协议栈header数msg的添加, 会有很多不同的实现, 也就是put和push多种使用方法.
就这样, 我门算是把头部和数据加到了skb中. 
有一个问题比如我们的照片太大了20MB, 而一个skb通常的内核肯定不能分除这么大那么.
就有多个skb的来存储了.
[Skbuffs - A tutorial](http://people.sissa.it/~inno/pubs/skb.pdf)
[Network buffers The BSD, Unix SVR4 and Linux approaches](http://people.sissa.it/~inno/pubs/skb-reduced.pdf)
[skb_shared_info](http://marc.info/?l=linux-netdev&m=115508038817177&w=2)
This "shared info" (so called because it can be shared among copies of the skb 
within the networking code)  --LDD3 17.10
关于[ frags 和 frag_list ](http://stackoverflow.com/questions/10039744/difference-between-skbuff-frags-and-frag-list/) 看我的答案
现在我们的skb 就填好了.
有一个问题当你把skb 加到某个队列时, 你应该主义到.skb用的不是标准的kernel style list!
而是
struct sk_buff		*next;
struct sk_buff		*prev;
这就是传统c风格(名字不确定, 我这么叫)的链表.
不用怀疑, 这里肯定有猫腻!
1. 不需要pointer -> entry转化了
2. skb的list操作多数需要返回entry, list_head是不支持的如skb_dqueue_tail.
所以, 自己动手丰衣足食.
在有了一个他填满poor network stack的报文后, 我们就可以做一些操作了.
实际存报文的内存叫skb data.其他的名字head rom, tail rom, paged data, head data
也应该知道.有了paged data整个结构看上去有点像章鱼, 所以有了head, 强行解释:-)
dataref标识的仅仅head data共享的次数也就是skb_clone.
而frags 是get_page, frag_list 是skb_get.
内核的命名方式, 有时糟糕透了, 完全不符合:

	概念性or推理性的认知.
skb->data_len这个成员就非常不直观!他是paged-data的长度.
data_len= len - (tail-data)
难道, data到tail这段就不是数据了吗? 非常杀脑细胞.
继续吐槽, skb_get,skb_put根本就不是一会事啊.
还是kref_get/put看着顺眼.
就好比标准库的strcpy, strlcpy, strncpy. 我们要看看内核这几个得到skb的函数using case.
首先看明白kfree_skb结合上面的函数分析, 你就知道这几个函数基本区别了.
了解这些函数的使用场景将成为, 本文的第二个亮点, 应为这以为着, 你讲对内核的数据流转
过程非常清楚.本文的第一个亮点, 揭示了网络协议是如何创建的.
本文还将有两个亮点!第三个是: 我将从内核提供的功能角度如napi, rps, netfilter这些角度.
解释为什么内核这么实现这些功能, 起始这些和协议就没什么关系了.
 第四个亮点, 我们将回到协议栈本身, 谈谈为什么linux内核网络栈是怎么实现的.
现在是周日凌晨1点09距离周一还有23个小时, 哎.
好, 我们开始探索协议栈的数据流也就是报文生命周期的问题.
为了更好的理解协议栈的数据流walkthrough, 我们先看看skb_get/clone/copy, pskb_copy这几
个函数的使用特征, 也就是什么时候该使用什么函数.
先从最简答的skb_get开始, skb_get背后的技术[reference counting](http://en.wikipedia.org/wiki/Reference_counting)引用计数.
严格说来, 引用计数不是一种synchronization方法! 对数据并发依然需要同步方法.
引用计数, 最常用在内存对象的生命周期管理, 在c里面, 对象有三种周期, auto, static, allocate.
引用计数就是管理allocate, 动态分配的heap上的内存. 引用计数, 保证数据的可访问.
但是访问共享数据依然需要同步方法避免竞态的出现.
所以skb_get, 使用场景readonly, 也就是不能修改报文的所有skb, skb data都不可以.
一个错误的使用场景:

	git show 18fa11efc279c20af5eefff2bbe814ca067
在dev_queue_xmit, 一进来就修改了skb->mac_header in skb_reset_mac_header().
接下来还有skb_update_prio(). 总之很多修改, 在skb_get之后, 再有两个以上的并发
访问修改skb, 就是并发访问, 因为所有的skb成员mac_header priority都是共享数据啊!
首先, 你使用skb_get就以为着你知道, 可定存在着另一个进程 or 中断会对这个skb进行访问.
使用skb_get的情况就是, 你能肯定, 其他流程, 包括你这个流程没有修改skb的内容, or
对并发访问做了良好的同步处理.显然后面这种并发的处理通常是不合适的, 一个skb的内容依靠
获取锁的顺序, 显然是令人难以接收的. 我现在是4.0的代码,通过cscope只看到了87次skb_get
的使用, 近35次是driver, net/irda占了30次, 另外net/nfc, net/llc占了13次, 
也就是说内核协议栈很少使用这个方法!我会把其他几个重点看下:
net/netlink/af_netlink.c

	if (skb_shared(p->skb)) {
		p->skb2 = skb_clone(p->skb, p->allocation);
	} else {
		p->skb2 = skb_get(p->skb);
	...
	netlink_broadcast_deliver(sk, p->skb2)//这个函数修改了skb
这段代码,如果被多个人共享, 显然要clone一个;反之, 到现在只有我们一个user.
按照通常, 我们就直接修改了, 想想这样对吗?你直接改了, 那么别人也可以这么干.
所以skb_get 第一种使用方法就是:

	如果不是共享的skb_get, 后直修改使用.
	如果是共享的, skb_clone!
	这么做的好处是减少了一次skb_clone, 只有在第二个人修改时才clone.
这是skb_get在修改的情况使用, 在这里我们使用的技巧是, 节省了一次clone的使用.
我们在这里使用了skb_get, 却做了修改. 这有反于, 我门通常认识的skb_get的使用.
但要记住这里不是并发访问!我们是可以修改的.为什么使用skb_get是为了在并发的情况
节省一次skb_clone.
貌似fast open很火啊
net/ipv4/tcp_fastopen.c tcp_fastopen_create_child()这个函数:

	if (unlikely(skb_shared(skb)))
		skb2 = skb_clone(skb, GFP_ATOMIC);
        else
                skb2 = skb_get(skb);
使用了同样的技巧!
同样代码出现在net/packet/af_packet.c tpacket_rcv()

第二种用法
net/core/skbuff.c skb_clone_fraglist这个函数
就是skb_get最原始的用法, 不修改这是保证对象存活.
net/tipc/msg.h <<tipc_skb_peek>>, 也是这种用法.
这种方法, 实际上是最为复杂, 因为他暗含着某处代码会把skb是放掉!
就看到这里吧, 或许, 还有其他的方法.

再看skb_clone, 上面也涉及到了.还是看些对比的用法.
tcp_transmit_skb这个函数:

	if (unlikely(skb_cloned(skb)))
                skb = pskb_copy(skb, gfp_mask);
        else
                skb = skb_clone(skb, gfp_mask);
用到了类似的skb_get的模式.
skb_shared 很好理解users > 1 is true.
skb_cloned 就有点复杂了:

	return skb->cloned &&
               (atomic_read(&skb_shinfo(skb)->dataref) & SKB_DATAREF_MASK) != 1;
这里的问题是, 我们为什么不能通过skb->cloned表明是否是cloned的skb, 我们知道
skb_clone会n->cloned = 1;把new oldskb都置cloned标志.
这里还检测了dataref是否>1. 我们摘掉clone后dataref是被加1的, 那什么情况cloned有,
但dataref不是< 2;那就是clone后一个报文kfree_skb了, kfree_skb虽然能释放一个clone出
来的skb却对另一个clone的skb爱莫能助. 所以虽然, cloned标志位存在但clone实质已无;
Got it?回到tcp_transmit_skb, 这里如果我们没有cloned 就clone一个如果cloneed.
这是什么逻辑?这里的差异是否创造一个skb head data的副本. 
答案在这里[pskb_copy() in tcp_transmit_skb()](http://thread.gmane.org/gmane.linux.network/206211/focus=206215)
我们仅从pskb_copy 和skb_clone函数的语义是不可能知道这个答案, 这超出了函数能表达的
涵义范围, 此时我们只能了解整个函数栈的数据流图才行.
tcp_transmit_skb我们要对skb 和skb data 都要修改, 这是目标.
如果cloned我们对skb data修改势必造成早先cloned出来的skb corruption!可能里面的
各种header指针不对了.在AF_PACKET这种情况就是, tcpdump希望收到原始的报文, 因为
我们的retransmit是修改了skb data造成AF PACKET收到的将不是他希望的就报文, 有可能
tcpdump收到的是一个修改一般的报文, 更糟.
什么也不管直接pskb copy, consume掉就的skb, 是可以的.所以这里还是换汤不换药,为了
节省一次pskb copy!
这里我们发现了三条数据流, 发送, 重传, af packet, 下面会再次探索.
所以这些skb_get/clone/copy, pskb用法:

	都是结合skb_cloned skb_shared一起用的, 当然不绝对如
	skb_get就可以用引用计数器那种. 具体怎么用还是要结合代码进行分析的!
	貌似skb_copy 和 pskb_copy, 应该是一个差不多的用法见skb_unshare这个函数.

还有一个问题, skb_get和 skb_clone pskb_copy, 这三种算法如果重叠着用会出现什么问题?
比如先skb_get了一下, 之后有clone了.是完全不相干的逻辑, 在skb_clone时考虑到这点所以
调用的是atomic_set(&n->users, 1);, 没有直接复制.

也差不多该分析, 协议栈的数据流图, 在这之前先挑重点总结下linux/skbuff.h里面的函数
先整体分析下类:
申请skb 空间: alloc_skb, build_skb, __napi_alloc_skb, __alloc_rx_skb
frag fraglist相关:
skb释放: kfree_skb, consume_skb等等
协议相关的: vlan checksum, memcpy_from_msg等
链表操作:
操纵skb 和skb data: get clone copy put push
skb成员赋值的操作: 比较多余, 可能比较工整写, 模块化.
还有一些比较附在的操作函数, 在这里分析一下:
* skb_pull
和skb_push, 相对, 应该是拆包的时候用的.
* __skb_trim: 重新设置了 len.不是坚守也可能变大.非线性什么不做, 给个警告.
* ___pskb_trim: 考虑了paged data.
* skb_header_pointer: 这个函数在smartqos里面用过, 主要就是考虑大nonlinear区的问题.
用来在skb去一块数据, 如果数据大小在headlen里面最容易了, 直接返回data + offset.
否则skb_copy_bits
* skb_copy_bits: WARNING fraglist藏在skb_walk_frags里面.
核心就是一个frags的for循环一点点小心翼翼的copy就行了. 
要注意这里有个kmap_atomic的操作!这是加深对page理解的千载难逢的机会.
kmap_atomic如果是直接映射区的页表由cpu主动完成.从一个page里面copy东西的时候.
低端页返回地址, 高端页映射到kmap_pte.之后开始copy.copy完了kunmap_atomic.
这里的问题是为什么要在这里做这种映射?
可能是用户态的高端page传到内核, __ip_append_data里面的__skb_fill_page_desc
会把page存到frags[i]里面.可能在进程上下文这些页面ok, 但是到了内核态切不是相关
进程的上下文, 这时候高端页,的映射页表就不对了, 因为每个进程的页表是不同的, 所以
要用kmap_atomic来一下.
* page_address这个函数如果已经映射page_solt里面取, pas保证了多个vaddr 映射到paddr.
继续看, 下面看三个非常复杂的三个函数, 好吧是因为我之前没看过.
___pskb_trim, pskb_pull, pskb_expand_head; 
* 最核心的就是pskb_expand_head.
先把他看了, 函数注释说sk_buff不变, 且返回后需要重新reload. 改的是skb 的head data.
如何sk_buff shared BUG()!显然pskb是针对private non-shared, 如果针对shared改了那么
别的执行流上sk_buff的成员都失效, 有可能coredump!所以直接BUG()了.
还有nhead < 0 直接BUG, why? 因为下面的memcpy
显示申请新空间, ok.之后旧的数据copy过来. ok
之后是shinfo, 为了性能优化考了nr frags个, 屌.
这里copy shinfo时dst是data+size, 这个size是slab的对应obj的size不是
上面申请时的size为什么这么做? obj 的size 应该大于申请的size.
这里是尽量利用所有空间了.更新下sk_buff相应成员OK了.
期间还处理skb_cloned的情况为什么? shared直接BUG
如果cloned则在多个sk_buff之间共享frags, skb_orphan_frags, 先把frags copy到现申请内核page
之后put user page, 在把内核page装入frags, 在get 内核page.
expand之后, 包括frags旧都是内核数据了.之后把旧的skb head data数据释放掉.
也就是说, 为什么我们不能直接把用户的frags, fill到新的head data的shinfo呢?
* skb_orphan_frags 大哥问号.
反正expand后head 大了, frags 内核page了, frag list 也get了.
* pskb_pull:我不看了, 函数注释很明确.
     The function makes a sense only on a fragmented &sk_buff,
       it expands header moving its tail forward and copying necessary
       data from fragmented part.
* pskb_trim:基本上看懂就是加上了paged 数据的处理
* skb_unclone: pskb_expand_head(skb, 0, 0, pri);
* skb_copy:之后全都线性了.  pskb_expand_head(skb, 0, 0, pri); identical to old data.
其他的以后再看吧, 这些够用了.
## fclone -- fast clone
[NET Implement SKB fast cloning.](http://git.kernel.org/cgit/linux/kernel/git/torvalds/linux.git/commit/?id=d179cd12928443f3ec29cfbc3567439644bd0afc)
[Fast SKB cloning, continued](http://lwn.net/Articles/140552/)
use in skb_clone function
use case 1: tcpdump and network stack
fclones->fclone_ref 这就是引用, 用处见skb_clone
skbuff_head_cache alloc的skb对应n->fclone = SKB_FCLONE_UNAVAILABLE;
* pskb_pull -- p abbrivated from oprivate
* truesize -- len of sk_buff + head_len + frags + frag_list
* data_len -- len of frags + frag_list
* len -- head_len + frgas + frag_list


正文开始
# Linux network stack workthrough
skb的流向和socket有关skb就是在socket中流的.
所以找到socket就行了.
[DevConf 2014 Kernel Networking Walkthrough](http://www.slideshare.net/ThomasGraf5/devconf-2014-kernel-networking-walkthrough)
[introduction to linux kernel tcp/ip ptocotol stack](http://www.slideshare.net/minibobo/linux-tcp-ip?related=1)
taobao的第5也说明了问题.
这是通常的skb的流向. 就是在socket里面按着协议走, 包括tcp的重传.
还有一种, 就是想kproxy那种, 人为的干扰skb的走向, netif_receive_skb就是一个点.
netif_receive_skb之后就是标准的内核协议栈的事情了包括bonding啊, vlan, bridge这些什么的.
我觉得这么说还是不够深度, 我们确实在探索skb在协议栈中的流转.
我们都知道协议栈中skb按着协议走的, 如果能指出什么时候我们可以合法地让报文转个向.
就能打到我们的目的, 多少能提升下对workthrough的理解的深度;)
* af_packet相关的
dev_queue_xmit的dev_queue_xmit_nit中clone后deliver_skb送上去.
netif_receive_skb 的__netif_receive_skb_core 的deliver_skb. 有个问题?
为什么skb直接送上去了没有skb_get之类的.原来每个deliver_skb都有
atomic_inc(&skb->users);为什么不是skb_get
* 主动调用netif_receive_skb 
很多pptp协议就是这么干的.
其实最经典还是pskb_copy和clone的那个场景!
这个应该多积累, 我感觉挺重要的.
[Understanding TCP/IP Network Stack & Writing Network Apps](http://www.cubrid.org/blog/dev-platform/understanding-tcp-ip-network-stack/)
这篇文章的介绍很好.
好吧我是有这个传统的很早以前, 我就喜欢这么搞....
## out
inet_stream_ops->tcp_sendmsg()->tcp_push()->__tcp_push_pending_frames()->tcp_write_xmit()->tcp_transmit_skb()->ipv4_specific.ip_queue_xmit()->
ip_local_out()->__ip_local_out()->NF_INET_LOCAL_OUT->dst_output()->
ip_output()
{
	//set in ip_mkroute_output
	skb->dev = dev = skb_dst(skb)->dev; //!!!
	skb->protocol = htons(ETH_P_IP);
}->NF_INET_POST_ROUTING->ip_finish_output()->

ip_finish_output2-> dst_neigh_output
{ 
	neigh_hh_output // hh already in below:-)
	or 
	n->output = neigh_resolve_output{dev_hard_header}
}
->dev_queue_xmit()
{

	__dev_xmit_skb->__qdisc_run->qdisc_restart()->dev_hard_start_xmit()
	or 
	validate_xmit_skb->skb_gso_segment->skb_mac_gso_segment-> ptype->callbacks.gso_segment=inet_gso_segment->tcp4_gso_segment,
	dev_hard_start_xmit()
}
xmit_one->
{
	dev_queue_xmit_nit is Sun's Network Interface Tap (NIT)
	netdev_start_xmit->ops->ndo_start_xmit{this functions is init in createing device} = e100_xmit_frame
}

softirq:net_tx_action()->qdisc_run()

## in & forward
* NAPI poll_list net_device
driver intr add skb to private queue -> e100_intr()->__netif_rx_schedule()->__napi_schedule(netdev,nic->napi)->:
add napi to poll_list and __raise_softirq_irqoff()
do_softirq->net_rx_action()->
+netdev->poll()=e100_poll()private function->e100_rx_clean()...->
netif_receive_skb()->deliver_skb->
private queue and private function

* Non-NAPI input_pkt_queue skb
driver intr vortex_rx()->netif_rx()->add skb to SD input_pkt_queue->napi_schedule(backlog)->add backlog to SD poll_list __raise_softirq_irqoff()
async:net_rx_action()->
+backlog->poll()=process_backlog()
->netif_receive_skb()->deliver_skb->
skb to sd input_pkt_queue process

* common path
pt_prev->func=ip_rcv()->NF_INET_PRE_ROUTING->ip_rcv_finish()->
ip_route_input()->ip_route_input_slow()
{
	local_input dst.input??=ip_local_deliver()
	or
	ip_mkroute_input()->__mkroute_input():dst.input=ip_forward() 紧接着dst.output??=ip_output()
}
dst_input()->
{
	ip_local_deliver()->NF_INET_LOCAL_IN->ip_local_deliver_finish()->inet_protos.tcp_v4_rcv()
	or 
	ip_forward()->NF_INET_FORWARD->ip_forward_finish()->dst_output()见上。
}
* Differences
1 NAPI has not netif_rx():input_pkt_queue.
2 NAPI and Non-NAPI used different napi->poll 决定本质上的区别。
3 vortex_rx() 多，e100_rx_clean()多！这点可以看出不同优势来。

* Need clean
net_tx_action->output_queue/每个设备的qdisc and  clear_bit__QDISC_STATE_SCHED qdisc_run add back
__QDISC_STATE_SCHED是否加入softdata
qdisc_restart: 如果队列有数据就返回大于零 继续减小weight_p
__qdisc_run queue no data __QDISC_STATE_SCHED not set, only in this case!
driver tx, stack xmit

# Linux network technical feature
内核协议栈(包含底层设备链路层)提供了很多技术机制, 比如
SG I/O, TSO, RPS, NAPI等等. 这些技术的目的都是什么呢?
这个问题重要到, 如果你知道了他, 这些技术就不再那么高深么测了, 神秘感全无.
最为重要的就是, 你再也不用为了记住这些技术而头疼了.
首先协议栈本身, 就是完成信息交换这个简单的目的. 搞出那么多名堂来是为什么呢?
就内核提供这些技术而言, 基本上都是为了提高性能!
我严格区分性能提高, 功能实现. 虽然很多技术的疆界不是那么明晰.
## SG IO
如果device support NETIF_F_SG 直接copy_form user msghdr to frgs[] zero copy!
p_append_data
这是设备的一个feature. 内核和协议栈只是小角色, 边角料.

[NETIF_F_FRAGLIST and NETIF_F_SG difference](http://thread.gmane.org/gmane.linux.network/153666)
validate_xmit_skb()->__skb_linearize()
ip fragment 不是为了fraglist而是把skb变小. 所以这里可能有问题linearize后skb过大.
如果经过ip_fragment应该,不会出现, 自己倒腾的就可能.
compound page


## offload
* TSO in tcp_v4_connect
[TSO Explained](https://tejparkash.wordpress.com/2010/03/06/tso-explained/)
One Liner says: It is a method to reduce cpu workload of packet cutting in 1500byte and asking hardware to perform the same functionality.
* GSO
[GSO: Generic Segmentation Offload](http://thread.gmane.org/gmane.linux.network/37287)
TSO = GSO_TCPV4
frags = sg I/O
frag_list
*GRO
napi -> dev ->inet->skb

# INET 
现在我们来看具体的network stack的实现.
linux kernel的tcp/ip实现是有自己的名字的就叫INET!
An implementation of the TCP/IP protocol suite for the LINUX operating system.  
INET is implemented using the  BSD Socket interface as the means of communication 
with the user level. 
## 内核协议栈的代码可以分为:
1. 协议相关, bsd socket也算是吧, qdisc也是.
2. 内核提供的基础架构skb
3. 内核的优化rss, rps, gso, napi之类的.
4. 增强功能af_packet, netlink, netfilter, 不属于协议的, 也算不上内核的东西,只是
一个外界的需求抓包哎, 防火墙之类的.
接下来要看看具体的linux TCP/IP network stack的实现了.
有些实现看似夹着协议的前缀如__ip_append_data实际上内核优化frags的体现, 不要眯了眼.
但是, 通过这些隐含的功能去探索, 标注,理解代码却非常赞!
## 协议栈运行的本质?
出去一层层依据协议类型和参数[Encapuslation](http://en.wikipedia.org/wiki/Encapsulation_\(networking\))
进来一层层decapuslation 报文头部, 根据头部, 协议, 还有参数进行操作.

## package name in different layer
An individual package of transmitted data is commonly called a frame on the link layer, L2; 
a packet on the network layer; a segment on the transport layer; and a message on the application layer.


## FIXME Implemention of protocols
* inet_create
sock->ops = inet_protosw->ops = inet_stream_ops
* proto_ops -- fops 
is a good name stand for all PF_*, all 协议族, but sock_generic_ops is better 具体协议与BSD socket api的通用接口
* proto, -- specific fs, like ext,  btfs in *inetsw*
sock的lab决定具体的slab, 如tcp_sock/udp_sock, 根本的发送方法tcp_sendmsg, 协议的真正实体!
* 越来越具体
BSD socket api ->proto_ops(sock type base)协议通用api ->proto (udp/tcp_prot)
sys_bind -> inet_stream_ops ->inet_bind ->sk_prot->bind(likely, is NULL)
write->inet_stream_ops->sendmsg->tcp_sendmsg
* inet_connection_sock_af_ops
icsk->icsk_af_ops
* net_protocol -- l4 rcv in *inet_protos*
是iphdr中protocol成员的延伸, 所以有了tcp_protocol/udp_protocol all in inet_protos
* packet_type -- l3 rcv in ptype_all and ptype_base
pt_prev->func



# BSD socket layer
Details and skills in Stevens Unix network programming.
我们现在开始探索linux 网络协议栈的socket layer.
以socket()这个系统调用开始.
我门首先要了解什么是socket. 就好像, 小时候拿两个纸杯中间用线连起来,
模拟电话传消息一样. socket 起始也这么会事, 可以说socket也是一种
communication protocol, 和network layer 一样地址就是最重要的, 也是第一位的.
What is socket in wikipedia?
A socket is one endpoint of a two-way communication link between two programs 
running on the network. An Internet socket is characterized by at least the following :
Local socket address: Local IP address and port number
Protocol: A transport protocol (e.g., TCP, UDP, raw IP, or others).
Remote socket address, if connected to another socket.
我们在用户态这头有endpoint, 套接字的一头. 还需要另外一头.
没错!我们要找到另外一头. 怎么找? 比如你和小伙伴, 那么纸杯+线的就够了.
如果你要给异地的情人 or 亲人通话, 那个破纸杯肯定不够了. 这时候你就需要真的电话了
玩cs野战的时候, 对讲机就够了.所以说, 从通信手段上就决定了我们的另一头的位置.
这样说有些本末倒置, 毕竟, 你是先有了说话的对象之后决定具体用什么方法.
那在网络世界, 所谓的说话对象是什么呢? 其实是, network layers.
正是layer, 才是网络世界的真正实体, 比如抓包你像和自己的linker层建立一个socket
电话线, 用来私语.比如ping你想和另外一台主机的network 层眉目传情.
最常见的就是应用层的信息交互, 也就是常见的tcp or udp socket.
所以回头来看看我们的socket系统调用:

	int socket(int domain, int type, int protocol);
你看到没有socket的第一个参数是叫domain, 不是狭隘的protocol协议族之类的!
什么叫domain, 就是领域, 范围的意思, 这完全符合socket作为一个工具的性质!
你要先确定你沟通的范围, man手册给出的例子, 注意这里是以AF开头, 明白了吧:

	       Name                Purpose                          Man page
       AF_UNIX, AF_LOCAL   Local communication              unix(7)
       AF_INET             IPv4 Internet protocols          ip(7)
       AF_INET6            IPv6 Internet protocols          ipv6(7)
       AF_IPX              IPX - Novell protocols
       AF_NETLINK          Kernel user interface device     netlink(7)
       AF_X25              ITU-T X.25 / ISO-8208 protocol   x25(7)
       AF_AX25             Amateur radio AX.25 protocol
       AF_ATMPVC           Access to raw ATM PVCs
       AF_APPLETALK        AppleTalk                        ddp(7)
       AF_PACKET           Low level packet interface       packet(7)
       AF_ALG              Interface to kernel crypto API
AF_UNIX/LOCAL: 你在同一台电脑上的应用层通信.
AF_NETLINK: 应用层和内核通信(linker, ip, tcp等多层, 甚至是非网络的内容).
AF_APPLETALK: 这个主要是苹果的设备进行通信, 具体不了解.
AF_PACKET: 这个就是dev_queue_xmit和netif_receive_skb照顾的接口, 应用层与linker层通信.
AF_INET: 这个是应用层与internet网络上的主机进行通信, 范围很广,遍布互联网,多个层.
现实中通信质量是有区别的, 就好比你是用上世界哔哔机, 大哥大还是现在android 苹果.
socket的第2个参数, 就是确定通信质量的.
       SOCK_STREAM     Provides sequenced, reliable, two-way, connection-based byte streams.  
			An out-of-band data transmission  mechanism  may  be supported.
       SOCK_DGRAM      Supports datagrams (connectionless, unreliable messages of a fixed maximum length).
       SOCK_SEQPACKET  Provides  a  sequenced,  reliable, two-way connection-based data transmission path 
			for datagrams of fixed maximum length; a consumer is required to read 
			an entire packet with each input system call.
       SOCK_RAW        Provides raw network protocol access.
       SOCK_RDM        Provides a reliable datagram layer that does not guarantee ordering.
       SOCK_PACKET     Obsolete and should not be used in new programs; see packet(7).
SOCK_RDM完全没有听过啊, 不过注意RDM表示reliable, 但是不代表有序ordering, 
也就是说, 包不会丢失有重传, 但收到顺序不保证.这里想说的是, 一个socket的各种性质是分开的.
而这些性质就是所谓的通信质量! You buy a bog, 顺序反了就成a bog buy you.
我见过的就是stream, dgram, raw三种.
现在我们来探索下所谓的raw和stream, dgram到底有什么区别.这些都是非常基本的概念, 
之前都被我忽略掉了.
man手册上 A raw socket receives or sends the raw datagram not including link level headers.
加上对于af packet我们知道他是在dev_queue_xmit和netif_receive_skb处得到, 所以这个raw是相对来说.
他包含了一些协议的头部, 但同时限于ip往上的头部.
而stream和dgram. 看上去和dgram很像, 但raw可能收到重复的packet而dgram缺不会(就原始协议来讲)
ping就是用的这种socket:

	socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)
为什么要用这种, 我说的协议的语义上.stream和dgram不行吗?
原因是ping连接到的是icmp 他是在network层. 而另另外的stream 和dgram都是封装了
transport layer的内容后直接到的network 层的ip, 我们没有办法访问到icmp协议.
raw的另外一个特性, 就是允许跨国transport layer自己构建包.
这让我想起了电影里面钻到电线里面的情节, 没错你的能力够你也可以.
从没有保证这点看raw和dgram很像. 而dgram就是传输层的raw!
下面我们来看看, 到底一个高质量的通信线路是什么样的, 具有什么性质.
stream 是一个非常重要, 且牛x的概念, 我在I/O部分, 解释过.
这里简单说一句, stream最牛b的地方在于他在数学上是有专门的定义!
那么在这里stream 表示的是复合的概念, 对于tcp实现就是:
connection-oriented, reliability, error-check, flow control, congestion control
记住这五个概念这是所有高质量传输的共性.
接下来简单的说一下. 

## connection-based
到底什么是面向连接的网上只有wikipedia给的解释最合理, 其他的扯到了别的性质.
两点: session and in order.

* Session
In computer science, in particular networking, a session is a semi-permanent 
interactive information interchange. 
session，中文经常翻译为会话，其本来的含义是指*有始有终*的一系列动作/消息
[Instance of tcp session in BSD socket](http://www.scottklement.com/rpg/socktut/overview.html)
[TCP Session - Handshaking in protocol](http://www.dummies.com/how-to/content/network-basics-tcp-session-establishment-handshaki.html)
这个对应tcp的三次握手, 4次挥手, 
* in order
涉及到的另外一个概念Virtul circuit
A virtual circuit (VC) is a means of transporting data over a packet switched
 computer network in such a way that it appears as though there is a dedicated 
physical layer link between the source and destination end systems of this data. 
对应tcp 的接收队列, seq number

## Reliability
sliding window
ARQ(go back n)
显然seq number这些也是需要的.

## error-check
checksum

## flow control
qdisc

## congestion control
cubic, reno这些.
slow start

之于第三个socket()参数嘛, android还有国产天语阿里云和google nexus 6之分.
socket的参数protocol不是指trnasport layer,而是domain的一个instance(ETH_P_IP)
另外socket的第一个参数被称为domain而不是协议族, 暗含像PF_PACKET这种定义.
但实际上PF_Packet只是一种socket, 参见man 7 packet.
PF前缀这里体现了内核定义的混乱! 他不符合protocol layer的定义, 所以不是protocol!

介绍完socket()接口后我们进一步, 来看看这个socket()产生的具体'纸杯线'
也就是所谓的sock, 文章的质量是不能降低, 不仅意味着, 不能吸引到读者更重要的是
浪费了自己的时间. 所以再开始sock的探索前, 要明确我们要的是什么? 这很难.
FIXME, 我不知道, 先看着吧.好吧先看workthough, 之后看下address, 就完了.就这么定了.
显示alloc sock: sock_alloc 这和alloc_skb没什么本质差异.这个socket就是[霸王的卵](http://movie.douban.com/subject/5308201/)
这里有个问题内核使用sockfs来完成socket结构的申请, 把sock和一个inode放到一起了.
为什么? 这事为了使其它的read write close之类的也能对socket fd生效.
要知道网络这几个接口, 脑袋是有反骨的, 死活没有融合到unix的哲学当中:

	一切皆文件
愣是多了几个socket, bin connect, accept才降服, open弱爆了.
[Linux Sockets and the Virtual Filesystem](http://isomerica.net/~dpn/socket_vfs.pdf)
这一句非常屌:　

	sock->type = type;
申请完sock后就是, [incarnation](http://en.wikipedia.org/wiki/Incarnation) a sock, 这个需要dominator的帮助.
domainator在内核叫net_proto_family, 也没什么错. 因为角度不同:

	struct net_proto_family __rcu *net_families[NPROTO] __read_mostly;
通过sock_register注册一共三十几个全在这呢, cscope全能看到.这表明我们还在sock层.
挑几个重要的:inet_create, netlink_create, unix_create, packet_create
到现在你应该明白, 整个socket就是一个[受肉仪式](http://movie.douban.com/subject/7051375/)
妈妈生孩子是受肉, 格里菲斯鲜红的贝黑利特也是.我们先看inet_create()
这里用到了inetsw(和inetsw_array本质一样)是个链表数组表头是SOCK_RAW/STREAM/DGRAM/MAX
这些, node是inet_protosw结构包含proto, type, protocol等.
proto就是protocol对应的协议, 这个必须记住!
struct proto_ops inet_stream_ops.这里是对应的sock的操作. 我们思考一下.
感觉上我们只要有一个proto就够了, 为什么多了一个proto_ops为什么?
我直观上认为proto 本身就是ops了!
实际上这些proto_ops是proto的抽象, 中间层, 最终还是要调用proto, 如tcp_prot
只能怪proto_ops名字起地不好.更好的名字是:
proto_ops -> inet_ops(内核里真没这个名字)
proto -> proto_ops
算了.
受肉开始:

	sock->ops = answer->ops;sock是socket, ops是proto_ops如inet_stream_ops
此时socket对应的是domain和type抽象结合的ops, 如inet_stream_ops. 这就是亮点.
实际上是属于socket layer的ops!应为落脚点是stream/sockraw这些
接下来申请真正的sock结构

	sk = sk_prot_alloc(prot
交织的线
socket -> sock
proto_ops -> proto
sock这个结构是真正属于domain的, 不同于socket. socket和sock是指针的关系.
而sock和tcp/inet sock这些是千面神的关系, 一个本体.
申请后就是处世化, 先是inet 层, 之后是具体的proto 如sk->sk_prot->init

	tcp_v4_init_sock
这是和协议相关的, 这里inet_connection_sock *icsk = inet_csk(sk);
先获得tcp connect的属性的sock, 初始化这个ops:

	icsk->icsk_af_ops = &ipv4_specific;
我们详细看看, 这个函数的语义, icsk_af_ops这里指定了, 所谓的基于连接的
socket的ops方法.这是什么语义呢? 或者说为什么来了个这个.
为什么叫这个名字, 首先能确认的是icsk_af_ops都是和ip相关的也就是"地址"了.
显然纵观tcp的5个属性只有icsk这个是和地址关系最为密切的!所以network层相关的
放到这里ok!
我们看到了一个熟悉的面孔tcp_transmit_skb() 就是skb_clone和pskb遇到的.
这个函数调用了icsk->icsk_af_ops->queue_xmit=ip_queue_xmit
这下子就全明白了, tcp的下面的疆界是icsk_af_ops!!!
这也是ip的起始之地, 轮回的广场!
我们来回忆一下整个受肉的过程.
先是sockfs那里申请socket 拿到proto_ops.
之后是申请sock, inet_sock初始化, tcp初始化和icsk->icsk_af_ops = &ipv4_specific;
完了.
socket和inode放到一起.
下面几个tcp/inet/icsk sock 一初始化就完了. 起始很简单.
接下来, socket和fd关联下就完了, 这就是walkthrough啊:
sock_map_fd这个函数是真正关联read和socket的!

	sock_alloc_file 关联socket_file_ops到file!read就是这里取的.
看下connect
sock = sockfd_lookup_light(fd,
完了, sock是socket.

看下read
file->f_op->read_iter =sock_read_iter-> sock->ops->recvmsg
其中struct socket *sock = file->private_data;
那么这个inode有毛用啊?可能是inode保存了, mode, u/gid为了权限吧.貌似这样.FIXME!
好吧这样 bsd socket layer就结束了.
显示申请socket 和inode 初始化proto_ops.
申请sock附上proto, 之后是inet_sock, 再是具体协议相关的tcp_sock,
 和icsk_af_ops = &ipv4_specific
之后是file结构和对应socket_file_ops没了, 真的完了.
换了raw 和icmp呢?kao,竟然就叫ping_prot, 这他妈逼格也太高了吧.
直接通过socket_file_ops连接到了ping_v4_sendmsg 直接ipv4了
收的化是用户态&sk->sk_receive_queue. ping_recvmsg->skb_recv_datagram
底层是icmp_rcv-> icmp_pointers->ping_rcv ->sock_queue_rcv_skb到sk_receive_queue
ping可以用dgram和raw两种方法实现.
看来这就是netstat看不到ping的原因.虽然不是端口
发送的时候inet_sendmsg -> ping_get_port->inet_num, 竟然是ping table比较ping的地址
再看一言PF packet
po的名字太恶心叫 p_sock 不好吗?
        po->prot_hook.func = packet_rcv;

        if (sock->type == SOCK_PACKET)
                po->prot_hook.func = packet_rcv_spkt;

        po->prot_hook.af_packet_priv = sk;
这就是packet的真相, 实现的不错!

那么socket layer还有什么
sk_backlog_rcv 是在!sock_owned_by_user(sk)调用的.貌似用户态在ioctl吧.
暂时backlog 收包.那么l2tp_ip_recv 调sk_receive_skb->他 做什么用呢?
原来是所谓的l2tpcontrol报文啊.
proto是l2tp_ip_prot
也就是说sock 层可以暂存报文.
没了
感觉sock就是初始化sock, 之后收包找sock, 发包ping需要bind port就完了.
收发包用什么队列还是proto自己说了算的.

## FAQ
### socket lock
[lock_sock or sock_hold](http://www.spinics.net/lists/netdev/msg136306.html)
[bh_lock_sock](http://www.linuxfoundation.org/collaborate/workgroups/networking/socket_locks)

* FIXME sock->pfmemealloc
Yes, I only wanted to drop the packet if we were under pressure
when skb was allocated. If we hit pressure between when skb was
allocated and when __netdev_alloc_page is called,
+in sk_filter
[netvm: Allow skb allocation to use PFMEMALLOC reserves](https://groups.google.com/forum/#!msg/linux_net/-YtWB66adxY/Qqm_y4U09IAJ)
[netvm: Allow skb allocation to use PFMEMALLOC reserves - gmane 08/14](http://thread.gmane.org/gmane.linux.kernel/1152658)

socket是跟协议族绑定的概念, 所以要用inet_create, netlink_create

* FIXME inet_timewait_sock 
deal heavily loaded servers without violating the protocol specification 

* sk_set_memalloc
SOCK_MEMALLOC, sock has feature mem alloc for free memory.
只有到了sock层才能分辨, sock是否是memalloc的.
sk_filter
在socket layer, 我们看到我们有linker以上的收发包的能力, 内核栈还是很灵活的.

下面进入到核心tcp 和 ip协议.
#Transport layer
不涉及具体的协议内容:), 只是看看内核都做了哪些优化, 变种特工.
tcp的核心发包函数tcp_write_xmit and tcp_transmit_skb
上面主要是和cork和nagle有关
完了
在tcp协议的5点基础属性, 后世对tcp做了很多优化!


Details in l4.md
#Network layer
ip_append_data 和ip_push_pending_frames弄frag_list
ip_push_pending_frames -> __ip_make_skb & ip_send_skb ->ip_local_out
把&sk->sk_write_queue上的数据最后编程skb链表变成了, 还skb pull掉了潜在的ip 头部
第一个skb->frag_list的成员. 用的不太多啊.
ip_append_data中间出了以为如果可以ufo 那么就到frags的碗里去!
否则就生成一串skb挂到&sk->sk_write_queue上, 
Details in l3.md
# Data link layer
Details in l2.md
# Physical layer -- PHY
* Physical Coding Sublayer
* Physical Medium Attachment Sublayer
* Physical Medium Dependent Sublayer

#Net initialization
start_kernel-> parse_early_param irq timers softirq -> rest_init(): kthread
{
	do_basic_setup()  
	{
		driver_init
		sock_init
		do_initcalls()
		{
			net_dev_init: Initializing the Device Handling Layer
			{
				per-CPU 
				proc
				sysfs
				ptype_base
				dst_init
				softirq: net_rx/tx_action
				dev_cpu_callback: CPU hotplug.
			}
		}
	}
	free_init_mem()
	run_init_process()
}

## network init
inet_init()->ip_init()->ip_rt_init()->ip_fib_init()->fib_hash_init():create kmem_cache

## net device init
* net_dev_init
* nic init
e100_init_module	pci_register_driver:构建结构	driver_regiser:注册到内核	really_probe()drv->probe:初始化。
vconfig add		regiser_vlan_device：构建结构	register_netdevice:注册到内核	dev->init():初始化
