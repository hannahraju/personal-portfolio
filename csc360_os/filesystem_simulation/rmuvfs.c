/*
 * Name: Hannah Raju
 * Date: 31 July 2018
 * Filename rmuvfs.c
 * Details: CSC 360 Assignment #4 - Bonus
 */
 
#include <arpa/inet.h>
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include "disk.h"

int main(int argc, char *argv[]) {
    superblock_entry_t sb;
    int  i;
    char *imagename = NULL;
    char *filename  = NULL;
    FILE *f;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--image") == 0 && i+1 < argc) {
            imagename = argv[i+1];
            i++;
        } else if (strcmp(argv[i], "--file") == 0 && i+1 < argc) {
            filename = argv[i+1];
            i++;
        }
    }

    if (imagename == NULL || filename == NULL) {
        fprintf(stderr, "usage: rmuvfs --image <imagename> " \
            "--file <filename in image>");
        exit(1);
    }
	
	// read in superblock
	f = fopen(imagename, "r+");
	if(f==NULL){
		fprintf(stderr, "error: file not found.\n");
	}
	
	fread(&sb, 1, 30, f);
	sb.block_size = htons(sb.block_size);
	sb.num_blocks = htonl(sb.num_blocks);
	sb.fat_start = htonl(sb.fat_start);
	sb.fat_blocks = htonl(sb.fat_blocks);
	sb.dir_start = htonl(sb.dir_start);
	sb.dir_blocks = htonl(sb.dir_blocks);
	
	// seek to start of dir entries
	fseek(f, sb.dir_start*sb.block_size, SEEK_SET);
	
	// find dir entry with matching file name
	directory_entry_t dir;
	
	while(1){
		
		fread(&dir, 1, SIZE_DIR_ENTRY, f);
		
		if(dir.status == 0){
			fprintf(stderr, "error: file does not exist.\n");
		}
		
		if(strcmp(dir.filename, filename) == 0){
			break;
		}
	}
	
	// determine entry # in FAT
	int fat_num;
	fat_num = htonl(dir.start_block) - (sb.dir_start+sb.dir_blocks);
	
	
	// set that dir entry status to 0
	fseek(f, -64, SEEK_CUR);
	unsigned char status;
	status = DIR_ENTRY_AVAILABLE;
	fwrite(&status, 1, 1, f);
	fclose(f);
	
	return 0;
}