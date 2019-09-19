#!/bin/bash

Usage() {
    echo ""                                                      1>&2
    echo "Usage: $0 [-p port] [-m program] name num_cores"       1>&2
    echo "    name = hostname or IP address of Majic probe"      1>&2
    echo "    num_cores = Number of Octeon cores"                1>&2
    echo "    port = TCP/IP port used for core 0"                1>&2
    echo "    program = overrides default mdi-server executable" 1>&2
    echo ""                                                      1>&2
    exit 2
}

base_port=8888
mdiserver_program=mdi-server.static

#
# Parse command options.
#
while :
do
    case $1 in
        -p)  base_port=$2
             shift 2
             ;;
        -p*) base_port=`echo "$1" | sed 's/^..//'`
             shift
             ;;
        -m)  mdiserver_program=$2
             shift 2
             ;;
        -m*) mdiserver_program=`echo "$1" | sed 's/^..//'`
             shift
             ;;
        --)  shift
             break
             ;;
        -*)  Usage
             exit 2
             ;;
        *)   break
             ;;
    esac
done

# Make sure the number of parameters is correct.
#
if [ $# -ne 2 ]; then
     Usage
     exit 2
fi
#
majic=$1
num_cores=$2


# This is the top level path for the SDK Majic stuff
CAVIUM_MAJIC_PATH=${OCTEON_ROOT}/ejtag/mentor-graphics-majic

# Point to EDTM binaries in Cavium distribution
MAJIC_PATH=${CAVIUM_MAJIC_PATH}
# If EDTM is properly installed this should point to the EDTM distribution instead
mdi_server_path=`which ${mdiserver_program} 2>&1` && MAJIC_PATH=`echo "${mdi_server_path}"|sed "s/\(.*\)\/bin\/${mdiserver_program}/\1/g"`

if [ ! -x ${MAJIC_PATH}/bin/${mdiserver_program} ]; then
    echo "Couldn't execute ${MAJIC_PATH}/bin/${mdiserver_program}.  Exiting..."
    exit 2
fi

# Change to the temporary directory and start the servers
core=0
rm -rf .mdicfg
while [ ${core} -lt $2 ]
do
    port=`expr ${base_port} + ${core}`
    echo "Starting ${mdiserver_program} for core ${core} on port ${port}"
    mkdir -p .mdicfg/mdi-${core}
    cp -a ${CAVIUM_MAJIC_PATH}/cavium_minimal/* .mdicfg/mdi-${core}/
    pushd .mdicfg/mdi-${core} > /dev/null

    #Create a config file with the correct host name
    cp epimdi-template.cfg epimdi.cfg
    sed -i "s|MAJIC_PATH|${MAJIC_PATH}|g" epimdi.cfg
    sed -i "s/MAJIC_HOST/${majic}:e/g"    epimdi.cfg

    cp cavium_minimal-template.cmd cavium_minimal.cmd
    sed -i "s/MAJIC_CORE/${core}/g" cavium_minimal.cmd
    ${MAJIC_PATH}/bin/${mdiserver_program} -l ${MAJIC_PATH}/bin/mdi.so -r 8 -n 38 -p ${port} &> ../mdi-server-${core}.log &
    popd > /dev/null
    core=`expr ${core} + 1`
done

echo "Hit Control-C to kill MDI servers"
wait

