#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tar.h"
#include "test_manager.h"


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

    char * f ="archive.tar"; 

    char cmd[51];
    strncpy(cmd, argv[1], 25);

    printf("=============================================================\n");
    printf("Step 0:         Creating a valid archive.tar from ./file.txt ...\n");
    printf("=============================================================\n\n");

    TAR_HEADER*header;
    create_tar_data(&header,"file.txt");
    if(save_tar_data("archive.tar", header, "file.txt", 1)<0){
        printf("Error saving tar data");
        exit(-1);
    }
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 0.1:       Testing the valid archive.tar\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    int ret = test_archive(cmd,f,1);
    if(ret==0){
        printf("    - Tar file is extracted correctly\n");
    }else if(ret == -1){
        printf("    - Tar file did not extract correctly\n");
        return -1;
    }
    printf("\n\n\n\n");
    printf("=============================================================\n");
    printf("Step 1:         Fuzzing the name field...\n");
    printf("=============================================================\n\n");
    test_name_field(f, cmd);
    printf("\n\n\n\n");



    return 0;
}
