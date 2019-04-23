/*
 * Name: Hannah Raju
 * Date: 31 July 2018
 * Filename lsuvfs.c
 * Details: CSC 360 Assignment #4 - Task #2
 */

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include "disk.h"

// function declarations
int convert_one(int);
int convert_many(int*,int);


char *month_to_string(short m) {
    switch(m) {
    case 1: return "Jan";
    case 2: return "Feb";
    case 3: return "Mar";
    case 4: return "Apr";
    case 5: return "May";
    case 6: return "Jun";
    case 7: return "Jul";
    case 8: return "Aug";
    case 9: return "Sep";
    case 10: return "Oct";
    case 11: return "Nov";
    case 12: return "Dec";
    default: return "?!?";
    }
}


void unpack_datetime(unsigned char *time, short *year, short *month, 
    short *day, short *hour, short *minute, short *second)
{
    assert(time != NULL);

    memcpy(year, time, 2);
    *year = htons(*year);

    *month = (unsigned short)(time[2]);
    *day = (unsigned short)(time[3]);
    *hour = (unsigned short)(time[4]);
    *minute = (unsigned short)(time[5]);
    *second = (unsigned short)(time[6]);
}


int main(int argc, char *argv[]) {
    superblock_entry_t sb;
    int  i;
    char *imagename = NULL;
    FILE *f;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--image") == 0 && i+1 < argc) {
            imagename = argv[i+1];
            i++;
        }
    }

    if (imagename == NULL)
    {
        fprintf(stderr, "usage: lsuvfs --image <imagename>\n");
        exit(1);
    }

	// get superblock info so we can find start of root dir
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
	
	// DIR starts at block 125 -> 256 bytes/block + 125 blocks = byte 3125
	// general: DIR starts at: sb.block_size * sb.dir_start
	
	// run to byte sb.block_size * sb.dir_start = start of root dir
	f =  fopen(imagename, "r");
	fseek(f, sb.block_size*sb.dir_start, SEEK_SET);
	
	
	directory_entry_t dir;
	short year;
	short month;
	short day;
	short hour;
	short min;
	short sec;
	
	while(1){
		
		fread(&dir, 1, 64, f);
		
		if(dir.status==0){
			break;
		}
		
		unpack_datetime(dir.create_time, &year, &month, &day, &hour, &min, &sec);
		printf("%8d %d-%s-%d %02d:%02d:%02d %s\n", htonl(dir.file_size), year, month_to_string(month), day, hour, min, sec, dir.filename);
		
	}

	fclose(f);
	
    return 0; 
}