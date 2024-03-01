#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tar.h"
#include "test_manager.h"

//#define A 10
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


    char cmd[51];
    strncpy(cmd, argv[1], 25);

    //printf("\n\n\n\n");
    //printf("=============================================================\n");
    //printf("Step 0:         Creating a valid archive.tar from ././files/file.txt ...\n");
    //printf("=============================================================\n\n");
    //test_valid_tar(cmd, 0);


    //int vals[] = {65, 65, 65};
    //tar("archive.tar",1,vals,0,
    //1,1,
    //1,"./files/file.txt");
    //int vals[A] = {65};
    //for (int i = 0; i<10;i++){
        //printf("%d, ",vals[i]);
    //}

    printf("\n\n\n\n");
    printf("=============================================================\n");
    printf("Step 1:         Fuzzing the name field...\n");
    printf("=============================================================\n\n");
    test_name_field(cmd, 1);

    printf("\n\n\n\n");
    printf("=============================================================\n");
    printf("Step 3:         Fuzzing the uid field...\n");
    printf("=============================================================\n\n");
    test_uid_field(cmd, 0);



    //printf("\n\n\n\n");
    //printf("=============================================================\n");
    //printf("Step X:         Testing archives with more than 1 file...   \n");
    //printf("=============================================================\n\n");
    //test_two_files(cmd, 0);
    return 0;
}
