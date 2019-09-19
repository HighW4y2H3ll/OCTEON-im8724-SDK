#!/usr/bin/python
#
import sys
import os

#
# Map from interface name to [interface number, max number of ports]
#
o68_iface_atoi = {
    "gmx0" : [0, 4],
    "gmx1" : [1, 1],
    "gmx2" : [2, 4],
    "gmx3" : [3, 4],
    "gmx4" : [4, 4],
    "ilk0" : [5, 8],
    "ilk1" : [6, 8],
    "npi"  : [7, 32],
    "loop" : [8, 8]
}

#
# @param ucfg_x is a string, i.e., quoted name of the config variable
# @return True if the variable ucfg_x exists
#
def helper_has_ucfg(ucfg_x):
    return (ucfg_x in globals())

#
# short hand for printing out of stderr
#
def print_error(err_str):
    print >> sys.stderr, err_str

#
# 1. config file
#
config_file = os.getenv("CVMX_UCFG_FILE")
if config_file == None:
    print_error("$CVMX_UCFG_FILE undefined.")
    quit(1)
if os.path.exists (config_file):
    execfile (config_file)
else:
    print_error(config_file + ": No such a file.")
    quit(1)

#
# 2. model
#
octeon_model = os.getenv("OCTEON_MODEL")
if octeon_model == None:
    print_error("$OCTEON_MODEL undefined.")
    quit(2)
if "OCTEON_CN68XX" in octeon_model:
    iface_atoi = o68_iface_atoi 
else:
    print_error(octeon_model + " not supported")
    quit(2)

#
# 3. pknd/bpid -- leave them to the default
#

#
# 4. pko_nport
#
if helper_has_ucfg('cvmx_ucfg_pko_nport'):
    for iface in cvmx_ucfg_pko_nport:
        if iface not in iface_atoi:
            print_error(iface + "is not available")
	    quit(4)
	iface_num = iface_atoi[iface][0]
	index_max = iface_atoi[iface][1]
	if (len(cvmx_ucfg_pko_nport[iface])) > index_max:
	    print_error("A maximum of " + str(index_max) + 		\
		"physical ports are supported on " + iface + ".")
	    quit(4)
	for index in range(len(cvmx_ucfg_pko_nport[iface])):
	    print "\t    if (" + str(index) + 				\
		" < cvmx_helper_interface_enumerate(" + str(iface_num ) +	\
		"))\n\t\tcvmx_cfg_port[" + str(iface_num) +		\
		"][" + str(index) + "].ccpp_pko_num_ports = " +		\
		str(cvmx_ucfg_pko_nport[iface][index]) + ";"
else:
    cvmx_ucfg_pko_nport = {}

#
# 5. pko_nqueue
#
if helper_has_ucfg('cvmx_ucfg_pko_nqueue'):
    for iface in cvmx_ucfg_pko_nqueue:
        if not iface in iface_atoi:
            print_error(iface + "is not available")
	    quit(5)
	index_max = iface_atoi[iface][1]
	iface_num = iface_atoi[iface][0]
	if not iface in cvmx_ucfg_pko_nport:
	    cvmx_ucfg_pko_nport[iface] =				\
	    [ 1 for x in range(index_max)]
	for index in cvmx_ucfg_pko_nqueue[iface]:
	    if index < 0 or index >= index_max:
	        print_error("index " + str(index) +			\
		    " is out of boundary [0, " + str(index_max) +	\
		    ") for cvmx_ucfg_pko_nqueue[" + iface + "].")
	        quit(5)
	    print ""
	    print "\t    cvmx_cfg_port[" + str(iface_num) + "][" +	\
	        str(index) + "].ccpp_pko_nqueues = cvmx_ucfg_nq;"
	    m = cvmx_ucfg_pko_nport[iface][index]
	    n = len(cvmx_ucfg_pko_nqueue[iface][index])
	    for i in range(min(m, n)):
		print 							\
		    "\t    cvmx_cfg_pko_nqueue_pool[cvmx_ucfg_nq++] = " \
		    + str(cvmx_ucfg_pko_nqueue[iface][index][i]) + ";"
