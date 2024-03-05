#include "test_manager.h"
#include "tar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int test = 0;

int test_archive(char * extractor, char * tar_name, char delete_after){
    char buff[100];
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
    if(rv!=1 && delete_after){
        remove(tar_name);
    }
    return rv;
}

int test_field(char * extractor, int delete_after, int offset, int len){

    int vals[len];

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 0:       Testing with field all NULLs\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char new_tar_name0[50];
    test++;
    snprintf(new_tar_name0, sizeof(new_tar_name0), "success_test_%d.tar", test);

    for(int i=0;i<len;i++) vals[i]=0;

    tar(new_tar_name0,1,vals,offset,
    len, 1,1,
    1);

    test_archive(extractor, new_tar_name0, delete_after);


    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 1:       Testing with field all EOFs\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");

    char new_tar_name1[50];
    test++;
    snprintf(new_tar_name1, sizeof(new_tar_name1), "success_test_%d.tar", test);

    for(int i=0;i<len;i++) vals[i]=EOF;
    vals[len-1]='\0';
    tar(new_tar_name1,1,vals,offset,
     len,1,1,
    1);
    test_archive(extractor, new_tar_name1, delete_after);

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 2:       Testing with field non-ASCII value\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");

    char new_tar_name2[50];
    test++;
    snprintf(new_tar_name2, sizeof(new_tar_name2), "success_test_%d.tar", test);

    for(int i=0;i<len;i++) vals[i]=-5;
    tar(new_tar_name2,1,vals,offset,
     len,1,1,
    1);
    test_archive(extractor, new_tar_name2, delete_after);
    
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 3:       Testing with field with a random ascii number\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");

    char new_tar_name3[50];
    test++;
    snprintf(new_tar_name3, sizeof(new_tar_name3), "success_test_%d.tar", test);

    for(int i=0;i<len;i++) vals[i]=41;

    tar(new_tar_name3,1,vals,offset,
     len,1,1,
    1);
    test_archive(extractor, new_tar_name3, delete_after);

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 4:       Testing with field with special characters\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");

    char new_tar_name4[50];
    test++;
    snprintf(new_tar_name4, sizeof(new_tar_name4), "success_test_%d.tar", test);

    for(int i=0;i<len;i++) vals[i]='\t';

    tar(new_tar_name4,1,vals,offset,
     len,1,1,
    1);
    test_archive(extractor, new_tar_name4, delete_after);

    char new_tar_name5[50];
    test++;
    snprintf(new_tar_name5, sizeof(new_tar_name5), "success_test_%d.tar", test);

    for(int i=0;i<len;i++) vals[i]='\n';

    tar(new_tar_name5,1,vals,offset,
     len,1,1,
    1);
    test_archive(extractor, new_tar_name5, delete_after);

    char new_tar_name6[50];
    test++;
    snprintf(new_tar_name6, sizeof(new_tar_name6), "success_test_%d.tar", test);

    for(int i=0;i<len;i++) vals[i]='\\';

    tar(new_tar_name6,1,vals,offset,
     len,1,1,
    1);
    test_archive(extractor, new_tar_name6, delete_after);


    char new_tar_name7[50];
    test++;
    snprintf(new_tar_name7, sizeof(new_tar_name7), "success_test_%d.tar", test);

    for(int i=0;i<len;i++) vals[i]=' ';

    tar(new_tar_name7,1,vals,offset,
     len,1,1,
    1);
    test_archive(extractor, new_tar_name7, delete_after);

    return 0;
}

int test_two_files( char * extractor ,int delete_after){
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 1:       Testing extraction of 2 files without padding\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");

    char * new_tar_name ="success_multiple_without_padding.tar";
    tar(new_tar_name, 0, NULL, 0,
     0, 0,1,
     2);
    test_archive(extractor, new_tar_name, delete_after);

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 2:       Testing extraction of 2 files without ending blocks \n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");

    char * new_tar_name1 ="success_multiple_without_ending_blocks.tar";
    tar(new_tar_name1, 0, NULL, 0,
     0, 1,0,
     2);
    test_archive(extractor, new_tar_name1, delete_after);

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 3:       Testing extraction of 2 empty files \n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");

    char * new_tar_name2 ="success_multiple_empty_files.tar";
    tar(new_tar_name2, 0, NULL, 0,
     0, 1,1,
     2);
    test_archive(extractor, new_tar_name2, delete_after);

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 4:       Testing extraction of mismatched header and file data\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");

    char * new_tar_name3 ="success_multiple_mismatched.tar";
    tar(new_tar_name3, 0, NULL, 0,
     0, 0,0,
     2);
    test_archive(extractor, new_tar_name3, delete_after);

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 5:       Testing many files tar\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");

    for(int i=0;i<10;i++){
        char new_tar_name4[50];
        snprintf(new_tar_name4, sizeof(new_tar_name4), "success_test_many_%d.tar", i );

        tar(new_tar_name4, 0, NULL, 0,
        0, 0,0,
        2);
        for(int j=0;j<i;j++){

        }

        test_archive(extractor, new_tar_name4, delete_after);

    }



    return 0;
}
