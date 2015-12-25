---
tags: cs
title: The art of Programming
date: 2015-02-27 15:46:14
category: cs
---
# Reference
[Programming tool](https://en.wikipedia.org/wiki/Programming_tool)
[Utility software](https://en.wikipedia.org/wiki/Utility_software)
programming principles
High cohesion low coupling
# Computer programming
Programming language theory
Programming language 
Compiler construction 

Coding style
[Programming principles](http://en.wikipedia.org/wiki/Category:Programming_principles)
Programming paradigms
Design pattern
Algorithms and data structures

Mathematics
Automata
Logic

Software engineering
Debuging
Testing
# Source code editor
vim easymotion
# Coding style
GNU coding standards, Linux kernel coding style, Shell coding standard
# Mathematics
Introduction to Mathematical Thinking - Devlin, Keith
数学思维是什么样的? 如何构建呢?
1.In mathematics, and more specifically in algebra, a domain is a ring such that ab = 0 implies a = 0 or b = 0.
Complement
radix complement
diminished radix complement
Codata
Collection
## Commenting
Copyleft/right,
Author and maintainer
Update log
Name of the module
Usage
## Label
It is considered to be safer that the label reflect what happens at the
destination, and not the reason for reaching the label. --Julia
# Programming paradigms
## defensive programming
Low tolerance against "potential" bugs
Assertion, panic in user space
How to use assertion
# Algorithms and data structures
Plz, reference algorithm.md
# Design pattern
1. Publish-Subscribe Mechanism 
2. Observation
# Code navigation tools
* ctags
ctags -R --regex-c="/^SYSCALL_DEFINE[0-9]?\(([a-zA-Z0-9_]*).*/sys_\1/"
* cscope
* GNU global
* LXR Cross Referencer
* Google code search
* How to find hook functions of a struct ops
For example, all start_xmit of ppp_channel_ops, 
pppol2tp_chan_ops, pppol2tp_xmit,
pptp_chan_ops pptp_xmit
pppoe_chan_ops pppoe_xmit
or grep -nr 'ppp_channel_ops' /path/to/{l2tp, pptp, pppoe}
* locate codes in complex program source files .
Bubble locate, begin with the fundermental system api for example socket(), bind(), connect()...then raise
# Review
# Build 
# Version control
* [git](http://git-scm.com/docs)
git ls-files -d |xargs -i git checkout {}
git log -S "RTN_UNSPEC" --source --all
* reverse find all the blame from start commit id to now
git blame --reverse 9e734775f7c22d2f89943ad6c745571f1930105f..   include/linux/netlink.h
* find kernel version of path
git describe --contains
git tag --contains <id>
* find commit id of a tag
git rev-parse v2.6.12-rc2
*  blame before a commit
git blame sha1^ -- path/to/file
* find commit after specific commit
git log  --oneline  --ancestry-path   c146066ab^^..HEAD  net/ipv4/ip_output.c
* find when line was deleted
git log -c -S'missingtext' /path/to/file
## git pull
git pull {repo} {remotebranchname}:{localbranchname}
git pull origin xyz:xyz
git pull next master:now
## git sendmail
shadowsocks
/etc/proxychains.conf
/home/firo/.gitconfig
proxychains git send-email --to dbaryshkov@gmail.com --cc rmk+kernel@arm.linux.org.uk --cc linux-pcmcia@lists.infradead.org  0001-Drivers-pcmcia-Fix-memroy-leak-in-sa11xx_drv_pcmcia_.patch
git send-email --smtp-encryption=tls --smtp-server=smtp.gmail.com --smtp-user=firogm@gmail.com --smtp-server-port=587 --to "kernel-janitors@vger.kernel.org" 0001-x86-Remove-unnecessary-void-pointer-casts.patch
##git diff
git -c core.whitespace=tab-in-indent diff --check
# Debugging
see debugging.md
# Testing
1.Black-box testing test software function.
2.White-box testing test software internal logic.
kvm  & supermin
# Mail
mutt
fetchmail
fetchmail -d0 -vk pop.gmail.com
msmtp
/home/firo/.msmtprc
procmail
