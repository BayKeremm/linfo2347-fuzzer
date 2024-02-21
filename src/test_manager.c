#include "test_manager.h"
#include "tar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    new_tar_name field is a null termintated character string
*/
int test_name_field(char * tarName, char * extractor) {
    // TEST 1 write 0s 
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 1.1:       Testing with name field all NULLs\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name ="archive_name_null.tar";
    TAR_HEADER * header;
    create_tar_data(&header,"file.txt");
    char buff1[100];
    memset(buff1, '0', 98);
    buff1[99] ='\0';
    edit_header(&header, 0, buff1);
    if(save_tar_data(new_tar_name, header, "file.txt",1)<0){
        printf("Error saving tar file\n ");
        return -1;
    }
    test_archive(extractor,new_tar_name,1);


    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 1.2:       Testing with name field all EOFs\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name2 ="archive_name_eof.tar";
    TAR_HEADER * header2;
    create_tar_data(&header2,"file.txt");
    char buff2[100];
    memset(buff2, EOF, 99);
    edit_header(&header2, 0, buff2);
    if(save_tar_data(new_tar_name2, header2, "file.txt",1)<0){
        printf("Error saving tar file\n ");
        return -1;
    }
    test_archive(extractor,new_tar_name2,1);

    // TEST 3 write random numbers
    return 0;
}

int test_archive(char * extractor, char * tar_name,char delete_after){
    char buff[51];
    strncpy(buff,extractor, sizeof(buff) - strlen(tar_name) - 1);
    strncat(buff, " ", 1);
    strncat(buff, tar_name, sizeof(buff) - strlen(buff) - 1);

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
    if (rv!= 1 && delete_after)remove(tar_name);
    return rv;


}
/*
    int offset = 0;
    char new_tar_name[20];
    for(unsigned short int i = 128; i <= 255; i++) {
        snprintf(new_tar_name, sizeof(name), "name_test_%02x", i);
        char hex[3];
        snprintf(hex, sizeof(hex), "%02x", i);

        char buff[51];
        strncpy(buff,extractor, sizeof(buff) - strlen(new_tar_name) - 1);
        strncat(buff, " ", 1);
        strncat(buff, new_tar_name, sizeof(buff) - strlen(buff) - 1);
        //printf("%s ", buff);

        write_bytes(tarnew_tar_name,  offset,hex, name);
        int ret = test_archive(buff);
        if(ret==1){
            printf("Fucker crashed for the new_tar_name %s\n", name);
        }else{
            remove(new_tar_name);
        }

    }
    return 0;
*/