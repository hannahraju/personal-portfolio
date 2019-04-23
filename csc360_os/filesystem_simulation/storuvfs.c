/*
 * Name: Hannah Raju
 * Date: 31 July 2018
 * Filename storuvfs.c
 * Details: CSC 360 Assignment #4 - Task #4
 */
 
 // check for filename already existing

#include <arpa/inet.h>
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include "disk.h"


/*
 * Based on http://bit.ly/2vniWNb
 */
 
void pack_current_datetime(unsigned char *entry) {
    assert(entry);

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    unsigned short year   = tm.tm_year + 1900;
    unsigned char  month  = (unsigned char)(tm.tm_mon + 1);
    unsigned char  day    = (unsigned char)(tm.tm_mday);
    unsigned char  hour   = (unsigned char)(tm.tm_hour);
    unsigned char  minute = (unsigned char)(tm.tm_min);
    unsigned char  second = (unsigned char)(tm.tm_sec);

    year = htons(year);

    memcpy(entry, &year, 2);
    entry[2] = month;
    entry[3] = day;
    entry[4] = hour;
    entry[5] = minute;
    entry[6] = second; 
}


int next_free_block(int *FAT, int max_blocks) {
    assert(FAT != NULL);

    int i;

    for (i = 0; i < max_blocks; i++) {
        if (FAT[i] == FAT_AVAILABLE) {
            return i;
        }
    }

    return -1;
}

int convert(int* nums);


int main(int argc, char *argv[]) {
    superblock_entry_t sb;
    int  i;
    char *imagename  = NULL;
    char *filename   = NULL;
    char *sourcename = NULL;
    FILE *f;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--image") == 0 && i+1 < argc) {
            imagename = argv[i+1];
            i++;
        } else if (strcmp(argv[i], "--file") == 0 && i+1 < argc) {
            filename = argv[i+1];
            i++;
        } else if (strcmp(argv[i], "--source") == 0 && i+1 < argc) {
            sourcename = argv[i+1];
            i++;
        }
    }

    if (imagename == NULL || filename == NULL || sourcename == NULL) {
        fprintf(stderr, "usage: storuvfs --image <imagename> " \
            "--file <filename in image> " \
            "--source <filename on host>\n");
        exit(1);
    }
	
	if(strlen(filename) > 30){
		fprintf(stderr, "file name too long, max 30 characters");
	}
	// scan through source file to determine its length
	
	int c;
	int len=0;
	f = fopen(sourcename, "r");
	if(f==NULL){
		fprintf(stderr, "error: host file does not exist.\n");
		exit(1);
	}
	while(1){
		c = fgetc(f);
		if(c==EOF){
			break;
		}
		len++;
		
	}	
	fclose(f);
	
	
	// scan source file again and store in array of length
	char source[len];
	f = fopen(sourcename, "r");
	fread(&source, 1, len, f);
	fclose(f);
	
	// read in superblock info
	f = fopen(imagename, "r+");
	fread(&sb, 1, 30, f);
	sb.block_size = htons(sb.block_size);
	sb.num_blocks = htonl(sb.num_blocks);
	sb.fat_start = htonl(sb.fat_start);
	sb.fat_blocks = htonl(sb.fat_blocks);
	sb.dir_start = htonl(sb.dir_start);
	sb.dir_blocks = htonl(sb.dir_blocks);
	
	
	// find available entry in FAT for file contents to be written
	
		// Make FAT array
		int FAT[sb.num_blocks];
		int temp[4];
		int j;
	
		// read in 4 bytes at a time from fat and convert to single value
		fseek(f, sb.fat_start*sb.block_size, SEEK_SET);
		for(i=0; i<sb.num_blocks; i++){
			for(j=0; j<4; j++){
				temp[j] = fgetc(f);
			}
			FAT[i] = convert(temp);
		}
	
		// determine first available entry in FAT
		int av_entry;
		av_entry = next_free_block(FAT, sb.num_blocks);
		
		if(av_entry == -1){
			fprintf(stderr, "error: no available room on disk image.\n");
			exit(1);
		}
	
	// jump to dir blocks & find first available spot for dir entry
	// && check if that filename already exists
	fseek(f, sb.dir_start*sb.block_size, SEEK_SET);
	directory_entry_t dir;
	int dir_count = 0;
	while(1){
	
		fread(&dir, 1, SIZE_DIR_ENTRY, f);

		if(dir.status == 0){
			break;
		}
		if(strcmp(dir.filename, filename) == 0){
			fprintf(stderr, "error: filename already exists.\n");
			exit(1);
		}
		
		dir_count++;

	}
	// get back to correct spot, after last dir entry
	fseek(f, -1*SIZE_DIR_ENTRY, SEEK_CUR);
	
	// create DIR entry
	unsigned char status;
	unsigned int data;
	unsigned char time [DIR_TIME_WIDTH];
	unsigned char p [DIR_PADDING_SIZE];
	char fname [ DIR_FILENAME_MAX];
	// STATUS
	status = DIR_ENTRY_NORMALFILE;
	fwrite(&status, 1, 1,f);
	
	// START BLOCK
	data = ntohl(sb.dir_start+sb.dir_blocks+av_entry);
	fwrite(&data, 1, 4, f);
	
	// NUM BLOCKS IN FILE
	data = ntohl((len/sb.block_size) + 1);
	fwrite(&data, 1, 4, f);
	
	// FILE SIZE
	data = ntohl(len);
	fwrite(&data, 1, 4, f);
	
	// CREATE & MODIFY TIMES
	pack_current_datetime(time);
	fwrite(&time, 1, DIR_TIME_WIDTH, f);
	fwrite(&time, 1, DIR_TIME_WIDTH, f);
	
	strcpy(fname, filename);
	fwrite(&fname,1, 30, f);
	
	for(i=0; i<DIR_PADDING_SIZE; i++){
		p[i] = 0xff;
	}
	
	fwrite(p, 1, 6, f);
	
	//write contents text to data block
	fseek(f, (sb.dir_start+sb.dir_blocks+av_entry)*sb.block_size, SEEK_SET);
	fwrite(&source, 1, len, f);
    return 0; 
}

int convert(int* nums){
	char hex[10];
	int dec = 100;
	sprintf(hex, "%x%x%x%x", nums[0], nums[1], nums[2], nums[3]);
	sscanf(hex, "%x", &dec);
	
	return dec;
}
	
	
