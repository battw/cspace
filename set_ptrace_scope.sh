#!/bin/bash
#Allows gdb to attach to processes
pts=/proc/sys/kernel/yama/ptrace_scope
if [[ $(cat $pts) -ne $1 ]]
then
    sudo bash -c "echo $1 > $pts"
fi
