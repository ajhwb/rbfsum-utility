#!/bin/sh
#
# Script to insert rbfsum data to libra database
# Copyright (C) 2011 - Ardhan Madras <ajhwb@knac.com>
#

rbfsumdir=/srv/www/htdocs/rbfsum
rundir=/data/project/rbfsum-utility
lockfile=/tmp/rbfsum2db.lock
dir=''
name=''
filename=''


if [ ! -x $rundir/start-date ]; then
  echo 'Could not run program, aborting...'
  exit 1
fi

if [ ! -x $rundir/rbfsum2db ]; then
  echo 'Could not run program, aborting...'
  exit 1
fi

dir=`date +%Y-%m`
name=$($rundir/start-date $((`date +%s` - 86400)))
filename=$rbfsumdir/$dir/$name.txt

flock -n $lockfile -c '$filename > /dev/null 2> /dev/null'

if [ $? -eq 1 ]; then
  echo 'Another process is currently running, aborting...'
  exit 1
fi

exit 0

