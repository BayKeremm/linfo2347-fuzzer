#include <stdio.h>
#include <string.h>
#include "help.c"

int test_archive(char * name){
    FILE *fp;
    int rv = 0;
    char buf[33];

    if ((fp = popen(name, "r")) == NULL) {
        printf("Error opening pipe!\n");
        return -1;
    }

    if(fgets(buf, 33, fp) == NULL) {
        printf("No output\n");
        goto finally;
    }
    if(strncmp(buf, "*** The program has crashed ***\n", 33)) {
        printf("Not the crash message\n");
        goto finally;
    } else {
        printf("Crash message\n");
        rv = 1;
        goto finally;
    }
    finally:
    if(pclose(fp) == -1) {
        printf("Command not found\n");
        rv = -1;
    }
    return rv;

}
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
    if (argc < 2)
        return -1;

    char cmd[51];
    strncpy(cmd, argv[1], 25);
    cmd[26] = '\0';
    strncat(cmd, " archive.tar", 25);

    printf("Creating a valid tar file...\n");
    create_tar_file("archive.tar");
    printf("Created the valid file now testing...\n");
    int ret = test_archive(cmd);
    if(ret == 0){
        printf("the valid archive extracts correct. \n");
    }

    printf("Starting testing the name field (printing crash messages)...\n");

}