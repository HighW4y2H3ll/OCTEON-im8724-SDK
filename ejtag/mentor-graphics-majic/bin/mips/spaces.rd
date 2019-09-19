// Register Definition File (spaces.rd)
// Defines memory and register spaces for the MIPS Architecture

SPACE=MEMORY	0x0	// Normal memory access
SPACE=PHYSICAL  0x09    // Physical memory space
SPACE=MAJIC_DA  0xFE    // Debug Agent (:DA) space
SPACE=MAJIC_INT 0xFF    // MAJIC Internal Space

SPACE=GR	0x100	// r0 - r31 
SPACE=TLB	0x200	// TLB registers  0..? 
SPACE=MR	0x300	// mdhi, mdlo, (acc0-2 54xx)
SPACE=LX	0x400	// Lexra core specific 

// Coprocess access spaces
SPACE=CP0_CTL	0x900   // Some newer MIPS32 chips use this space
SPACE=CP0_GEN	0x800	// Coprocessor control register (cause, sr, etc)
SPACE=CP1_CTL	0xA00	// floating point control
SPACE=CP1_GEN	0xB00	// floating point
SPACE=CP2_CTL	0xC00	// CP2 - typically not used
SPACE=CP2_GEN	0xD00	// "
SPACE=CP3_CTL	0xE00	// Mips I/II architecture chips only
SPACE=CP3_GEN	0xF00	// Mips I/II architecture chips only

// MIPS III and above (including MIPS32/64) architectures
SPACE=ICT	0xE00	// Instruction Cache tags 
SPACE=DCT	0xF00	// Data Cache tags 
