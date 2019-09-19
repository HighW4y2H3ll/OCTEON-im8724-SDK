#!/bin/bash
# Script to help automate adding a new board
# extract the list of board names from the board.cfg file
board_list=`fgrep -v '#' boards.cfg | awk '{print $4}' | uniq`
donor_board=""
echo "This script will create a new board.  It will overwrite all of"
echo "the configuration files for the new board if they exists but it"
echo "will by no means completely replace everything.  Press ^C"
echo "if you do not wish to continue."
echo
echo "Please enter the number for a donor board to use as the base for the new board."
echo "The donor board is often the evaulation board;  see the SDK documentation on"
echo "porting a new board to U-Boot for more information on donor board selection."
select donor_board in $board_list ; do
	if [ ! -z "$donor_board" ] ; then
		echo "$donor_board selected"
		break;
	fi
done

new_board=""

while [ -z ${new_board} ] ; do
	echo
	echo "Enter the name for the new board (without the \"octeon_\")"
	echo "It should be lower case and all one word, for example, foo6800"
	echo -n "Enter new board name: "
	read new_board
done

echo
echo "The new board name will be octeon_${new_board}"

grep "octeon_$new_board" boards.cfg >/dev/null
if [ "$?" = "0" ] ; then
	echo "$new_board already exists in boards.cfg."
fi
echo
echo "Do you want to continue? [1=yes, 2=no]"
select answer in "yes"  "no" ; do
	case "$answer" in
	"yes"	) break ;;
	"no"	) echo "Exiting at you request."; exit 1;;
	esac
done

echo
echo "Does the $new_board use an Octeon CN70XX/CN71XX? [1=yes, 2=no]"
select octeon_cn70xx in "yes" "no" ; do
	case "$octeon_cn70xx" in
	"yes"	) octeon_cn70xx=",OCTEON_CN70XX"; break;;
	"no"	) octeon_cn70xx=""; break;;
	esac
done
echo "Is this a stage 2 bootloader? [yes/no]"
isstage2=""
if [ -n "$octeon_cn70xx" ] ; then
	select isstage2 in "yes" "no" ; do
		case "$isstage2" in
		"yes"	) isstage2=",OCTEON_HEADER_ARGS=\"--stage2\""; break;;
		"no"	) isstage2=""; break;;
		esac
	done
fi
echo
echo "Does the $new_board support PCI/PCIe host mode? [1=yes, 2=no]"
select pci_host in "yes"  "no" ; do
	case "$pci_host" in
	"yes"	) pci_host="1"; break ;;
	"no"	) pci_host="0"; break ;;
	esac
done

echo "Setting up the files and directories for ${new_board},"
echo "using donor board ${donor_board}:"
echo "	Copying configuration file(s)..."

cp -f "include/configs/octeon_${donor_board}.h" "include/configs/octeon_${new_board}.h"

shared_board=`grep '^#include \"octeon_.*_shared.h' "include/configs/octeon_${donor_board}.h" | awk '{print $2}' | sed s/\"//g`

#Translate new board name into all upper case to creat the defines
new_board_upper=`echo "${new_board}" | tr a-z A-Z`
donor_board_upper=`echo "octeon_"$donor_board | tr a-z A-Z`
if [ ! -z "$shared_board" ] ; then
	echo "	Found memory configuration file $shared_board"
	echo "	Copying $shared_board to octeon_${new_board}_shared.h..."
	cp -f "include/configs/${shared_board}" "include/configs/octeon_${new_board}_shared.h"
	# Changing the shared include file name in octeon_${new_board}.h
	echo "	Changing the shared include file name in octeon_${new_board}.h..."
	sed -i "s/octeon_${donor_board}_shared.h/octeon_${new_board}.h/" "include/configs/octeon_${new_board}_shared.h"
	sed -i "s/__${donor_board_upper}_SHARED_H__/__OCTEON_${new_board_upper}_SHARED_H__/g" "include/configs/octeon_${new_board}_shared.h"
fi

echo "	Creating directory board/octeon/$new_board..."
mkdir -p "board/octeon/"$new_board
echo "  Copying files and creating symbolic links..."
cp "board/octeon/${donor_board}/${donor_board}_board.c" "board/octeon/${new_board}/${new_board}_board.c"
cp "board/octeon/${donor_board}/${donor_board}.dts" "board/octeon/${new_board}/${new_board}.dts"
cd "board/octeon/${new_board}"
echo "	Creating links to Makefile and make components in common_make..."
ln -sf ../common_make/Makefile .
ln -sf ../common_make/config.mk .
cd ../../..

# Add board to boards.cfg
echo "	Adding $new_board to boards.cfg..."
grep "octeon_$new_board" boards.cfg >/dev/null
if [ "$?" = "0" ] ; then
	echo "	(The board $new_board already exists in boards.cfg.  Replacing existing entry...)"
	sed -i '/^octeon_'$new_board'.*/d' boards.cfg
fi
echo "octeon_${new_board}        mips        octeon      ${new_board}       octeon         -      octeon_${new_board}:OCTEON_${new_board_upper},OCTEON_PCI_HOST=${pci_host}${octeon_cn70xx}${isstage2}" >> boards.cfg
rm -f .boards.depend
echo
echo "Done creating files and directories for board ${target_board}"
echo
echo "Please take the following steps:"
echo "1) Edit cvmx-app-init.h in the executive directory to add the new board ${new_board}:"
echo "	1a) Add CVMX_BOARD_TYPE_CUST_${new_board_upper} to cvmx_board_type_enum list"
echo "	1b) Add CVMX_BOARD_TYPE_CUST_PRIVATE_${new_board_upper} to the function"
echo "		cvmx_board_type_to_string()"
echo
echo "2).  Edit arch/mips/cpu/octeon/lib_octeon_shared.c:"
echo "	2a) Add the include configs/octeon_${new_board}_shared.h near include for"
echo "		octeon_${donor_board}_shared.h."
echo "	2b) Add compiled-in spd array entry to for the new board ${new_board},"
echo "		duplicating the sections for the ${donor_board} board."
echo "	2c) Add entry to octeon_board_ddr_config_table array for the new board"
echo "		 ${new_board}, duplicating the sections for the ${donor_board} board."
echo "	2d) Search for CONFIG_OCTEON_"$donor_board_upper" to find any other the areas in"
echo "		lib_octeon_shared.c which need to be edited."
echo
echo "To build the boot loader, type: make octeon_${new_board}_config"
echo "This will create the file config.h which tells the Makefile which board to build."
echo "Then type: make"
echo
echo "If the donor board is an available evaluation board, then after the build is"
echo "complete, install u-boot-octeon_${new_board}.bin onto the donor board to "
echo "verify this process completed without error."
echo
echo "The next step is to start editing include/configs/octeon_${new_board}.h,"
echo "include/configs/octeon_${new_board}_shared.h,"
echo "board/octeon/${new_board}/${new_board}.dts and"
echo "board/octeon/${new_board}/${new_board}_board.c to customize them for the new board."
echo "See the U-Boot porting guide supplied with the SDK documentation for help."
echo
echo
echo "Have fun!"

exit 0
