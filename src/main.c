#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tar.h"
#include "fuzzer.h"
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

    char * f ="archive.tar"; 

    char cmd[51];
    strncpy(cmd, argv[1], 25);
    printf("Command is: %s\n",cmd);


    printf("Creating a valid tar file...\n");
    create_tar_data("file.txt");
    printf("Created the valid file\n");
    //test_archive(cmd,f);

    //printf("Creating the output directory...\n");
    //mkdir("./output", 0700);
    printf("Starting testing the name field (printing crash messages)...\n");
    //test_name_field(f, cmd);

    //printf("Removing the output directory...\n");
    //rmdir("./output");


    return 0;
}

/*
    char test1[51];
    strncpy(test1,cmd, sizeof(cmd));
    strncat(test1, " archive.tar", 25);
    int ret = test_archive(test1,f);
    if(ret == 0){
        printf("the valid archive extracts correct. \n");
    }
*/