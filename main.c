#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"
#include "fuzzer.h"
#include "test_manager.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void create_tar_file(const char* filename) {
    struct tar_t header;
    memset(&header, 0, sizeof(struct tar_t)); // Initialize header with zeros

    // Fill in header fields with appropriate values
    strcpy(header.name, "file.txt");
    strcpy(header.mode, "0000664");
    strcpy(header.uid, "0001750");
    strcpy(header.gid, "0001750");
    strcpy(header.size, "00000000040");
    strcpy(header.mtime, "14564715214");
    strcpy(header.chksum, "       "); // to be calculated later
    header.typeflag = '0'; // Regular file
    strcpy(header.magic, "ustar");
    strcpy(header.version, "00");
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

/**
 * Launches another executable given as argument,
 * parses its output and check whether or not it matches "*** The program has crashed ***".
 * @param the path to the executable
 * @return -1 if the executable cannot be launched,
 *          0 if it is launched but does not print "*** The program has crashed ***",
 *          1 if it is launched and prints "*** The program has crashed ***".
 *
 * BONUS (for fun, no additional marks) without modifying this code,
 * compile it and use the executable to restart our computer.
 */
int main(int argc, char* argv[])
{
    if (argc < 2){
        printf("Wrong number of arguements\n");
        return -1;
    }

    char * f ="archive.tar"; 

    char cmd[51];
    strncpy(cmd, argv[1], 25);


    printf("Creating a valid tar file...\n");
    create_tar_file(f);
    printf("Created the valid file now testing...\n");

    char test1[51];
    strncpy(test1,cmd, sizeof(cmd));
    strncat(test1, " archive.tar", 25);
    int ret = test_archive(test1);
    if(ret == 0){
        printf("the valid archive extracts correct. \n");
    }

    //printf("Creating the output directory...\n");
    //mkdir("./output", 0700);
    printf("Starting testing the name field (printing crash messages)...\n");
    test_name_field(f, cmd);

    //printf("Removing the output directory...\n");
    //rmdir("./output");


    return 0;
}