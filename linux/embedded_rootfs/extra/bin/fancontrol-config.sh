#!/bin/bash
#
# Interface Masters Technologies, Inc. 2016
#

# This script will generate the configuration for fancontrol
# The generated config will be used as /etc/fancontrol

PIDFILE="/var/run/fancontrol.pid"

if [ -f "$PIDFILE" ]
then
	echo "File $PIDFILE exists. This typically means that the"
	echo "fancontrol daemon is running. You should stop it before running fancontrol-config.sh."
	echo "If you are certain that fancontrol is not running, then you can delete"
	echo "$PIDFILE manually."
	exit 1
fi

if [ "`id -u`" != "0" ]
then
	echo "You need to be root to run this script."
	exit 1
fi

DELAY=5 # 3 seconds delay is too short for large fans, thus I increased it to 5
MAX=255

if [ -d "/sys/class/hwmon" ]
then
	DIR="/sys/class/hwmon"
	PREFIX='hwmon*'
fi

cd $DIR
DEVICES=`echo $PREFIX`
if [ "$PREFIX" = "$DEVICES" ]
then
	echo $0: 'No sensors found! (modprobe sensor modules?)'
	exit 1
fi

# We may need to adjust the device path
OLD_DEVICES="$DEVICES"
DEVICES=""
ignore=false

for device in $OLD_DEVICES
do
	ignore=false

	if [ ! -r "$device/name" ]; then
		device="$device/device"
	fi

	if test $ignore = true; then
		continue
	fi

	DEVICES="$DEVICES $device"
done

for device in $DEVICES
do
	# Find available fan control outputs
	MATCH=$device/'pwm[1-4]'
	device_pwm=`echo $MATCH`
	if [ "$MATCH" != "$device_pwm" ]
	then
		PWM="$PWM $device_pwm"
	fi

	# Find available fan monitoring inputs
	MATCH=$device/'fan[1-4]_input'
	device_fan=`echo $MATCH`
	if [ "$MATCH" != "$device_fan" ]
	then
		FAN="$FAN $device_fan"
	fi
done
if [ -z "$PWM" ]
then
	echo $0: 'There are no pwm-capable sensor modules installed'
	exit 1
fi
if [ -z "$FAN" ]
then
	echo $0: 'There are no fan-capable sensor modules installed'
	exit 1
fi

print_devices()
{
	local name device

	for device in $DEVICES
	do
		name=`cat $device/name 2> /dev/null`
		[ -z "$name" ] && name="unknown (no name attribute)"
		echo "$1$device is $name"
	done
}

echo 'Found the following devices:'
print_devices "   "
echo

echo 'Found the following PWM controls:'
for i in $PWM
do
	P=`cat $i`
	echo "   $i           current value: $P"
	if [ -w $i ]
	then
		if [ "$GOODPWM" = "" ]
		then
			GOODPWM=$i
		else
			GOODPWM="$GOODPWM $i"
		fi
	fi
done

if [ "$GOODPWM" = "" ]
then
	echo 'There are no usable PWM outputs.'
	exit 1
fi

echo
echo 'Found the following fan sensors:'
for i in $FAN
do
	S=`cat $i`
	if [ "$S" = "0" -o "$S" = "-1" ]
	then
		echo "   $i     current speed: 0 ... skipping!"
	else
		echo "   $i     current speed: $S RPM"
		if [ "$GOODFAN" = "" ]
		then
			GOODFAN=$i
		else
			GOODFAN="$GOODFAN $i"
		fi
	fi
done
echo

if [ "$GOODFAN" = "" ]
then
	echo 'There are no working fan sensors, all readings are 0.'
	echo 'Make sure you have a 3-wire fan connected.'
	echo 'You may also need to increase the fan divisors.'
	echo 'See doc/fan-divisors for more information.'
	exit 1
fi

for device in $DEVICES
do
	# Find available temperature monitoring inputs
	MATCH=$device/'temp[1-4]_input'
	device_temp=`echo $MATCH`
	if [ "$MATCH" != "$device_temp" ]
	then
		TEMPS="$TEMPS $device_temp"
	fi
done

if [ -z "$TEMPS" ]
then
	echo $0: 'There are no temperature-capable sensor modules installed'
	exit 1
fi

echo
echo 'Found the following temperature sensors:'
for i in $TEMPS
do
	S=`cat $i`
	echo "   $i     current temperature: $S degree C"
done
echo

DevicePath()
{
	if [ -h "$1/device" ]
	then
		readlink -f "$1/device" | sed -e 's/^\/sys\///'
	fi
}

DeviceName()
{
	if [ -r "$1/name" ]
	then
		cat "$1/name" | sed -e 's/[[:space:]=]/_/g'
	elif [ -r "$1/device/name" ]
	then
		cat "$1/device/name" | sed -e 's/[[:space:]=]/_/g'
	fi
}

FCCONFIG="/etc/fancontrol"

# Remember the path and name of each device with at least one
# reference (pwm, temp or fan) in the configuration file.
# This function sets globals DEVPATH and DEVNAME as a side effect.
RememberDevices()
{
	local used entry device name path tempfandev pwmdev
	DEVPATH=""
	DEVNAME=""

	for device in $DEVICES
	do
		device=`echo "$device" | sed -e 's/\/.*$//'`

		used=0
		for entry in $1 $2
		do
			pwmdev=`echo "$entry" | sed -e 's/\/.*$//'`
			tempfandev=`echo "$entry" | sed -e 's/^[^=]*=//' -e 's/\/.*$//'`

			if [ "$device" = "$pwmdev" -o "$device" = "$tempfandev" ]
			then
				used=1
			fi
		done
		if [ "$used" -eq 0 ]
		then
			continue
		fi

		# Record the device path and name. This lets the fancontrol
		# script check that they didn't change. If they did, then the
		# configuration file can no longer be trusted.
		path=`DevicePath "$device"`
		if [ -z "$DEVPATH" ]
		then
			DEVPATH="$device=$path"
		else
			DEVPATH="$DEVPATH $device=$path"
		fi

		name=`DeviceName "$device"`
		if [ -z "$DEVNAME" ]
		then
			DEVNAME="$device=$name"
		else
			DEVNAME="$DEVNAME $device=$name"
		fi
	done
}

SaveConfig()
{
	local tmpfile

	RememberDevices "$FCTEMPS" "$FCFANS"

	echo
	echo "Saving configuration to $FCCONFIG..."
	tmpfile=`mktemp -t pwmcfg.XXXXXXXXXX` || { echo "$0: Cannot create temporary file" >&2; exit 1;  }
	trap " [ -f \"$tmpfile\" ] && /bin/rm -f -- \"$tmpfile\"" 0 1 2 3 13 15
	echo "# Configuration file generated by fancontrol-config.sh, changes will be lost" >$tmpfile
	echo "INTERVAL=$INTERVAL" >>$tmpfile
	echo "DEVPATH=$DEVPATH" >>$tmpfile
	echo "DEVNAME=$DEVNAME" >>$tmpfile
	echo "FCTEMPS=$FCTEMPS" >>$tmpfile
	echo "FCFANS=$FCFANS" >>$tmpfile
	echo "MINTEMP=$MINTEMP" >>$tmpfile
	echo "MAXTEMP=$MAXTEMP" >>$tmpfile
	echo "MINSTART=$MINSTART" >>$tmpfile
	echo "MINSTOP=$MINSTOP" >>$tmpfile
	[ -n "$MINPWM" ] && echo "MINPWM=$MINPWM" >>$tmpfile
	[ -n "$MAXPWM" ] && echo "MAXPWM=$MAXPWM" >>$tmpfile
	mv $tmpfile $FCCONFIG
	chmod +r $FCCONFIG
	#check if file was written correctly
	echo 'Configuration saved'
}

filter_cfgvar()
{
	echo "$1" | sed -e 's/ /\n/g' \
		  | egrep "$2" \
		  | sed -e 's/.*=//g'
}

INTERVAL=10
ADTMINTEMP_1=20
ADTMINTEMP_2=20
ADTMAXTEMP_1=85
ADTMAXTEMP_2=85
ADTMINSTART_1=100
ADTMINSTART_2=100
ADTMINSTOP_1=60
ADTMINSTOP_2=60
MINTEMP=""
MAXTEMP=""
MINSTART=""
MINSTOP=""
tmpfile="/tmp/tmp.config"
tmp="="
temp1="/temp1_input"
FCTEMPS=""

for device in $DEVICES
do
	name=`cat $device/name 2> /dev/null`
	if [ $name == "acpitz" ]
	then
		acptiz_hwmon=$device
	fi
	if [ $name == "max6699" ]
	then
		max6699_hwmon=$device
	fi
done
FCFANS=""
tmp="="
utemp="/temp2_input"
FCTEMPS=""

i=1
for pwms in $PWM
do
	pwmsed=`echo ${pwms} | sed -e 's/\//\\\\\//g'`
	FAN=`echo $GOODFAN|cut -d' ' -f$i`
	fc_fans="`echo $FCFAN | sed -e "s/${pwmsed}[^ ]* *//g"` ${pwms}=$FAN"
	if [ -z "$FCFANS" ]
	then
		FCFANS="$fc_fans"
	else
		FCFANS="$FCFANS $fc_fans"
	fi

	fan_pwm=`echo $PWM|cut -d' ' -f$i`
	min_temp=$fan_pwm$tmp$ADTMINTEMP_2
	if [ -z "$MINTEMP" ]; then
		MINTEMP="$min_temp"
	else
		MINTEMP="$MINTEMP $min_temp"
	fi

	max_temp=$fan_pwm$tmp$ADTMAXTEMP_2
	if [ -z "$MAXTEMP" ]; then
		MAXTEMP="$max_temp"
	else
		MAXTEMP="$MAXTEMP $max_temp"
	fi

	min_start=$fan_pwm$tmp$ADTMINSTART_2
	if [ -z "$MINSTART" ]; then
		MINSTART="$min_start"
	else
		MINSTART="$MINSTART $min_start"
	fi

	min_stop=$fan_pwm$tmp$ADTMINSTOP_2
	if [ -z "$MINSTOP" ]; then
		MINSTOP="$min_stop"
	else
		MINSTOP="$MINSTOP $min_stop"
	fi

	fc_temp=`echo $PWM|cut -d' ' -f$i`
	fc_temp_1=$fc_temp$tmp$max6699_hwmon$utemp
	if [ -z "$FCTEMPS" ]; then
		FCTEMPS="$fc_temp_1"
	else
		FCTEMPS="$FCTEMPS $fc_temp_1"
			fi
	i=$((i+1))
done
SaveConfig
