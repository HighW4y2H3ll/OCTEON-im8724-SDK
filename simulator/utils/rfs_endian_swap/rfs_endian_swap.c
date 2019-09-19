#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>


void endian_swap(uint64_t *x)
{
    char y[8];
    char *in = (char *) x;
    int i;

    for(i=0;i<8;i++)
    {
        y[i] = in[7-i];
    }
    for(i=0;i<8;i++)
    {
        in[i] = y[i];
    }

}

void usage()
{
    printf("Usage : rfs_endian_swap <source_rootfs> <converted_rootfs>\n");
    printf("        This program converts the source_rootfs file\n");
    printf("        to converted_rootfs file by swapping 64bit endianess\n");
}
 
int main(int argc, char *argv[])
{
    FILE *fs,*ft;

    if(argc!=3) 
    {
        printf("ERROR: More arguments needed.\n");
        usage();
        exit(-1);
    }
    fs = fopen(argv[1],"r");
    if(fs==NULL)
    {
        printf("ERROR: Cannot open file %s\n", argv[1]);
        exit(-1);
    }
 
    ft = fopen(argv[2],"w");
    if(ft==NULL)
    {
        printf("ERROR: Cannot open file %s\n", argv[2]);
        fclose(fs);
        exit(-1);
    }
 
    while(1)
    {
#define DATASZ 2048
        char data[DATASZ];
        int read_len, write_len, i;
        uint64_t *data_ptr = (uint64_t *) &data;

        read_len = fread (&data, 1, DATASZ, fs);
        //printf("read_len=%d\n", read_len);
        for(i=0; i<(read_len/8); i++)
        {
            endian_swap(data_ptr);
            data_ptr++;
        }
        write_len = fwrite(&data, 1, read_len,ft);
        if (write_len != read_len) 
        {
            printf("Error writing to file %s\n",argv[2]);
            fclose(fs);
            fclose(ft);
            exit(-1);
        }       
        if (read_len != DATASZ) 
        {
            if ((read_len%8) != 0) 
                printf("ERROR: File size not a multiple of 8\n");
            break;
        }
        
    }
    fclose(fs);
    fclose(ft);
    return 0;
}
