#include "tar.h"
#include <stdio.h>

int write_to_tar(char * to, char * from,char apply_padding, char apply_ending_blocks){
    FILE * tar = fopen(to, "ab");
    if (!tar) {
        printf("Cannot open file %s for reading.", to);
        exit(-1);
    }
    FILE * file_to_add = fopen(from, "r");
    if (!file_to_add) {
        printf("Cannot open file %s for reading.", from);
        fclose(tar);
        exit(-1);
    }

    // Copy bytes from source file to destination file
    char buffer[4096];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, 4096, file_to_add)) > 0) {
        fwrite(buffer, 1, bytes_read, tar);
    }
    if(apply_padding){
        // Calculate padding size
        long current_size = ftell(tar);
        long padding_size = 512 - (current_size % 512);

        // Write padding zeros
        for (int i = 0; i < padding_size; i++) {
            fputc(0, tar);
        }

    }
    fclose(file_to_add);
    if(apply_ending_blocks){
        // Write 1024 zeros at the end of the tar file
        char zeros[1024] = {0};
        fwrite(zeros, sizeof(char), 1024, tar);
    }
    fclose(tar);

}

int tar(char * tarname, 
        char edit_head, int * values_to_fill, int offset, int LEN,
        char apply_padding, char apply_ending_blocks,
        int num_of_files, ...){

    va_list args;      
    int i;
    FILE * tar_file = fopen(tarname, "ab");
    
    va_start(args, num_of_files);  

    for (i = 0; i < num_of_files; ++i) {
        char *str = va_arg(args, char*);  
        TAR_HEADER * header;
        create_tar_header(&header,str);

        if(edit_head){
            edit_header(&header, offset, values_to_fill, LEN);
        }

        fwrite(header, sizeof(TAR_HEADER), 1, tar_file);
        free(header);
        header = NULL;

        FILE * file = fopen(str, "r");
        if (!file) {
            printf("Cannot open file %s for reading.", str);
            fclose(tar_file);
            exit(-1);
        }

        // Copy bytes from source file to destination file
        char buffer[4096];
        size_t bytes_read;
        while ((bytes_read = fread(buffer, 1, 4096, file)) > 0) {
            fwrite(buffer, 1, bytes_read, tar_file);
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
        fclose(file);
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
void create_tar_header(TAR_HEADER ** header, char * file_to_tar){
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

    (*header)->typeflag = REGTYPE;
    snprintf((*header) -> linkname, sizeof((*header) -> linkname), "%099o", (int) st.st_nlink);

    memcpy((*header) -> magic, TMAGIC, TMAGLEN);
    memcpy((*header) -> version, TVERSION, TVERSLEN);

    strncpy((*header) -> uname, "User", 5);                     // default value
    strncpy((*header) -> gname, "None", 5);                     // default value

    snprintf((*header) -> devmajor, sizeof((*header) -> devmajor), "%07o", major(st.st_rdev));
    snprintf((*header) -> devminor, sizeof((*header) -> devminor), "%07o", minor(st.st_rdev));

    snprintf((*header)->chksum, sizeof((*header)->chksum), "%06o", calculate_checksum(*header));
}

int save_tar_data(char * tar_file_name, TAR_HEADER * header, char * file_to_tar, char apply_padding,char apply_ending_blocks){
    // Write header and padding to file
    FILE* tar_file = fopen(tar_file_name, "wb");
    fwrite(header, sizeof(TAR_HEADER), 1, tar_file);
    free(header);
    header = NULL;

    // Write file contents to tar file
    FILE * file = fopen(file_to_tar, "r");
    if (!file) {
        printf("Cannot open file %s for reading.", file_to_tar);
        fclose(tar_file);
        exit(-1);
    }

    // Copy bytes from source file to destination file
    char buffer[4096];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, 4096, file)) > 0) {
        fwrite(buffer, 1, bytes_read, tar_file);
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
    if(apply_ending_blocks){
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
