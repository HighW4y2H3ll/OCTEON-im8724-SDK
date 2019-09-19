adma is an example initially created to demonstrate how to use the SDK
AXI DMA API (cvmx-adma.[ch]) to transfer data between Octeon main
memory and the Endor PHY under Octeon CNF71XX.

Running as an SE app, it tests the the AXI DMA by verifying data
transfers. As an Linux user-land app, it downloads a file from Linux
running on Octeon to the PHY or vice versa.
