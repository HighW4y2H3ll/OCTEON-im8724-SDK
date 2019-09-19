Readme for named block example

By default, this example uses the cvmx_bootmem_alloc_named_* functions 
to allocate, free, and find named bootmem blocks.  It does a variety 
of operations and checks for expected results.  This example can 
be run as either a standalone simple executive application or 
as a Linux application.

This example can also be used as a tool for diagnose named blocks by specifing 
additional command line parameters:

./named-block -l                     <---- list all named blocks 
./named-block -r "named_block_name"  <--- remove named block by a given name

This example only uses a single core.
