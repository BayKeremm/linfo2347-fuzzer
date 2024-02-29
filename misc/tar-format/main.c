#include "tar.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <sys/sysmacros.h>
/*
    filename: file to tar, could be a c file like "main.c"
*/
int create_tar_header(char * filename){
    struct stat st;
    if (lstat(filename, &st)){
        printf("Cannot stat %s", filename);
        return -1;
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
int main(int argc, char * argv[]){
    create_tar_header("trash.c");

    return 0;
}