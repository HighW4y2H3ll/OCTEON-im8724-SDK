OCTEON Simple Executive based Traffic Generator

1. Introduction

   This example implements a traffic generator using the OCTEON simple executive
   programming environment. It displays all the statistics available about
   packets sent and received. It is capable of sustaining 10 Gbps line rate for
   all packet sizes, and 14 Gbps for larger packet sizes.


2. Overview

   Logically, traffic-gen has two parts, the control plane and the data path.
   The control plane runs on the first core (the control core) among the group
   of cores it is booted up with. The actual traffic-generation is performed 
   by the rest of the cores (data cores).

   From SDK-3.0, the data cores are not bound to the ports anymore. Instead, the
   control core maintains a pool of data cores and dynamically assign them to
   traffic-generating tasks upon user commands. As a result, although the total
   number of cores necessary to send out all the ports on a board stays the
   same, much fewer cores are required to start traffic-gen and send out the
   individaul ports.


3. Running the Traffic Generator
3.1 Boot up
   The following commands will start it off of a compact flash:

	$ fatload ide 0 $(loadaddr) traffic-gen
	$ bootoct $(loadaddr) coremask=ffff

   The traffic generator takes a few seconds to probe and configure all of the
   OCTEON ethernet hardware, then presents a simple text based interface out the
   serial port.

   WARNING:
	The traffic generator enables hardware flow control. Make sure your
	terminal program is setup properly. If not, traffic-gen may hang on
	startup.


3.2 The Ports

   After startup, the traffic generator will show a column of statistics for
   each ethernet port that is connected to the board. The ethernet port numbers
   are different for each ethernet port. For example, EBT5800 can have 10
   SPI4000 plus 4 RGMII and EBB6800 can have 3 SGMII plus 1 ILK (8 channel)
   ethernert port combinations.  The horizontal screen width is about 174
   columns. If you aren't using some of the ports, you can hide them using the
   "hide" command. 

   Please use the following table for referring to the ports in EBB6800.

        INTERFACE   QLM   PORT NUMBER
       ------------------------------------------------------------------
         SGMII       0     2048,2064,2080,2096
         XAUI        0     2112
         RXAUI       1     2368
         SGMII       2     2560,2576,2592,2608
         XAUI        2     2624
         SGMII       3     2816,2832,2848,2864
         XAUI        3     2880
         SGMII       4     3072,3088,3104,3120
         XAUI        4     3136
         ILK         1     1024,1025,1026,1027,1028,1029,1030,1031
         ILK         2     1280,1281,1282,1283,1284,1285,1286,1287
       ------------------------------------------------------------------

   Please use the following table for referring to the ports in EBB7800.

        INTERFACE   QLM   PORT NUMBER
       ------------------------------------------------------------------
         SGMII/XFI/10G_KR          0     2048,2064,2080,2096
         XAUI/XLAUI/40G_KR4        0     2048
	 RXAUI                     0     2048,2064
         SGMII/XFI/10G_KR          1     2304,2320,2336,2352
         XAUI/XLAUI/40G_KR4        1     2304
	 RXAUI                     1     2304,2320
         SGMII/XFI/10G_KR          2     2048,2064,2080,2096
         XAUI/XLAUI/40G_KR4        2     2048
	 RXAUI                     2     2048,2064
         SGMII/XFI/10G_KR          3     2304,2320,2336,2352
         XAUI/XLAUI/40G_KR4        3     2304
	 RXAUI                     3     2304,2320
         SGMII/XFI/10G_KR          4     2560,2576,2592,2608
         XAUI/XLAUI/40G_KR4        4     2560
	 RXAUI                     4     2560,2576
         SGMII/XFI/10G_KR          5     2816,2832,2848,2864
         XAUI/XLAUI/40G_KR4        5     2816
	 RXAUI                     5     2816,2832
         SGMII/XFI/10G_KR          6     3072,3088,3104,3120
         XAUI/XLAUI/40G_KR4        6     3072
	 RXAUI                     6     3072,3088
         SGMII/XFI/10G_KR          7     3328,3344,3360,3376
         XAUI/XLAUI/40G_KR4        7     3328
	 RXAUI                     7     2304,2320
         ILK                       4     1024,1025,1026,1027,1028,1029,1030,1031
         ILK                       5     1280,1281,1282,1283,1284,1285,1286,1287
       ------------------------------------------------------------------

   On EBT5800, the SPI4000 ports are 0 - 9 and RGMII ports are 16 - 19.
   Throughout this README, it is assumed you are not using the SPI4000 and don't
   care about port 0 through 9 in EBT5800. 

   The following command hide the first 10 ports, leaving only the 4 RGMII
   visible:

	Command> hide 0 9
	
   Note: Currently this example does almost no input validation, so many input
	mistakes may cause it to crash. Verify commands before you enter them.
	
   After hiding the SPI4000 columns, you should have a screen similar to the one
   below:
   
          0:00:00         32 |  *Port 16|  *Port 17|  *Port 18|  *Port 19|  Totals
       22:RX packets         |         1|         0|         0|         0|         1
       23:RX octets          |        64|         0|         0|         0|        64
       25:RX Mbps            |         0|         0|         0|         0|         0
       26:tx.size            |        60|        60|        60|        60|
       27:tx.type            |  IPv4+UDP|  IPv4+UDP|  IPv4+UDP|  IPv4+UDP|
       28:tx.payload         |       abc|       abc|       abc|       abc|
       29:TX packets         |         0|         0|         0|         0|         0
       30:TX octets          |         0|         0|         0|         0|         0
       32:TX Mbps            |         0|         0|         0|         0|         0
       34:Total RX packets   |        37|         0|         0|         0|        37
       37:Total TX packets   |         0|         0|         0|         0|         0
       46:dest.mac           | fb71009bf| fb71009c0| fb71009c1| fb71009c2|
       50:src.mac            | fb71009be| fb71009bf| fb71009c0| fb71009c1|
       54:dest.ip            |10.17.0.99|10.18.0.99|10.19.0.99|10.20.0.99|
       58:src.ip             |10.16.0.99|10.17.0.99|10.18.0.99|10.19.0.99|
       72:bridge             |       off|       off|       off|       off|
       76:validate packets   |       off|       off|       off|       off|

   Statistics update once per second. All statistics except the bottom totals
   are rates per second. The totals are cumulative since startup or the last
   "clear" command. Currently, the OCTEON clock speed calculation is a little
   off. This causes the rates displayed and sent to be slightly off the physical
   rates. The variation is much less than 1%.


3.3 The Commands

   Many commands have optional parameters and work on a port range, e.g.,
   the command to clear the statistics of a range of ports,

      clear [<port range>]

   [<port range>] means [<first port> [<last port>]] so this is the same as
  
      clear [<first port> [<last port>]]

   which means

      clear                           Clear the port range set by the "default" command.
      clear <port>                    Clear port <port>'s statistics.
      clear <first port> <last port>  Clear the statistics of ports <first port> <last port>.

   Note that [<x> [<y>]] means you can have:  nothing, <x>, or <x> <y> (but not
   <y> without <x>). For example,
      
      clear 17                        Clear port 17's statistics.
      clear 16 19                     Clear the statistics of ports 16 to 19.

   Some commands have a port range and another optional parameter, e.g.,

      tx.size [[<port range>] <size>]           Size of the packet transmitted, excluding the frame CRC

   Note that <size> is "less optional" than <port range>.  That is, if there is
   only one parameter, it is <size> and if there are only two one is <port> and
   the other is <size>. Again, this is the same as

      tx.size [[<first port> [<last port>]] <size>]

   which means

      tx.size                                  Display the sizes of all ports in the current default port range.
      tx.size <size>                           Set the size of all ports of the current default port range to <size>.
      tx.size <port> <size>                    Set the size of port <port> to <size>.
      tx.size <first port> <last port> <size>  Set the size of ports <first port>..<last port> to <size>.
      	Set the size of the packet (per port) to transmit, excluding the
      	ethernet frame CRC. Valid range is 60 to 1514.

   It is also common for commands with an optional parameter to display their
   current settings if that parameter is missing. In this case you can not
   display the current value of a range of ports (including a single port)
   unless single port unless your default port range is set to that.

   For example, "tx.size 17" will set all default port sizes to 17, not display
   port 17's current value.  To display only port 17's current value the two
   commands "default 17" and "tx.size" could be used, although if your default
   range is 16..19, it is easier to just use "tx.size" and ignore 16, 18, and
   19's value.  Also note that the size for each port is printed in one of the
   rows for that port.

   In addition, some are "toggle or set" style commands, e.g.,

      row [<first row> [<last row>]] [on|off]    Toggle or set the row range on or off

   This means:
   
      row                             Toggle all rows (hide the rows you did see, show the rows that were hidden)
      row on                          Turn all rows on (display all rows)
      row off                         Turn all rows off (hide all rows)
      row <row>                       Toggle row <row> (hide row <row> if it was displayed, display it if not)
      row <row> on                    Turn row <row> on (display row <row>)
      row <row> off                   Turn row <row> off (hide row <row>)
      row <first row> <last row>      Toggle rows <first row>..<last row>.  Note that this can get confusing.
      row <first row> <last row> on   Turn rows <first row>..<last row> on (display them)
      row <first row> <last row> off  Turn rows <first row>..<last row> off (hide them)

   "hli" (highlight) is very similar, and "freeze" is the same but with out the range.

   Note that "hli", "on", and "off" are literal strings, but "<row>", "<first
   row>", etc. are variables that have a numeric value.

   Some commands, like "tx.payload [<port range. [<payload type> | help]]" take
   non-numeric values for thier parameters <payload type> in this example. TAB
   completion, "help", or CTRL-D, or simply not supplying the parameter can show
   a list of acceptable values for this type of command.

   Finally, one command is special in that the first set of brackets are literal
   rather than being used to show that parameters are optional:

      "tx.data[<index>] [[<port range>] <data>]"

   Valid forms of this command are:

      tx.data[<index>]                 Display packet data bytes (<index>*8)..((<index>*8)+7) as a 64-bit number for the default ports*
      tx.data[<index>] <data>          Set packet data bytes (<index>*8)..((<index>*8)+7) as a 64-bit number for the default ports
      tx.data[<index>] <port> <data>   Set packet data bytes (<index>*8)..((<index>*8)+7) as 64-bit number for port <port>
      tx.data[<index>] <first port> <last port> <data>    Ditto for ports <fisrt port>..<last port>
      * Note that in actuality it only displays the values if the ports are not hidden.
      Also note that spaces are not allowed in [<index>].

   For example,
      tx.data[10] 16 0                 Sets bytes 80..87 of port 16's data to 0.

   Here is a list of available Commands: (case insensitive)

      default [<first port> [<last port>]]      Set or display the default port range
      start [<port range>] | all                Start transmitting on these ports
      stop [<port range>] | all                 Stop transmitting on these ports
      show [<port range>] | all                 Display the statistics for these ports (i.e. show columns)
      hide [<port range>] | all                 Hide the statisitcs for these ports (i.e. hide columns)
      clear [<port range>] | all                Clear statistics counters for these ports
      clearall                                  Clear statistics counters for all ports
      reset [<port range>] | all                Reset ports to traffic-gen startup defaults
      tx.size [[<port range>] <size>]           Size of the packet transmitted, excluding the frame CRC (60..65524)
      tx.percent [[<port range>] <%>]           Set transmit rate as a % of gigabit
      tx.rate [[<port range] <packets/s>]       Set transmit rate in packets per second
      tx.count [[<port range>] <number>]        Automatically stop after this many packets
      tx.data[<index>] [[<port range>] <data>]  Set or read the data in the packet in 64bit words, index=0..size/8
      tx.payload [<port range> [<type> | help]] Set the data type for the payload (help to see choices)
      tx.type [<port range> [<type> | help]]    Set the type for the packet (help to see choices)
      tx.checksum [<port range>] [on|off|show]  Turn on/off UDP payload checksum
      csr address [<new value>]                 Read or write a CSR by exact address
      row [<first row> [<last row>]] [on|off|min]  Toggle or set the row range on or off or to min
      hli [<first row> [<last row>]] [on|off]   Toggle or set highlighting of a row range on or off
      freeze [on|off]                           Toggle or set the display updates on or off
      history [<first line> [<last line>]]      Show a command history range
      debug [<value>]                           Set or show the debug value
      cls                                       Clear the screen
      reboot                                    Reboot the OCTEON
      mii.read <dev> <reg>                      Read from the MDIO bus, device <dev>, register <reg> (bit 7 in dev specifies MDIO bus)
      mii.write <dev> <mmd> <reg> <value>       Write to the MDIO bus, device <dev>, register <reg> (bit 7 in dev specifies MDIO bus)
      mii45.read <dev> <mmd> <reg>              Read from the MDIO bus (clause 45), device <dev>, mmd <mmd>, register <reg> (bit 7 in dev specifies MDIO bus)
      mii45.write <dev> <reg> <value>           Write to the MDIO bus (clause 45), device <dev>, mmd <mmd>, register <reg> (bit 7 in dev specifies MDIO bus)
      phy.speed [[<port range>] [auto] [full/half] [10/100/1000]]    Program the PHY negotiation speed
      src.mac [[<port range>] <address>]        Set or show the source MAC address. Set using xx:xx:xx:xx:xx:xx.
      src.mac.inc [[<port range>] <increment>]  Set or show the source MAC address increment
      src.mac.min [[<port range>] <address>]    Set or show the source MAC minimum. Once the address reaches this value it will wrap to max
      src.mac.max [[<port range>] <address>]    Set or show the source MAC maximum. Once the address reaches this value it will wrap to min
      dest.mac [[<port range>] <address>]       Set or show the destination MAC address. Set using xx:xx:xx:xx:xx:xx.
      dest.mac.inc [[<port range>] <increment>]  Set or show the destination MAC address increment
      dest.mac.min [[<port range>] <address>]    Set or show the destination MAC minimum. Once the address reaches this value it will wrap to max
      dest.mac.max [[<port range>] <address>]    Set or show the destination MAC maximum. Once the address reaches this value it will wrap to min
      tx.vlan [[<port range>] <vlan bytes>|off] Set or show the bytes to be inserted into the L2 header--eg. use '81000ffd' for VLAN 4093.
      src.ip [[<port range>] <address>]         Set or show the source IP address. Set using xxx.xxx.xxx.xxx.
      src.ip.inc [[<port range>] <increment>]   Set or show the source IP address increment
      src.ip.min [[<port range>] <L4 ip>]       Set or show the source IP address minimum. Once the IP reaches this value it will wrap to max
      src.ip.max [[<port range>] <L4 ip>]       Set or show the source IP address maximum. Once the IP reaches this value it will wrap to min
      dest.ip [[<port range>] <address>]        Set or show the destination IP address. Set using xxx.xxx.xxx.xxx.
      dest.ip.inc [[<port range>] <increment>]  Set or show the destination IP address increment
      dest.ip.min [[<port range>] <L4 ip>]      Set or show the destination IP address minimum. Once the IP reaches this value it will wrap to max
      dest.ip.max [[<port range>] <L4 ip>]      Set or show the destination IP address maximum. Once the IP reaches this value it will wrap to min
      ip.tos [[<port range>] <value>]           Set or show the IP TOS field.
      src.port [[<port range>] <L4 port>]       Set or show the source UDP(L4) port
      src.port.inc [[<port range>] <increment>] Set or show the source UDP(L4) port increment
      src.port.min [[<port range>] <L4 port>]   Set or show the source UDP(L4) port minimum. Once the port reaches this value it will wrap to max
      src.port.max [[<port range>] <L4 port>]   Set or show the source UDP(L4) port maximum. Once the port reaches this value it will wrap to min
      dest.port [[<port range>] <L4 port>]      Set or show the destination UDP(L4) port
      dest.port.inc [[<port range>] <inc>]      Set or show the destination UDP(L4) port increment
      dest.port.min [[<port range>] <L4 port>]  Set or show the destination UDP(L4) port minimum. Once the port reaches this value it will wrap to max
      dest.port.max [[<port range>] <L4 port>]  Set or show the destination UDP(L4) port maximum. Once the port reaches this value it will wrap to min
      arp.request <port> <ip address>           Send an ARP request to <ip address> on <port>
      tx.arp.reply [<port range>] [on|off|show] Controls sending of ARP replies when receiving ARP reqests with matching IP addr
      rx.arp.request [<port range>] [on|off|show] Controls setting of dest MAC address when receiving ARP requests
      rx.arp.reply [<port range>] [on|off|show] Controls setting of dest MAC address when receiving ARP replies
      find.max [<tx port> <rx port>|default]    Perform a binary search to find the max TX rate where all packets are received
      scan.sizes [<tx port> <rx port>|default]  Send 1000 packets at each size from 60 to 65524 bytes, stopping if any bytes are lost
      bridge [[<port range>] <port>|off]        Bridge incomming packets for a port out another port
      rx.packets [<port range>] | all           Show RX packets for a range of ports
      rx.octets [<port range>] | all            Show RX octets for a range of ports
      rx.mbps [<port range>] | all              Show RX Mbps for a range of ports
      rx.total_packets [<port range>] | all     Show total RX packets for a range of ports
      rx.total_octets [<port range>] | all      Show total RX octets for a range of ports
      rx.total_errors [<port range>] | all      Show total RX errors for a range of ports
      rx.display [<port range>] [on|off|show]   Turn on/off display of received packets
      tx.packets [<port range>] | all           Show TX packets for a range of ports
      tx.octets [<port range>] | all            Show TX octets for a range of ports
      tx.mbps [<port range>] | all              Show TX Mbps for a range of ports
      tx.total_packets [<port range>] | all     Show total TX packets for a range of ports
      tx.total_octets [<port range>] | all      Show total TX octets for a range of ports
      higig [[<port range>] <header size>]      Size of HiGig header to prepend to the packets. Must be 0, 12, or 16
      validate [<port range>] [on|off|show]     Turn on/off of packet validation using CRC32C.
      editing                                   Show help for editing
      alias [<prefix>] | <[name> <command>      Show or set an alias for a name or function key (F1..F12, Shift-F1..Shift-F8)
      unalias <name>                            Remove alias <name>.
      <Pause/Break key>                         Toggle transmit on/off for default ports (if only using default ports).
      <Function Keys>/<Shift-Function Keys>     Can be mapped to various functions (see alias).
      echo <message>                            Print <message>.
      type <text>                               Assume <text> was typed, useful for function key mapping.
      help [<prefix>] [<lines>]                 Show help for commands starting with <prefix> in pages of <lines>


4. Comand Line Editing

   Many vi style modes are available for command and history navigation.
   Use ARROW KEYS, HOME, END or ESC-h,ESC-j,ESC-k,ESC-l,ESC-0,ESC-$,ESC-w,ESC-b, or ESC-fx for navigation.
   Use TAB for commmand completion, multiple to cycle, or CTRL-D for a list of possible commands.
   Use ESC-i,ESC-A,ESC-a,ESC-c for insert mode, ESC-R,ESC-r for overwrite mode (behavior like vi).
   Use INSERT to toggle insert/overwrite mode, DELETE to delete, F1 for help.
   Use !!, !<num> or !<string> to repeat previous commands in the history.
   Use ::, :<num> or :<string> to recall previous commands in the history to current.
   Use ?<string>, /<string>, ESC-?<string>,ESC-/<string>,ESC-n,ESC-N to also recall to current.
   Use CTRL-l erase the screen, ESC-D to clear the rest of the current line.
   Use ESC-d<movement> to delete, ESC-~ to change case, ESC-u to undo (one level).
   See the vi man page for more info on the operation of various commands.
   
   Note: If you don't see any statistics, you may have hidden or frozen them.
   
   Note: This program works best with a very wide and tall terminal window.


5. Scripting

   The traffic-gen can be scripted in one of two different ways. The first, and
   most common method, is through the serial port. Traffic-gen uses hardware
   flow control to allow sending of scripts over the serial port. You can use
   the builtin scripting of Minicom, or send commands directly out of the serial
   device. For example, a file of commands can be sent to the traffic-gen using
   the following command:

	$ cat <script_file> > /dev/ttyS0
	
   It is not necessary to close Minicom. In addition to the serial port,
   Traffic-gen is capable of receiving commands through PCI. The normal
   bootloader command utility oct-pci-bootcmd is supported by the Traffic-gen. A
   command can be sent as follows:

	$ oct-pci-bootcmd "tx.size 512"
	

   NOTE: This example requires at least two cores and therefore is not supported
   on some of the CN30XX, CN50xx chips.


6. Warning and Error Messages
6.1 SCHED_TASK_TX_ONE
   
   This message happens when a core that's sending is scheduled again to send,
   e.g.,

	SCHED_TASK_TX_ONE: 2112 is already enabled

   It can be caused by either user starting to send out a port that a (data)
   core is already working on, e.g., by issuing

   	start 2112

   twice or it might indicate a real bug.

   
7. Build

   For 'normal' build just run 'make' in ../sdk/examples/traffic-gen.
   
   For cleaning use 'make clean'.
   
   In order to include new 'dump' submenu build with 
   'CVMX_CMD_DUMP=1 make clean all' in ../sdk/examples/traffic-gen. Note that 
   this will also rebuild libcvmx.a to include the new 'dump' submenu functions.
   This way the menu is extended with commands to show BGX/QLM/PKI/PKO/SSO
   config/status. Start with the command 'dump help' to see choices.
