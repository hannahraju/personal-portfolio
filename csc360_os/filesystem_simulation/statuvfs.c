/*
 * Name: Hannah Raju
 * Date: 31 July 2018
 * Filename statuvfs.c
 * Details: CSC 360 Assignment #4 - Task #1
 * 
 * References:
 * - https://stackoverflow.com/questions/7841926/how-to-print-only-certain-parts-of-a-string?lq=1
 * - formatting: https://stackoverflow.com/questions/1505311/how-can-i-clean-up-misaligned-columns-in-text
 *
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include "disk.h"


int convert(int*);
int main(int argc, char *argv[]) {
    superblock_entry_t sb;
    int  i;
    char *imagename = NULL;
    FILE  *f;
	//int   *fat_data;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--image") == 0 && i+1 < argc) {
            imagename = argv[i+1];
            i++;
        }
    }

    if (imagename == NULL)
    {
        fprintf(stderr, "usage: statuvfs --image <imagename>\n");
        exit(1);
    }
	
	f = fopen(imagename, "r");
	
	if(f==NULL){
		fprintf(stderr, "Error: file not found.\n");
		exit(1);
	}
	fread(&sb, 1, 30, f);
	fclose(f);
	
	sb.block_size = htons(sb.block_size);
	sb.num_blocks = htonl(sb.num_blocks);
	sb.fat_start = htonl(sb.fat_start);
	sb.fat_blocks = htonl(sb.fat_blocks);
	sb.dir_start = htonl(sb.dir_start);
	sb.dir_blocks = htonl(sb.dir_blocks);
	
	f = fopen(imagename, "r");
	
	int c;
	int n = 0;
	
	// get to end of superblock -> FAT starts at 256
	fseek(f, sb.block_size, SEEK_SET);
	/*
	while(n<sb.block_size){
		c = fgetc(f);
		if(c==EOF){
			break;
		}
		n++;
		
	}
	*/
	// FAT only 1 block large
	
	// iterate through FAT and determine how many entries are res/alloc
	
	//int num_alloc = 0;
	int num_res = 0;
	int num_free = 0;
	int j;
	int nums[4];
	
	for(j=0; j<sb.num_blocks; j++){
		for(i=0; i<4; i++){
			
			c = fgetc(f);
			if(c==EOF){
				fprintf(stderr, "Error: end of file reached.. invalid file");
			}
			nums[i] = c;
			
		}
		
		int dec;
		dec = convert(nums);
		
		// check if reserved
		if(dec == FAT_RESERVED){
			num_res++;
		}
		
		// check if free
		else if(dec == FAT_AVAILABLE){
			num_free++;
		}
		
	}
	
	fclose(f);
	printf("%s (%s)\n\n", FILE_SYSTEM_ID, &imagename[7]);
	fprintf(stdout, "-------------------------------------------------\n");
	fprintf(stdout, "  Bsz   Bcnt  FATst FATcnt  DIRst DIRcnt\n");
	fprintf(stdout, "%5d %6d %6d %6d %6d %6d\n\n", sb.block_size, sb.num_blocks, sb.fat_start, sb.fat_blocks, sb.dir_start, sb.dir_blocks);
	fprintf(stdout, "-------------------------------------------------\n");
	fprintf(stdout, " Free   Resv  Alloc\n");
	
	// num reserved blocks is size of entries in FAT -> ie where dir starts
	// num alloc blocks is how many are allocated + num dirs
	fprintf(stdout, "%5d %6d %6d\n", num_free, num_res, sb.num_blocks-(num_free+num_res));
	
	
    return 0;  
}

int convert(int * nums){
	char hex[10];
	int dec = 100;
	sprintf(hex, "%x%x%x%x", nums[0], nums[1], nums[2], nums[3]);
	sscanf(hex, "%x", &dec);
	
	return dec;
}
