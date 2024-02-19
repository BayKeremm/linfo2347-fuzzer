#include "fuzzer.h"

/*
    fileName: The archive file one wants to edit
    offset: The offset to start editting, byte offset 
    byteSequence: Bytes to write given in string format, it is parsed in
        the function to integers for example "aa11bbcc22" will make it write
        the hex values 0xaa, 0x11, 0xbb,...

fseek(f,offset+j,SEEK_SET);

fputc(num,f);
*/
int write_bytes(char * fileName, unsigned int offset ,char * byteSequence,char * newFileName){
    FILE * f = fopen(fileName,"r+b");

    struct tar_t entry;
    fread(&entry, sizeof(struct tar_t), 1, f);

    
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
    fwrite(&entry, sizeof(struct tar_t), 1, tar_file);
    fclose(tar_file);
        
    fclose(f);
    return 0;
}