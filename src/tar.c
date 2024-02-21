#include "tar.h"

/*
    filename: file to tar, could be a c file like "main.c"
*/
int create_tar_data(char * filename){
    struct stat st;
    if (lstat(filename, &st)){
        printf("Cannot stat %s\n", filename);
        exit(-1);
    }
    TAR_HEADER * header = malloc(sizeof(TAR_HEADER));
    memset(header, 0, sizeof(TAR_HEADER));
    strncpy(header -> name, filename, 100); //at most 100bytes
    snprintf(header -> mode,  sizeof(header -> mode),  "%07o", st.st_mode & 0777);
    snprintf(header -> uid,   sizeof(header -> uid),   "%07o", st.st_uid);
    snprintf(header -> gid,   sizeof(header -> gid),   "%07o", st.st_gid);
    snprintf(header -> size,  sizeof(header -> size),  "%011o", (int) st.st_size);
    snprintf(header -> mtime, sizeof(header -> mtime), "%011o", (int) st.st_mtime);
    strncpy(header -> gname, "None", 5);                     // default value
    memcpy(header -> magic, TMAGIC, TMAGLEN);
    memcpy(header -> version, TVERSION, TVERSLEN);

    snprintf(header -> devmajor, sizeof(header -> devmajor), "%07o", major(st.st_rdev));
    snprintf(header -> devminor, sizeof(header -> devminor), "%07o", minor(st.st_rdev));

    snprintf(header->chksum, sizeof(header->chksum), "%06o", calculate_checksum(header));

    // Write header and padding to file
    FILE* tar_file = fopen("archive.tar", "wb");
    fwrite(header, sizeof(TAR_HEADER), 1, tar_file);
    free(header);

    // Write file contents to tar file
    FILE *file_to_tar = fopen(filename, "r");
    if (!file_to_tar) {
        printf("Cannot open file %s for reading.", filename);
        fclose(tar_file);
        return -1;
    }

    char c;
    while ((c = fgetc(file_to_tar)) != EOF) {
        fputc(c, tar_file);
    }

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

    fclose(tar_file);
    fclose(file_to_tar);

    return 0;
}
/*
    fileName: The archive file one wants to edit
    offset: The offset to start editting, byte offset 
    byteSequence: Bytes to write given in string format, it is parsed in
        the function to integers for example "aa11bbcc22" will make it write
        the hex values 0xaa, 0x11, 0xbb,...

fseek(f,offset+j,SEEK_SET);

fputc(num,f);
*/
int create_tar(char * fileName, unsigned int offset ,char * byteSequence,char * newFileName){
    //printf("inside write_bytes\n");
    FILE * f = fopen(fileName,"r");

    TAR_HEADER entry;
    fread(&entry, sizeof(TAR_HEADER), 1, f);
    fclose(f);

    
    int len = strlen(byteSequence);

    // Iterate through the string two characters at a time
    int j=0;
    for (int i = 0; i < len; i += 2) {
        // Extract two characters
        char hex[3] = {byteSequence[i], byteSequence[i+1], '\0'};
        
        // Convert hex string to integer
        int num = strtol(hex, NULL, 16);

        // write num to offset + j
        *((char * )&entry + offset + j) = num;
        j++;
    }

    // Calculate and fill checksum
    snprintf(entry.chksum, sizeof(entry.chksum), "%06o", calculate_checksum(&entry));

    // Write header and padding to file
    FILE* tar_file = fopen(newFileName, "wb");
    fwrite(&entry, sizeof(TAR_HEADER), 1, tar_file);
    fclose(tar_file);
        
    return 0;
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
