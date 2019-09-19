#! /bin/sh
for d in /proc/irq/* ; do
    if [ -r $d/affinity_hint ] ; then
	hint=`cat $d/affinity_hint`
	val=`echo -n $hint | sed -e 's/,//g' -e 's/0\+/0/g'`
	if [ "$val" != 0 ] ; then
	    echo $hint > $d/smp_affinity
	fi
    fi
done
