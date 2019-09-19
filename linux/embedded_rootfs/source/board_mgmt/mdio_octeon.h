#ifndef __MDIO_OCTEON_H_
#define __MDIO_OCTEON_H_

#include <stdint.h>

/* Or MII_ADDR_C45 into regnum for read/write on mii_bus to enable the 21 bit
   IEEE 802.3ae clause 45 addressing mode used by 10GIGE phy chips. */
#define MII_ADDR_C45                    (1<<30)

int octeon_mdiobus_write(int bus, int phy_id, int regnum, uint16_t val);
int octeon_mdiobus_read(int bus, int phy_id, int regnum);

#endif /* __MDIO_OCTEON_H_ */
