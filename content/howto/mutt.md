---
date: 2015-04-25
title: 使用mutt收发邮件
category: howto
---

![Trigun](http://upload.wikimedia.org/wikipedia/en/5/5f/Trigun_manga.jpg)
###Reference
昨天用web Gmail给社区的maillist回信, 被退信了. 提示Gmail发送的邮件包含html.
这才想起来社区的mailist只接收:

> No MIME, no links, no compression, no attachments.  Just plain text

我之前发给社区的邮件是用git sendmail发给maillist, 不会有这个问题. 
有点不方便, 今天决定使用mutt.
需要mutt, fetchmail, procmail, msmtp, proxychains(天朝专用), vim/emacs
发一个邮件, 需要这么多程序啊, 以前一直以为一个mutt就够了, 还Gmail方便啊.
mutt的作用类似于胶水, 把其他程序联系起来.
fetchmail收, procmail过滤spam mail, msmtp 用来发送, proxychains 翻x墙,
vim/emacs用来编辑邮件.

###收邮件
其实, 并不像网上博客中说的那样, fetchmail 和procmail并不是必须的.
参考mutt官网的[UserStory/GMailOverIMAP](http://dev.mutt.org/trac/wiki/UseCases/Gmail)
你就可以想Web gmail一样实时推送邮件到你mutt了.
之后在.muttrc中加入:

	bind index G imap-fetch-mail
这样你就可以用G这个命令在mutt中主动同步gmail了.
此时翻墙用户启动mutt, proxychains会输出代理信息过滤到/dev/null

	proxychains mutt 2> /dev/null

###mutt 与发邮件
具体的配置看wangcong的[使用mutt处理电子邮件](http://wangcong.org/2007/03/09/-e4-bd-bf-e7-94-a8mutt-e5-a4-84-e7-90-86-e7-94-b5-e5-ad-90-e9-82-ae-e4-bb-b6-2/)

### 删邮件
在.muttrc配置如下，你在mutt中delete的mail在gmail的web上看不到了.
`set delete=yes`
可能需要在mutt中配置gamil trash, 求反馈.

###其他
* 国内gmail用户要用proxychains 做下代理, 这样就能使用gmail了:
`set sendmail="proxychains /usr/bin/msmtp"`

* wangcong的blog中的配置<不太符合习惯, 按着习惯和美感应该是:
`set indent_str=">"`

* mutt取消当前命令用ctrl g

* 靠gmail 存了两份发送邮件, 把这句注释掉, gmail自己本身会缓存的:
	`set record = "+[Gmail]/Sent Mail"`
