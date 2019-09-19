#!/bin/bash

cd /sys/dev/block
rm -f /dev/mmcblk*
devices=`ls | grep 179`

for i in $devices ;
  do
    major=`echo $i | cut -f1 -d:`
    minor=`echo $i | cut -f2 -d:`
   
    j=`readlink $i | cut -d'/' -f10-`
    j1=`cat $j | cut -d'/' -f2`

    if [ x$j1 != x ] ; then
        mknod /dev/$j1 b $major $minor 
    else
        mknod /dev/$j b $major $minor
    fi
done
    
