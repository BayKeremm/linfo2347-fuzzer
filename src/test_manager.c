#include "test_manager.h"
#include "fuzzer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    file_name field is a null termintated character string
*/
int test_name_field(char * tarName, char * cmd) {
    char * buff = malloc(sizeof(char)*100);
    // TEST 1 write 0s 
    printf("Testing writing '0's to the file_name field...\n");
    memset(buff,0,100);
    char * file_name = "archive_zeros.tar";
    write_bytes(tarName, 0, 
        buff, file_name);
    test_archive(cmd,file_name);
    free(buff);
    buff = NULL;
    

    // TEST 2 write character string longer than 100 
    printf("Testing writing 150 characters to name field\n");
    buff = malloc(sizeof(char)*400);
    memset(buff,128,400);
    file_name = "archive_long_name.tar";
    write_bytes(tarName, 0, 
        buff, file_name);
    test_archive(cmd,file_name);
    free(buff);
    buff = NULL;

    // TEST 3 write random numbers
    return 0;
}

int test_archive(char * cmd, char * file_name){
    char buff[51];
    strncpy(buff,cmd, sizeof(buff) - strlen(file_name) - 1);
    strncat(buff, " ", 1);
    strncat(buff, file_name, sizeof(buff) - strlen(buff) - 1);
    //printf("Buff is: %s\n",buff);
    FILE *fp;
    int rv = 0;
    char buf[33];

    if ((fp = popen(buff, "r")) == NULL) {
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
    //if (rv!= 1)remove(file_name);
    return rv;


}
/*
    int offset = 0;
    char file_name[20];
    for(unsigned short int i = 128; i <= 255; i++) {
        snprintf(file_name, sizeof(name), "name_test_%02x", i);
        char hex[3];
        snprintf(hex, sizeof(hex), "%02x", i);

        char buff[51];
        strncpy(buff,cmd, sizeof(buff) - strlen(file_name) - 1);
        strncat(buff, " ", 1);
        strncat(buff, file_name, sizeof(buff) - strlen(buff) - 1);
        //printf("%s ", buff);

        write_bytes(tarfile_name,  offset,hex, name);
        int ret = test_archive(buff);
        if(ret==1){
            printf("Fucker crashed for the file_name %s\n", name);
        }else{
            remove(file_name);
        }

    }
    return 0;
*/