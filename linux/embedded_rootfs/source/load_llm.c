#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysmips.h>
#include <errno.h>
#include "cvmx.h"
#include "cvmx-sysinfo.h"
#include "cvmx-llm.h"
#include "cvmx-atomic.h"

#define MAX_ERRORS 20

#ifdef LLM64
    #define LLM_WRITE cvmx_llm_write_wide
    #define LLM_READ  cvmx_llm_read_wide
    #define LLM_STEP  8
#else
static uint64_t read_llm_narrow_raw(cvmx_llm_address_t addr, int sep)
{
    cvmx_llm_data_t val;
    val = cvmx_llm_read_narrow(addr, sep);
    return(val.s.dat);
}
    #define LLM_WRITE cvmx_llm_write_narrow
    #define LLM_READ  read_llm_narrow_raw
    #define LLM_STEP  4
#endif

#define ULL unsigned long long

#define MIPS_CAVIUM_XKPHYS_READ    2010 /* XKPHYS */
#define MIPS_CAVIUM_XKPHYS_WRITE           2011 /* XKPHYS */
                                                                                     
static CVMX_SHARED int32_t warn_count;

int main(int argc, char *argv[])
{
    cvmx_llm_address_t addr = {0};
    uint64_t val;
    uint64_t read_back;
    uint64_t errors = 0;
    FILE *in;

    int32_t w = cvmx_atomic_fetch_and_add32(&warn_count, 1);
    cvmx_linux_enable_xkphys_access(w);
    cvmx_sysinfo_linux_userspace_initialize();

    cvmx_llm_initialize();

    if (argc != 2)
    {
        printf("Usage: %s file_to_load\n", argv[0]);
        return -1;
    }

    in = fopen(argv[1], "r");
    if (in == NULL)
    {
        perror("open");
        return -1;
    }

    while (!feof(in))
    {
        fread(&val, 1, 8, in);
        LLM_WRITE(addr, val, 0);
        read_back = LLM_READ(addr, 0);
        if (val!=read_back && errors++ < MAX_ERRORS)
        {
            printf("LLM Write failed: addr=0x%016llx correct=0x%016llx data=0x%016llx\n",
                   (ULL)addr.u64, (ULL)val, (ULL)read_back);
        }
        addr.s.address+=LLM_STEP;
    }

    addr.s.address=0;
    fseek(in, 0, 0);
    while (!feof(in))
    {
        fread(&val, 1, 8, in);
        read_back = LLM_READ(addr, 0);
        if (val!=read_back && errors++ < MAX_ERRORS)
        {
            printf("LLM Write failed: addr=0x%016llx correct=0x%016llx data=0x%016llx\n",
                   (ULL)addr.u64, (ULL)val, (ULL)read_back);
        }
        addr.s.address+=LLM_STEP;
    }
    fclose(in);

    if (errors)
    {
        printf("...additional errors omitted...\n\n"
               "Error: the contents of file and memory did not match (%lld read errors)\n"
               "       please make sure that data in the file is formatted for the specific width:\n"
               "         CN38XX/CN58XX : 64 bit (wide), 36 bit (narrow)\n"
               "         CN31XX : 64 bit (wide w/o ECC), 56 bit (wide w/ ECC), 32 bit (narrow).\n", CAST64(errors));
    }
    return errors ? -1 : 0;
}
