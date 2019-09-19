#define ADMA_PHY_IMEM	0
#define ADMA_PHY_DMEM	1
#define ADMA_PHY_SMEM	2

#define ADMA_ERROR	1

struct endor_phy_mem {
    int base;
    int size;
};

static const struct endor_phy_mem endor_phy_mem[3][3] = {  /* [tile][mtype] */
    {
    	{CVMX_BBP_PHY_IMEM0_BASE, CVMX_BBP_PHY_IMEM0_SIZE},
    	{CVMX_BBP_PHY_DMEM0_BASE, CVMX_BBP_PHY_DMEM0_SIZE},
    	{CVMX_BBP_PHY_SMEM0_BASE, CVMX_BBP_PHY_SMEM0_SIZE}
    },
    {
    	{CVMX_BBP_PHY_IMEM1_BASE, CVMX_BBP_PHY_IMEM1_SIZE},
    	{CVMX_BBP_PHY_DMEM1_BASE, CVMX_BBP_PHY_DMEM1_SIZE},
    	{CVMX_BBP_PHY_SMEM1_BASE, CVMX_BBP_PHY_SMEM1_SIZE}
    },
    {
    	{CVMX_BBP_PHY_IMEM2_BASE, CVMX_BBP_PHY_IMEM2_SIZE},
    	{CVMX_BBP_PHY_DMEM2_BASE, CVMX_BBP_PHY_DMEM2_SIZE},
    	{CVMX_BBP_PHY_SMEM2_BASE, CVMX_BBP_PHY_SMEM2_SIZE}
    }
};

static int usage(char *progname)
{
    printf("%s: read or write a tile's memory\n", progname);
    printf("Usage:\n\t`%s [-wt] tile file mtype offset nbytes'\n\t\tto read a tile's memory and save it in a file,"
           " or\n\t`%s [-wt] file tile mtype offset nbytes'\n\t\tto write a tile's memory with the content of the file.\n",
        progname, progname);
    printf("where\n");
    printf("\t [[-wt]] selects the word transfer mode\n\t\t (``byte transfer mode'' by default).\n");
    printf("\t  [tile] is 0 for tile RX0, 1 for RX1, and 2 for TX.\n");
    printf("\t  [file] is the name of the file.\n");
    printf("\t\t When reading tile memory, the file is overwritten from\n\t\t the beginning if it existed.\n");
    printf("\t\t When writing tile memory, the file's size should be big\n\t\t enough, i.e., larger than nbytes.\n");
    printf("\t [mtype] can be `smem', `imem', or `dmem'.\n");
    printf("\t[offset] refers to the offset, in bytes, to the tile memory's base.\n");
    printf("\t[nbytes] is the maximum number of bytes to read or write.\n");
    printf("\t\t The actual size of the transaction is\n\t\t min(nbytes, size-of-the-memory - offset)\n");
    printf("\t\t The tool warns when nbytes is bigger but proceeds.\n");
    printf("Exit Status\n");
    printf("\t0 for success.\n");
    printf("\t1 for failure (and a summary is printed out).\n");

    return 0;
}

/* 
 * With tile, t, 1, or 2), and filename, this function loads the the
 * content of the file to the imem of the tile.
 *
 * @return 0 for success and a negative value for failure.
 */
static int task_adma_linux(int argc, char *argv[])
{
    int tile, mtype, offset, nbytes;
    int m, n, fd, flags, ret;
    int is_read_tile, is_word_transfer; /* 1 for yes and 0 for no. */
    char *fname, buffer[1024 * 512];
    uint32_t phy_addr;

    /*
     * Resolve args.
     */
    is_word_transfer = 0;
    if (argc == 7)
    {
	int i;

        /* argv[1] has to be -wt */
	if (strncmp(argv[1], "-wt", 4) != 0)
	{
	    usage(argv[0]);
            return ADMA_ERROR;
	}

	is_word_transfer = 1;

	for (i = 1; i < 6; i++)
	    argv[i] = argv[i + 1];

        argc--;
    }

    if (argc != 6)
    {
	usage(argv[0]);
        return ADMA_ERROR;
    }

    /* tile and file */
#define is_valid_tile(arg)			\
	(strncmp(arg, "0", 2) == 0 ||		\
	 strncmp(arg, "1", 2) == 0 ||		\
	 strncmp(arg, "2", 2) == 0) 
    if (is_valid_tile(argv[1]))
    {
        is_read_tile = 1;
        tile = atoi(argv[1]);
	fname = argv[2];
	flags = O_CREAT | O_WRONLY | O_TRUNC;
    }
    else
    {
        is_read_tile = 0;
	if (is_valid_tile(argv[2]))
            tile = atoi(argv[2]);
	else
	{
	    printf("No valid tile specified.\n");
            return ADMA_ERROR;
	}

        fname = argv[1];
	flags = O_RDONLY;
    }

    /* phy mem type */
    if (strncmp(argv[3], "imem", 5) == 0)
        mtype = ADMA_PHY_IMEM;
    else if (strncmp(argv[3], "dmem", 5) == 0)
        mtype = ADMA_PHY_DMEM;
    else if (strncmp(argv[3], "smem", 5) == 0)
        mtype = ADMA_PHY_SMEM;
    else
    {
        printf("Memory type %s unrecognized.\n", argv[3]);
	return ADMA_ERROR;
    }

    if ((mtype == 0 || mtype == 1) && is_read_tile)
    {
        printf("Reading imem or dmem not allowed.\n");
	return ADMA_ERROR;
    }

    /* offset */
    offset = atoi(argv[4]);
    if (offset < 0 || ((offset == 0) && (strncmp(argv[4], "0", 2) != 0)))
    {
	/* 
	 * atoi(3) does not do error checking and returns 0 for
	 * invalid numbers. So the check.
	 */
	printf("Invalid offset.\n");
	return ADMA_ERROR;
    }

    if (offset >= endor_phy_mem[tile][mtype].size)
    {
	printf("Offset exceeds memory boundary.\n");
	return ADMA_ERROR;
    }

    /* nbytes */
    nbytes = atoi(argv[5]);
    if (nbytes <= 0)
    {
	printf("Invalid number of bytes to read/write.\n");
	return ADMA_ERROR;
    }

    if ((nbytes + offset) > endor_phy_mem[tile][mtype].size)
    {
        nbytes = endor_phy_mem[tile][mtype].size - offset;
	printf("Warning: nbytes truncated to %d\n", nbytes);
    }

    phy_addr = endor_phy_mem[tile][mtype].base + offset;

    /*
     * Reading/Writing tile mem.
     */
    fd = open(fname, flags);
    if (fd == -1) 
    {
        perror("open(2) error\n");
	return ADMA_ERROR;
    }

    if (is_read_tile)
    {
	/* read tile mem to buffer */
        ret = cvmx_adma_io((void *)buffer, phy_addr, nbytes, tile * 2,
            CVMX_ADMA_MAX_BURST16, is_word_transfer, 100);
        if (ret < 0)
        {
            printf("cvmx_adma_io returns %d\n", ret);
	    return ADMA_ERROR;
        }

	/* write buffer to file */
	n = nbytes;
	m = 0;
	while (n > 0)
	{
	    m = write(fd, buffer + nbytes - n, n);
	    if (m < 0)
	    {
	        perror("write(2) error");
	        return ADMA_ERROR;
	    }
	    n -= m;
	}
    }
    else
    {
	/* read file to buffer */
        n = 0;
	while (n < nbytes)
	{
	    m = read(fd, buffer + n, nbytes - n);
	    if (m == 0) /* EOF */
	    {
	        if (n < nbytes)
	        {
	            printf("File size too small.\n");
	    	    return ADMA_ERROR;
	        }
	    }
	    else if (m < 0)
	    {
                perror("read(2) error");
	    	return ADMA_ERROR;
	    }
	    n += m;
	}

	/* write buffer to tile */
        ret = cvmx_adma_io((void *)buffer, phy_addr, nbytes, tile * 2 + 1,
            CVMX_ADMA_MAX_BURST16, is_word_transfer, 100);
        if (ret < 0)
        {
            printf("cvmx_adma_io() returns error %d\n", ret);
	    return ADMA_ERROR;
        }
    }

    close(fd);

    return 0;
}
