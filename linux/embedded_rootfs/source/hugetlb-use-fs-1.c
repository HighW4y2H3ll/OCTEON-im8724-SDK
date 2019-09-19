  /*
       Example of using hugepage memory in a user application using the mmap
       system call.  Before running this application, make sure that the
       administrator has mounted the hugetlbfs filesystem (on some directory
       like /mnt) using the command mount -t hugetlbfs nodev /mnt. In this
       example, the app is requesting memory of size 2MB that is backed by
       huge pages.

       This test tests MAP_PRIVATE mappings. This program will use up 2*LENGTH 
       memory due to COW.  

     */
    #include <stdlib.h>
    #include <stdio.h>
    #include <unistd.h>
    #include <sys/mman.h>
    #include <fcntl.h>
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <unistd.h>
    #include <string.h>

    #define FILE_NAME "/mnth/hugepagefile"
    #define LENGTH (128UL*1024*1024)
    #define PROTECTION (PROT_READ | PROT_WRITE)

    #define ADDR (void *)(0x0UL)

    void check_bytes(char *addr)
    {
    	printf("First hex is %x\n", *((unsigned int *)addr));
    }

    void write_bytes(char *addr)
    {
    	unsigned long i;

    	for (i = 0; i < LENGTH; i++)
    		*(addr + i) = (char)i;
    }

    void read_bytes(char *addr)
    {
    	unsigned long i;

    	check_bytes(addr);
    	for (i = 0; i < LENGTH; i++)
    		if (*(addr + i) != (char)i) {
    			printf("Mismatch at %lu\n", i);
    			break;
    		}
    }

    int main(void)
    {
    	void *addr;
    	int fd, status=0, ret = 0;
        unsigned long i;

    	fd = open(FILE_NAME, O_CREAT | O_RDWR, 0755);
    	if (fd < 0) {
    		perror("Open failed");
    		exit(1);
    	}

    	addr = mmap(ADDR, LENGTH, PROTECTION, MAP_PRIVATE, fd, 0);
    	if (addr == MAP_FAILED) {
    		perror("mmap");
    		unlink(FILE_NAME);
    		exit(1);
    	}

        /* fault in all pages and fill them with 0s */
        memset(addr, 0, LENGTH);

        if (!fork()) {
                /* child should get new pages for this mapping now */
                check_bytes(addr);
                write_bytes(addr);
                read_bytes(addr);
                exit(0);

        } else {

                if (wait(&status) == -1) {
                        perror("wait");
			ret = 1;
			goto out;
                }

                /* make sure the parent's mapping still contains all 0s */
                for (i=0; i<LENGTH; i+=4) {
                        if (*(int*)(addr+i)) {

                                printf("COW failed for a forked process\n");
				ret = 1;
				goto out;
                        }	
                }
out:
                munmap(addr, LENGTH);
                close(fd);
                unlink(FILE_NAME);
      	        exit(ret);
       }

    }

