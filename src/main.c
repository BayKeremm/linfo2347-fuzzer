#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "tar.h"
#include "test_manager.h"
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>


int main(int argc, char* argv[])
{
    if (argc < 2){
        printf("Wrong number of arguements\n");
        return -1;
    }
    printf("=============================================================\n");
    printf("        LINFO2347 - Project 1: Generation based tar fuzzer\n");
    printf("=============================================================\n");
    printf("                     ______\n");
    printf("                   <((((((\\\\\\\n");
    printf("Get fuzzed!        /      . }\\\n");
    printf("                   ;--..--._|}\n");
    printf("(\\                 '--/\\--'  )\n");
    printf(" \\\\                | '-'  :'|\n");
    printf("  \\\\               . -==- .-|\n");
    printf("   \\\\               \\.__.'   \\--._\n");
    printf("   [\\\\          __.--|       //  _/'--.\n");
    printf("   \\ \\\\       .'-._ ('-----'/ __/      \\\n");
    printf("    \\ \\\\     /   __>|      | '--.       |\n");
    printf("     \\ \\\\   |   \\   |     /    /       /\n");
    printf("      \\ '\\ /     \\  |     |  _/       /\n");
    printf("       \\  \\       \\ |     | /        /\n");
    printf("        \\  \\      \\        /\n");
    printf("     .-.     .-.     .-.     .-.     .-.     .-.     .-.\n");
    printf("`._.'   `._.'   `._.'   `._.'   `._.'   `._.'   `._.'   `._.'\n");

    printf("\n\n\n\n");
    printf("=============================================================\n");
    printf("Step 0:         Preparing the environment...\n");
    printf("=============================================================\n\n");

    char cmd[51];
    strncpy(cmd, argv[1], 30);

    printf("=============================================================\n");
    printf("Step 1:         Fuzzing the name field...\n");
    printf("=============================================================\n\n");
    test_field(cmd, 1, NAME_FIELD_OFFSET, NAME_FIELD_LEN);
    printf("=============================================================\n");
    printf("Step 3:         Fuzzing the uid field...\n");
    printf("=============================================================\n\n");
    test_field(cmd, 1, UID_FIELD_OFFSET, UID_FIELD_LEN);

    printf("=============================================================\n");
    printf("Step 4:         Fuzzing the gid field...\n");
    printf("=============================================================\n\n");
    test_field(cmd, 1, GID_FIELD_OFFSET, GID_FIELD_LEN);


    printf("=============================================================\n");
    printf("Step 5:         Fuzzing the size field...\n");
    printf("=============================================================\n\n");
    test_field(cmd, 1, SIZE_FIELD_OFFSET, SIZE_FIELD_LEN);

    printf("=============================================================\n");
    printf("Step 6:         Fuzzing the mtime field...\n");
    printf("=============================================================\n\n");
    test_field(cmd, 1, MTIME_FIELD_OFFSET, MTIME_FIELD_LEN);

    printf("=============================================================\n");
    printf("Step 7:         Fuzzing the chksum field...\n");
    printf("=============================================================\n\n");
    test_field(cmd, 1, CHKSUM_FIELD_OFFSET, CHKSUM_FIELD_LEN);


    printf("=============================================================\n");
    printf("Step 8:         Fuzzing the typeflag field...\n");
    printf("=============================================================\n\n");
    test_field(cmd, 1, TYPEFLAG_FIELD_OFFSET, TYPEFLAG_FIELD_LEN);

    printf("=============================================================\n");
    printf("Step 9:         Fuzzing the linkname field...\n");
    printf("=============================================================\n\n");
    test_field(cmd, 1, LINKNAME_FIELD_OFFSET, LINKNAME_FIELD_LEN);

    printf("=============================================================\n");
    printf("Step 10:         Fuzzing the magic field...\n");
    printf("=============================================================\n\n");
    test_field(cmd, 1, MAGIC_FIELD_OFFSET, MAGIC_FIELD_LEN);

    printf("=============================================================\n");
    printf("Step 11:         Fuzzing the version field...\n");
    printf("=============================================================\n\n");
    test_field(cmd, 1, VERSION_FIELD_OFFSET, VERSION_FIELD_LEN);

    printf("=============================================================\n");
    printf("Step 12:         Fuzzing the uname field...\n");
    printf("=============================================================\n\n");
    test_field(cmd, 1, UNAME_FIELD_OFFSET, UNAME_FIELD_LEN);


    printf("=============================================================\n");
    printf("Step 13:         Fuzzing the gname field...\n");
    printf("=============================================================\n\n");
    test_field(cmd, 1, GNAME_FIELD_OFFSET, GNAME_FIELD_LEN);

    printf("=============================================================\n");
    printf("Step 14:         Testing archives with more than 1 file...   \n");
    printf("=============================================================\n\n");
    test_two_files(cmd, 1);

    printf("=============================================================\n");
    printf("Step 15:         Testing specific cases ...   \n");
    printf("=============================================================\n\n");
    test_specific(cmd, 1);


    return 0;
}

// from: https://stackoverflow.com/questions/2180079/how-can-i-copy-a-file-on-unix-using-c
//int cp(const char *to, const char *from)
//{
    //int fd_to, fd_from;
    //char buf[4096];
    //ssize_t nread;
    //int saved_errno;

    //fd_from = open(from, O_RDONLY);
    //if (fd_from < 0)
        //return -1;

    //fd_to = open(to, O_WRONLY | O_CREAT | O_EXCL, 0777);
    //if (fd_to < 0)
        //goto out_error;

    //while (nread = read(fd_from, buf, sizeof buf), nread > 0)
    //{
        //char *out_ptr = buf;
        //ssize_t nwritten;

        //do {
            //nwritten = write(fd_to, out_ptr, nread);

            //if (nwritten >= 0)
            //{
                //nread -= nwritten;
                //out_ptr += nwritten;
            //}
            //else if (errno != EINTR)
            //{
                //goto out_error;
            //}
        //} while (nread > 0);
    //}

    //if (nread == 0)
    //{
        //if (close(fd_to) < 0)
        //{
            //fd_to = -1;
            //goto out_error;
        //}
        //close(fd_from);

        ///* Success! */
        //return 0;
    //}

  //out_error:
    //saved_errno = errno;

    //close(fd_from);
    //if (fd_to >= 0)
        //close(fd_to);

    //errno = saved_errno;
    //return -1;
//}
    //printf("    - Creating output directory (if not exists)...\n");
    //char original_dir[100];
    //char output_dir[20] = "./output";

    //if (getcwd(original_dir, sizeof(original_dir)) == NULL) {
        //perror("Error getting current directory");
        //exit(EXIT_FAILURE);
    //}
    //printf("        - Current working directory: %s\n",original_dir);

    //if(access(output_dir, F_OK) == 0){
        //printf("        - output directory exists\n");
    //}else{
        //if (mkdir(output_dir, 0777) == 0) {
            //printf("        - Directory created successfully.\n");
        //}

    //}

    //printf("    - Copying the extractor (if not already in ./output))...\n");
    //char to[51];
    //strncpy(to,output_dir, strlen(output_dir));
    //strncat(to, "/", 1);
    //strncat(to, cmd, sizeof(to) - strlen(to) - 1);
    //if(access(to,F_OK)==0){
        //printf("        - Extractor is in the output directory\n");
    //}else{
        //printf("        - Copying the extractor...\n");
        //if(cp(to,cmd)==0){
            //printf("        - File copied successfully\n");
        //}
    //}

    //if (chdir(output_dir) == 0) {
        //printf("    - Changed directory to %s\n", output_dir);
    //}
