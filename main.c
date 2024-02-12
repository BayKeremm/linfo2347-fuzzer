#include <stdio.h>
#include <string.h>
#include "help.c"

void create_tar_file(const char* filename) {
    struct tar_t header;
    memset(&header, 0, sizeof(struct tar_t)); // Initialize header with zeros

    // Fill in header fields with appropriate values
    strcpy(header.name, "file.txt");
    strcpy(header.mode, "0644");
    strcpy(header.uid, "0000");
    strcpy(header.gid, "0000");
    strcpy(header.size, "12");
    strcpy(header.mtime, "00000000000");
    strcpy(header.chksum, "       "); // to be calculated later
    header.typeflag = '0'; // Regular file
    strcpy(header.magic, "ustar");
    strcpy(header.version, "0");
    strcpy(header.uname, "user");
    strcpy(header.gname, "group");
    strcpy(header.devmajor, "000000");
    strcpy(header.devminor, "000000");
    strcpy(header.prefix, "");

    // Calculate and fill checksum
    snprintf(header.chksum, sizeof(header.chksum), "%06o", calculate_checksum(&header));

    // Write header and padding to file
    FILE* tar_file = fopen(filename, "wb");
    fwrite(&header, sizeof(struct tar_t), 1, tar_file);
    fclose(tar_file);
}

int main() {
    create_tar_file("archive.tar");
    return 0;
}