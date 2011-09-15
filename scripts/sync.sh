#!/bin/sh
# Copyright (C) 2011 - Ardhan Madras <ardhan@mindotama.co.id>

lockfile=/tmp/sync.lock
target1=/nmx/mindo/rbfsum
target2=/nmx/mindo/rbfsum-xml
server1=rsync://172.19.0.82/mindo/rbfsum/*
server2=rsync://172.19.0.82/mindo/rbfsum-xml/

# make sure we have rsync installed
# 'which' print error message to stderr and success to stdout
which rsync &> /dev/null
ret=$?

if [ $ret -eq 1 ]; then
    printf "Please install 'rsync' first\n"
    exit $ret
fi

# try to acquire lock before process
flock -n $lockfile sleep 0
ret=$?

if [ $ret -eq 1 ]; then
    printf "Update is in progress, exiting\n"
    exit $ret
fi

# date && echo

flock -n $lockfile rsync -azuv --delete --delete-after ${server1} ${target1}
flock -n $lockfile rsync -azuv --delete --delete-after ${server2} ${target2}
