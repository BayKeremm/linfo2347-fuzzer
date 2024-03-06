#include "tar.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#define SIZE 300
#define SIZE_STR "00000000300"


int tar(char * tarname, 
        char edit_head, int * values_to_fill, int offset, int LEN,
        char apply_padding, char apply_ending_blocks,
        int num_of_files){

    int i;
    FILE * tar_file = fopen(tarname, "ab");
    
    for (i = 0; i < num_of_files; ++i) {
        TAR_HEADER * header;

        char new = 0;
        if(num_of_files % 2 == 0){
            new = 1;
        }
        create_tar_header(&header,new);

        if(edit_head){
            edit_header(&header, offset, values_to_fill, LEN);
        }

        fwrite(header, sizeof(TAR_HEADER), 1, tar_file);
        free(header);
        header = NULL;

        srand(time(NULL)); 

        // Define the size of random data to generate
        int random_data_size = SIZE;

        // Generate random data
        char random_data[random_data_size];
        for (int j = 0; j < random_data_size; j++) {
            random_data[j] = rand() % 256; // Generate random byte (0-255)
        }

        // Write random data to the tar file
        size_t bytes_written = fwrite(random_data, 1, random_data_size, tar_file);
        if (bytes_written < random_data_size) {
            // Error handling if not all data could be written
            printf("Error writing random data to tar file.");
            fclose(tar_file);
            exit(-1);
        }
        if(apply_padding){
            // Calculate padding size
            long current_size = ftell(tar_file);
            long padding_size = 512 - (current_size % 512);

            // Write padding zeros
            for (int i = 0; i < padding_size; i++) {
                fputc(0, tar_file);
            }

        }
    }
    if(apply_ending_blocks){
        char zeros[1024] = {0};
        fwrite(zeros, sizeof(char), 1024, tar_file);
    }
    fclose(tar_file);
    return 0;
}

/*
    filename: file to tar, could be a c file like "main.c"
*/
static int count = 0; 
void create_tar_header(TAR_HEADER ** header, int new_name){
    *header = (TAR_HEADER*)malloc(sizeof(TAR_HEADER));
    memset(*header, 0, sizeof(TAR_HEADER));

    
    if(new_name){
        count++;
    }
    char file[50];
    snprintf(file, sizeof(file), "file%d.txt", count );

    strncpy((*header) -> name, file, 100); //at most 100bytes

    char * uid = "00200";
    strncpy((*header) -> uid, uid, 8); //at most 100bytes
    
    char * gid = "00200";
    strncpy((*header) -> gid, gid, 8); //at most 100bytes

    char * str = SIZE_STR;
    strncpy((*header)->size, str, 12); //at most 100bytes

    memcpy((*header) -> magic, TMAGIC, TMAGLEN);
    memcpy((*header) -> version, TVERSION, TVERSLEN);

    char * linkname = "this is a linkname";
    strncpy((*header) -> linkname, linkname, 100); //at most 100bytes

    snprintf((*header)->chksum, sizeof((*header)->chksum), "%06o", calculate_checksum(*header));
}
void edit_header(TAR_HEADER ** header, unsigned int offset ,int * byteSequence, int LEN){
    // Iterate through the string two characters at a time
    int j=0;
    for (int i = 0; i < LEN; i ++) {
        //printf("%d, ",byteSequence[i]);
        (((char*)(*header))[offset + j])= byteSequence[i];
        j++;
    }

    // Calculate and fill checksum
    if(offset != 148)
    snprintf((*header)->chksum, sizeof((*header)->chksum), "%06o", calculate_checksum(*header));
}
/**
 * Computes the checksum for a tar header and encode it on the header
 * @param entry: The tar header
 * @return the value of the checksum
 */
unsigned int calculate_checksum(TAR_HEADER * entry){
    // use spaces for the checksum bytes while calculating the checksum
    memset(entry->chksum, ' ', 8);

    // sum of entire metadata
    unsigned int check = 0;
    unsigned char* raw = (unsigned char*) entry;
    for(int i = 0; i < 512; i++){
        check += raw[i];
    }

    snprintf(entry->chksum, sizeof(entry->chksum), "%06o0", check);

    entry->chksum[6] = '\0';
    entry->chksum[7] = ' ';
    return check;
}
