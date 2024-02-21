#include "test_manager.h"
#include "fuzzer.h"
#include <stdio.h>
#include <string.h>

int test_name_field(char * tarName, char * cmd) {
    int offset = 0;
    char name[20];
    for(unsigned short int i = 128; i <= 255; i++) {
        snprintf(name, sizeof(name), "name_test_%02x", i);
        char hex[3];
        snprintf(hex, sizeof(hex), "%02x", i);

        char buff[51];
        strncpy(buff,cmd, sizeof(buff) - strlen(name) - 1);
        strncat(buff, " ", 1);
        strncat(buff, name, sizeof(buff) - strlen(buff) - 1);
        //printf("%s ", buff);

        write_bytes(tarName,  offset,hex, name);
        int ret = test_archive(buff);
        if(ret==1){
            printf("Fucker crashed for the name %s\n", name);
        }else{
            remove(name);
        }

    }
    return 0;
}

int test_archive(char * name){
    FILE *fp;
    int rv = 0;
    char buf[33];

    if ((fp = popen(name, "r")) == NULL) {
        printf("Error opening pipe!\n");
        return -1;
    }

    if(fgets(buf, 33, fp) == NULL) {
        //printf("No output\n");
        goto finally;
    }
    if(strncmp(buf, "*** The program has crashed ***\n", 33)) {
        //printf("Not the crash message\n");
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