#!/bin/sh
#
# RbfSum generation script
# Copyright (C) 2011 - Mindotama Avia Teknik, Ardhan Madras <ardhan@knac.com>
#

rundir=/nmx/mindo
outdir=/nmx/mindo/rbfsum
xmldir=/nmx/mindo/rbfsum-xml
lockfile=/tmp/rbfsumgen.lock
dir1=/nmx/ringbuffer
dir2=/nmx/taurus/ringbuffer
start=0

do_cleanup ()
{
    rm -rf $lockfile
}

# Test and create our lock file
if [ -e $lockfile ]; then
    echo 'Another instance is already running, aborting...'
    exit
fi

touch $lockfile
if [ $? -ne 0 ]; then
    echo 'Could not create lock file, aborting...'
    exit 1
fi

# Sanity check
/nmx/bin/rbfsum > /dev/null 2> /dev/null
if [ $? -eq 127 ]; then
    echo 'Do you have `rbfsum'\'' installed?'
    do_cleanup
    exit 1
fi

# Check for working directory
if [ ! -d $rundir ]; then
    echo $rundir 'not exists, aborting...'
    do_cleanup
    exit 1
fi

# Check our start date and rbfsum2xml program
if [ ! -x $rundir/start-date ]; then
    echo $rundir'/start-date not exists, aborting...'
    do_cleanup
    exit 1
fi
if [ ! -x $rundir/rbfsum2xml ]; then
    echo $rundir'/rbfsum2xml not exists, aborting...'
    do_cleanup
    exit 1
fi

# For ringbuffer directories
if [ ! -d $dir1 ]; then
    echo $dir1 'not exists, aborting...'
    do_cleanup
    exit 1
fi
if [ ! -d $dir2 ]; then
    echo $dir2 'not exists, aborting...'
    do_cleanup
    exit 1
fi

# Back to the previous day, we want to generate rbfsum data
# from the previous day
s=$((`date +%s` - 86400))
start=`$rundir/start-date $s`

# Create directory based on start date
s=`printf $start | cut -d '-' -f-2`
mkdir -p $outdir/$s > /dev/null 2> /dev/null

if [ $? -ne 0 ]; then
    echo 'Could not create output directory, aborting...'
    do_cleanup
    exit 1
fi

# Create xml output directory
mkdir -p $xmldir > /dev/null 2> /dev/null

if [ $? -ne 0 ]; then
    echo 'Could not create xml directory, aborting...'
    do_cleanup
    exit 1
fi

# Okay, lets generate it
printf "Generated at `date +'%Y-%m-%d %H:%M:%S %Z'`\n\n" > $outdir/$s/$start.txt
cd $dir1 && /nmx/bin/rbfsum "*.BHZ" -s $start -h 24 >> $outdir/$s/$start.txt
cd $dir2 && /nmx/bin/rbfsum "*.BHZ" -s $start -h 24 >> $outdir/$s/$start.txt

# Build the xml filename
year=`printf $start | cut -d'-' -f1`
mon=`printf $start | cut -d'-' -f2`
day=`printf $start | cut -d'-' -f3`
xmlfile=$year$mon$day'.xml'

$rundir/rbfsum2xml $outdir/$s/$start.txt > $xmldir/$xmlfile
cp $xmldir/$xmlfile $xmldir/hariini.xml > /dev/null 2> /dev/null

echo $start'.txt'

do_cleanup
exit 0
