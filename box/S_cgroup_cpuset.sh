#!/bin/bash

CGROUP_CPU=/sys/fs/cgroup/cpu/slice
CGROUP_CPUSET=/sys/fs/cgroup/cpuset/slice
CPU=0

function run_sandbox {
  local CG_CPUSET="$1"
  local CG_CPU="$2"
  local CMD="$3"

  local PIPE="$(mktemp -u)"
  mkfifo "$PIPE"
  sh -c "read < $PIPE ; exec $CMD" &
  local TASK="$!"
  sleep .01
  mkdir -p "$CG_CPUSET"
  mkdir -p "$CG_CPU"
  tee "$CG_CPU"/cgroup.procs <<< "$TASK"

  tee "$CG_CPUSET"/cgroup.procs <<< "$TASK"

  tee "$PIPE" <<< sandox_done
  rm "$PIPE"
}

mkdir -p "$CGROUP_CPU"
mkdir -p "$CGROUP_CPUSET"
tee "$CGROUP_CPUSET"/cpuset.cpus <<< "0" 
tee "$CGROUP_CPUSET"/cpuset.mems <<< "0" 

run_sandbox "$CGROUP_CPUSET" "$CGROUP_CPU/cg-1" "stress-ng --cpu 1"
run_sandbox "$CGROUP_CPUSET" "$CGROUP_CPU/cg-2" "stress-ng --cpu 1"

read # click enter to cleanup
killall stress-ng
sleep .2
rmdir /sys/fs/cgroup/cpuset/slice/
rmdir /sys/fs/cgroup/cpu/slice/{cg-1,cg-2,}
