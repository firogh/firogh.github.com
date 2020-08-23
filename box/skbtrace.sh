#!/bin/bash
#
# Ahuthor: fyang@suse.com, firogm@gmail.com
# N.B: x86 64 only and based on perf trace

protocol=""

while getopts p:h opt
do
	case "$opt" in
	p)
		protocol="$OPTARG"
		;;
	h)
		echo "skbtrace -p ping"
		echo "Currently supports only ping application."
		exit 0
		;;
	esac
done
shift $((OPTIND - 1))

if [[ "$protocol" != "ping" ]];
then
	echo "Please run skbtrace -h."
	exit 1
fi

"$@"

ping_trace()
{



}

case "$protocol" in
ping)
	ping_trace();
	;;
esac

exit 0
