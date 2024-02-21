#include "tar.h"
#include <stdio.h>

/*
    filename: file to tar, could be a c file like "main.c"
*/
void create_tar_data(TAR_HEADER ** header, char * file_to_tar){
    struct stat st;
    if (lstat(file_to_tar, &st)){
        printf("Cannot stat %s\n", file_to_tar);
        exit(-1);
    }
    *header = (TAR_HEADER*)malloc(sizeof(TAR_HEADER));
    memset(*header, 0, sizeof(TAR_HEADER));
    strncpy((*header) -> name, file_to_tar, 100); //at most 100bytes
    snprintf((*header) -> mode,  sizeof((*header) -> mode),  "%07o", st.st_mode & 0777);
    snprintf((*header) -> uid,   sizeof((*header) -> uid),   "%07o", st.st_uid);
    snprintf((*header) -> gid,   sizeof((*header) -> gid),   "%07o", st.st_gid);
    snprintf((*header) -> size,  sizeof((*header) -> size),  "%011o", (int) st.st_size);
    snprintf((*header) -> mtime, sizeof((*header) -> mtime), "%011o", (int) st.st_mtime);
    strncpy((*header) -> gname, "None", 5);                     // default value
    memcpy((*header) -> magic, TMAGIC, TMAGLEN);
    memcpy((*header) -> version, TVERSION, TVERSLEN);

    snprintf((*header) -> devmajor, sizeof((*header) -> devmajor), "%07o", major(st.st_rdev));
    snprintf((*header) -> devminor, sizeof((*header) -> devminor), "%07o", minor(st.st_rdev));

    snprintf((*header)->chksum, sizeof((*header)->chksum), "%06o", calculate_checksum(*header));
}

int save_tar_data(char * tar_file_name, TAR_HEADER * header, char * file_to_tar, char apply_padding){
    // Write header and padding to file
    FILE* tar_file = fopen(tar_file_name, "wb");
    fwrite(header, sizeof(TAR_HEADER), 1, tar_file);
    free(header);

    // Write file contents to tar file
    FILE * file = fopen(file_to_tar, "r");
    if (!file) {
        printf("Cannot open file %s for reading.", file_to_tar);
        fclose(tar_file);
        return -1;
    }

    char c;
    while ((c = fgetc(file)) != EOF) {
        fputc(c, tar_file);
    }

    if(apply_padding){
        // Calculate padding size
        long current_size = ftell(tar_file);
        long padding_size = 512 - (current_size % 512);

        // Write padding zeros
        for (int i = 0; i < padding_size; i++) {
            fputc(0, tar_file);
        }

        // Write 1024 zeros at the end of the tar file
        char zeros[1024] = {0};
        fwrite(zeros, sizeof(char), 1024, tar_file);
    }
    fclose(file);
    fclose(tar_file);

    return 0;
}
/*
    fileName: The archive file one wants to edit
    offset: The offset to start editting, byte offset 
    byteSequence: Bytes to write given in string format, it is parsed in
        the function to integers for example "aa11bbcc22" will make it write
        the hex values 0xaa, 0x11, 0xbb,...
*/
void edit_header(TAR_HEADER ** header, unsigned int offset ,char * byteSequence){

    int len = strlen(byteSequence);
    //printf("byte sequence: %s length %d\n",byteSequence,len);

    // Iterate through the string two characters at a time
    int j=0;
    for (int i = 0; i < len; i += 2) {
        // Extract two characters
        char hex[3] = {byteSequence[i], byteSequence[i+1], '\0'};
        
        // Convert hex string to integer
        int num = strtol(hex, NULL, 16);

        // write num to offset + j
        //*((char * )(*header) + offset + j) = num;
        (((char*)(*header))[offset + j])= num;
        j++;
    }

    // Calculate and fill checksum
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
