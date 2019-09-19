#!/bin/bash

BGX_ROOT_DIR=/sys/bus/platform/drivers/octeon_bgx_nexus
BGX1_ADDR="11800e1000000"

# Set management interface, if SGMII mode
if [ `ls ${BGX_ROOT_DIR}/${BGX1_ADDR}.*/ | grep $BGX1_ADDR | wc -l` -eq 1 ]; then
    ifconfig eth2 up
fi

# Start fans management service (temperature thresholds in 1/1000C)
TEMP_MAX=70000; TEMP_CRIT=80000; TEMP_HYST=10000; RPM_MAX=16000; RPM_MIN=1000
for mt in /sys/class/hwmon/hwmon*/temp*_max; do
  echo $TEMP_MAX >$mt
done

for mt in /sys/class/hwmon/hwmon*/temp*_crit; do
  echo $TEMP_CRIT >$mt
done

for mt in /sys/class/hwmon/hwmon*/temp1_crit_hyst; do
  echo $TEMP_HYST >$mt
done

# CPU temp sensor thresholds are different
echo 80000 >/sys/class/hwmon/hwmon0/temp2_max
echo 90000 >/sys/class/hwmon/hwmon0/temp2_crit

for fsp in /sys/class/hwmon/hwmon*/fan*_max; do
  echo $RPM_MAX >$fsp
done

for fsp in /sys/class/hwmon/hwmon*/fan*_min; do
  echo $RPM_MIN >$fsp
done

# Enable fan, temp. alert and over temp. events
echo 1 >/sys/niagara/events/overt_sde
echo 1 >/sys/niagara/events/temp_ena
echo 1 >/sys/niagara/events/fan_ena

if [ -f "/etc/fancontrol" ]; then
    rm -f /etc/fancontrol
fi
/bin/fancontrol-config.sh > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "Starting fancontrol ..."
    echo 0 >/sys/niagara/fan/max_speed
    /sbin/airflow -m 80000 -c 90000 >/dev/null 2>&1 &
fi

# Start board management service
SFP_MM_ALLOW=
[ -x /sbin/fw_printenv ] && /sbin/fw_printenv | grep -qi "sfp_mm_allow" && SFP_MM_ALLOW="-s"
/sbin/board_mgmt -m fp,pim $SFP_MM_ALLOW &
# create i2c devices
buses=
for bus in `grep '^pim.*sfp\|^rxaui' /sys/bus/i2c/devices/i2c-*/name`; do
    bnum=$(echo $bus|sed 's#^.*i2c-##; s#/.*$##')
    [ -z "$bnum" ] && continue
    buses="${buses} i2c-$bnum"
    devname=/dev/i2c-$bnum
    [ -c $devname ] || mknod $devname c 89 $bnum
done
[ -n "$buses" ] && echo "Created I2C SFP devices"
