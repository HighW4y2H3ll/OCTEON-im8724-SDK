#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <getopt.h>
#include <dirent.h>

#define MAP_0_SIZE		256
#define VEN_ASCII_LEN	16
#define MAX_FILE_NAME	256
#define STR_I2C			"i2c"

static void find_i2c_folder(char *work_folder, char *mask, char *outBuf, int outBufLen)
{
	DIR *d;
	FILE *pFile;
	struct dirent *dir;
	size_t iResult;
	char filename[MAX_FILE_NAME+1];
	char ptCompare[256+1];

	d = opendir(work_folder);
	if (d) {
		while ((dir = readdir(d)) != NULL) {
			if(dir->d_type == DT_DIR) {
				if(strncmp(dir->d_name, STR_I2C, strlen(STR_I2C)) == 0) {
					snprintf (filename, MAX_FILE_NAME, "%s/%s/name", work_folder, dir->d_name);
					pFile = fopen(filename, "r");
					if(pFile) {
						iResult = fread(ptCompare, 1, sizeof(ptCompare)-1, pFile);
						if((iResult >= strlen(mask)) && (strncmp(ptCompare, mask, strlen(mask)) == 0)) {
							snprintf(outBuf, outBufLen-1, "%s/%s",
								work_folder, dir->d_name);
							break;
						}
						fclose(pFile);
					}
				}
			}
		}

		closedir(d);
	};
}

int find_i2c_device(char *devname, char *outBuf, int outBufLen)
{
	int iRet = 0; // we have not found device yet)
    char i2c_folder[256] = { 0 };
	char dev_folder[256] = { 0 };

	if(outBufLen > 1) {
		*outBuf = '\0';

		// first we need to find correct i2c folder name in /sys/devices
		find_i2c_folder("/sys/devices", "im_native", i2c_folder, sizeof(i2c_folder));
		if(*i2c_folder) {
			// next - searching for sfp in i2c folder
			find_i2c_folder(i2c_folder, devname, dev_folder, sizeof(dev_folder));
			if(*dev_folder) {
				snprintf(outBuf, outBufLen, "%s/%s", dev_folder, &dev_folder[strlen(i2c_folder)+strlen(STR_I2C)+2]);
				iRet = 1;
			}
		}
	}

	return iRet;
}
