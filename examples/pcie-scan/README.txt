
CN56XX PCIe Scanner

This simple executive application enumerates all host mode PCIe ports
and looks for PCI/PCIe devices. It enables PCI/PCIe bridges and scans
behind them. The purpose of this program is to allow user's to figure
out the PCIe topology using a CN56XX pass 1.

What the program does:
1) Initializes the PCIe host interface
2) Enumerates the PCIe config space until it receives a bus error due
	to the CN56XX pass 1 errata.
3) It saves information about the PCIe bus scan to the internal OCTEON
	key memory.
4) It then does a soft reset with BIST disabled. BIST is disabled so
	the contents of key memory will survive the soft reset.
5) The bootloader starts the application again.
6) State information is read from key memory to determine which PCI
	config space accesses to avoid.
7) Scanning starts at step #1 again.
8) Once the scan is complete, the results and number of reboots needed
	to scan the bus.

In order for step #5 to work, you must configure u-boot to
automatically load the scanner on boot. If you are loading the
scanner from compact flash, issue these commands:

setenv bootcmd 'fatload ide 0 0 pcie-scan; bootoct 0'
setenv bootdelay 1
saveenv
reset

As an example, an EBH5600 board with both PCI slots populated and a
Quad Nitrox card in the PCIe slot takes 193 reboots to scan the bus.
Here is the sample results from one of these runs:

PP0:~CONSOLE-> Checking for init_data
PP0:~CONSOLE-> Found init_data
PP0:~CONSOLE-> On reboot number 193
PP0:~CONSOLE-> PCIe: Waiting for port 0 link
PP0:~CONSOLE-> PCIe: Port 0 link active, 4 lanes
PP0:~CONSOLE-> Probing PCIe port 0
PP0:~CONSOLE-> 0:00:00.0 0x811410b5 Bridge
PP0:~CONSOLE-> 0:01:02.0 0x02421166
PP0:~CONSOLE-> 0:01:02.1 0x02421166
PP0:~CONSOLE-> 0:01:03.0 0x813910ec
PP0:~CONSOLE-> PCIe port 0 had 2 busses
PP0:~CONSOLE-> PCIe: Waiting for port 1 link
PP0:~CONSOLE-> PCIe: Port 1 link active, 4 lanes
PP0:~CONSOLE-> Probing PCIe port 1
PP0:~CONSOLE-> 1:00:00.0 0x851810b5 Bridge
PP0:~CONSOLE-> 1:01:01.0 0x851810b5 Bridge
PP0:~CONSOLE-> 1:02:00.0 0x0010177d
PP0:~CONSOLE-> 1:01:02.0 0x851810b5 Bridge
PP0:~CONSOLE-> 1:03:00.0 0x0010177d
PP0:~CONSOLE-> 1:01:03.0 0x851810b5 Bridge
PP0:~CONSOLE-> 1:04:00.0 0x0010177d
PP0:~CONSOLE-> 1:01:04.0 0x851810b5 Bridge
PP0:~CONSOLE-> 1:05:00.0 0x0010177d
PP0:~CONSOLE-> PCIe port 1 had 6 busses
PP0:~CONSOLE-> Enumeration took 193 reboots



