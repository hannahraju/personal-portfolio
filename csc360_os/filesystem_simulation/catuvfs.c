/*
 * Name: Hannah Raju
 * Date: 31 July 2018
 * Filename catuvfs.c
 * Details: CSC 360 Assignment #4 - Task #3
 */
 
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
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
        fprintf(stderr, "usage: catuvfs --image <imagename> " \
            "--file <filename in image>");
        exit(1);
    }

	// get superblock info so we can find file we are looking for
	f = fopen(imagename, "r");
	
	if(f == NULL){
		fprintf(stderr, "File not found.\n");
		exit(1);
	}
	fread(&sb, 1, 30, f);
	sb.block_size = htons(sb.block_size);
	sb.num_blocks = htonl(sb.num_blocks);
	sb.fat_start = htonl(sb.fat_start);
	sb.fat_blocks = htonl(sb.fat_blocks);
	sb.dir_start = htonl(sb.dir_start);
	sb.dir_blocks = htonl(sb.dir_blocks);
	fclose(f);
	
	// jump to root directory
	f = fopen(imagename, "r");
	fseek(f, sb.block_size*sb.dir_start, SEEK_SET);
	
	// iterate through files in root directory to find filename
	directory_entry_t dir;

	while(1){
		
		fread(&dir, 1, 64, f);
		
		if(strcmp(dir.filename,filename)==0){
			break;
		}
		
		if(dir.status == 0){
			fprintf(stderr, "Error: specified file not found in root directory.\n");
			exit(1);
		}
	}
	
	// get block containing contents of specified file
	
	fseek(f, htonl(dir.start_block)*sb.block_size, SEEK_SET);
	
	// print contents of file
	for(i=0; i<htonl(dir.file_size); i++){
		printf("%c", fgetc(f));
	}
	
	
    return 0; 
}
