#!/usr/bin/env bash

# zhm: below is simplest create method (can execute by hand)
# dd if=/dev/zero of=./disk1.img bs=1M count=1 seek=29
# dd if=/dev/zero of=./disk2.img bs=1M count=1 seek=29
# losetup /dev/loop1 disk1.img
# losetup /dev/loop2 disk2.img
# #add ==> filter = [ "r|/dev/loop1|", "r|/dev/loop2|" ] <== in /etc/lvm/lvm.conf
# pvcreate /dev/loop1 /dev/loop2
# vgcreate vgtst  /dev/loop1 /dev/loop2
# lvcreate -n lvtst vgtst -L 20M
# lvcreate -q --type snapshot -l 20%ORIGIN --snapshot --name "test123" vgtst/lvtst

DM_DEV_DIR=/dev

prepare_loop() {
	local size=$1
	shift # all other params are directly passed to all 'losetup' calls
	local i
	local slash

	test -f LOOP && LOOP=$(< LOOP)
	echo -n "## preparing loop device..."

	test ! -e LOOP
	test -n "$DM_DEV_DIR"

	#zhm only create loop0
	for i in 0; do
		test -e "$DM_DEV_DIR/loop$i" || mknod "$DM_DEV_DIR/loop$i" b 7 $i
	done

	echo -n .

	local LOOPFILE="$PWD/test.img"
	rm -f "$LOOPFILE"
	dd if=/dev/zero of="$LOOPFILE" bs=$((1024*1024)) count=0 seek=$(( size + 1 )) 2> /dev/null

	# Iterate through $DM_DEV_DIR/loop{0,1,2,3,4,5,6,7}
	for i in 0; do
		local dev="$DM_DEV_DIR/loop$i"
		losetup  "$dev" "$LOOPFILE"
		LOOP=$dev
		break
	done

	test -n "$LOOP" # confirm or fail
	BACKING_DEV=$LOOP
	echo "$LOOP" > LOOP
	echo "$LOOP" > BACKING_DEV
	echo "ok ($LOOP)"
}

prepare_ramdisk() {
	local size=$1

	echo -n "## preparing ramdisk device (size: ${size})..."
	modprobe brd rd_nr=1 rd_size=$((size * 1024)) || return 0

	BACKING_DEV=/dev/ram0
	echo "ok ($BACKING_DEV)"
	touch RAMDISK
}

prepare_backing_dev() {
	local size=${1=32}
	shift

	case $1 in
	"ramdisk" )
			echo "prepare_backing_dev ramdisk size: $size"
			#test ! -d /sys/block/ram0 && test "$size" -lt 16384
			test ! -d /sys/block/ram0 || echo "/sys/block/ram0 exist. exit!"
			test "$size" -lt 1024 || echo "input size ($size) should less than 1024M(1G)"
			# try to use ramdisk if possible, but for
			# big allocs (>1G) do not try to use ramdisk
			# Also we can't use BRD device prior kernel 4.16
			# since they were DAX based and lvm2 often relies
			# in save table loading between exiting backend device
			# and  bio-based 'error' device.
			# However with request based DAX brd device we get this:
			# device-mapper: ioctl: can't change device type after initial table load.
			prepare_ramdisk "$size" "$@" && return 0
			echo "(failed)"
			;;
	"loop" )
		echo "prepare_backing_dev loop size: $size"
		prepare_loop "$size" "$@" && return 0
		;;
	*)
		echo "unknow backing dev"
		;;
	esac
}

prepare_devs() {
	local n=${1:-3}
	local devsize=${2:-34}
	local devtype=${3:-loop}
	local pvname=${4:-pv}
	local shift=0
	local prefix=$devtype

	if [ $n -gt 9 ]; then
		echo "device number should less than 9"
		return 1
	fi

	touch DEVICES
	#zhm: there is only one backing device. then use dmsetup to divide into many virtual devices.
	prepare_backing_dev $(( n * devsize )) $devtype

	#echo -n "## preparing $n devices..."
	echo "## preparing $n devices..."
	local size=$(( devsize * 2048 )) # sectors
	local count=0
	rm -f CREATE_FAILED
	for i in $(seq 1 "$n"); do
		local name="${prefix}_$pvname$i"
		local dev="$DM_DEV_DIR/mapper/$name"
		DEVICES[$count]=$dev
		count=$((  count + 1 ))
		echo 0 $size linear "$BACKING_DEV" $(( ( i - 1 ) * size + shift )) > "$name.table"
		#echo "zhm prepare_devs call dmsetup, the table:"
		cat $name.table
		dmsetup create -u "TEST-$name" "$name" "$name.table" || touch CREATE_FAILED &
		test -f CREATE_FAILED && break;
	done

	if test -f CREATE_FAILED -a -n "$LVM_TEST_BACKING_DEVICE"; then
		echo "zhm prepare_devs failed"
		rm -f BACKING_DEV CREATE_FAILED
		return $?
	fi

	# non-ephemeral devices need to be cleared between tests
	test -f LOOP -o -f RAMDISK || for d in "${DEVICES[@]}"; do
		echo "zhm ==> device: $d"
		# ensure disk header is always zeroed
		dd if=/dev/zero of="$d" bs=32k count=1
		wipefs -a "$d" 2>/dev/null || true
	done

	printf "%s\\n" "${DEVICES[@]}" > DEVICES
	echo "created new dm DEVICE:"
	cat DEVICES
	#echo "ok"
	echo "*** done ***"
}


# e.g. to setup 10M ramdisk device
# lvmtst-setup.sh build ramdisk 2 10
# lvmtst-setup.sh clean ramdisk 
# lvmtst-setup.sh clean loop 
# lvmtst-setup.sh build loop 3 30

# theory:
# dd if=/dev/zero of=lvm0.img bs=1024k count=256
# losetup /dev/loop7 lvm0.img
# dmsetup create xxx => to create dm dev

ACTION=$1
DEVTYPE=$2
DEVNUM=$3
SIZE=$4

case $ACTION in
	"build" )
		echo "*** should add: filter = [ \"r|/dev/ram0|\", \"r|/dev/loop|\" ] in lvm.conf ***"
		dmsetup ls | cut -f 1 > DEVICES.orig
		prepare_devs $DEVNUM $SIZE $DEVTYPE && exit
		echo "(failed)"
	;;

	"clean" )
		#zhm: don't necessary to lvremove vgremove pvremove
		#test -n `pvs --nohead | grep pv1` || pvremove /dev/mapper/pv1 
		echo "dmsetup ls" && dmsetup ls
		dmsetup ls | cut -f 1 >> DEVICES.orig
		new_devs="$(sort DEVICES.orig | uniq -u)"
		#echo "will deleting devs: echo $new_devs"
		#above new_devs contains "\n", below echo convert to space splitted string
		dmsetup remove `echo ${new_devs}`
		if [ "$DEVTYPE" == "ramdisk" ]; then
			echo "clean brd module"
			rmmod brd
		elif [ "$DEVTYPE" == "loop" ]; then
			echo "losetup -D, delete test.img"
			losetup -D
			rm test.img
		else
			echo "clean brd module, losetup -D, delete test.img"
			rmmod brd
			losetup -D
			rm test.img
			dmsetup udevcookies
			dmsetup udevcomplete_all
		fi
		rm -vf BACKING_DEV CREATE_FAILED DEVICES DEVICES.orig LOOP loop_pv*.table ramdisk_pv*.table test.img RAMDISK
	;;

	* )
		echo "input: $0 build ramdisk/loop number size"
		echo "input: $0 clean [ramdisk/loop]"
	;;
esac

