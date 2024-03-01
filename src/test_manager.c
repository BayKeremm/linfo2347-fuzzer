#include "test_manager.h"
#include "tar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NAME_FIELD_LEN 100
#define NAME_FIELD_OFFSET 0
#define UID_FIELD_LEN 8
#define UID_FIELD_OFFSET 108

int test_two_files( char * cmd ,int delete_after){
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step X.1:       Testing extraction of 2 files \n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name ="archive_multiple.tar";

}
int test_name_field( char * extractor, int del) {
    int vals[NAME_FIELD_LEN];
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 1.1:       Testing with name field all NULLs\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name ="archive_name_null.tar";
    for(int i=0;i<NAME_FIELD_LEN;i++) vals[i]=0;
    tar(new_tar_name,1,vals,NAME_FIELD_OFFSET,
     NAME_FIELD_LEN, 1,1,
    1,"./files/file.txt");
    test_archive(extractor, new_tar_name, del);


    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 1.2:       Testing with name field all EOFs\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name1 ="archive_name_eof.tar";
    for(int i=0;i<NAME_FIELD_LEN;i++) vals[i]=EOF;
    tar(new_tar_name1,1,vals,NAME_FIELD_OFFSET,
     NAME_FIELD_LEN,1,1,
    1,"./files/file.txt");
    test_archive(extractor, new_tar_name1, del);

    return 0;
}
int test_uid_field( char * extractor, int del) {
    // writing 62 to gid (position 117) crashes for gid
    // writing non NULL to 116 crashes for uid, so eofs and random number is causedf from the same crash

    int vals[UID_FIELD_LEN];
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 3.1:       Testing with uid field all NULLs\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name ="archive_uid_null.tar";
    for(int i=0;i<UID_FIELD_LEN;i++) vals[i]=0;
    tar(new_tar_name,1,vals,UID_FIELD_OFFSET,
     UID_FIELD_LEN, 1,1,
    1,"./files/file.txt");
    test_archive(extractor, new_tar_name, del);


    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 3.2:       Testing with uid field all EOFs\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name1 ="archive_uid_EOF.tar";

    for(int i=0;i<UID_FIELD_LEN;i++) vals[i]=EOF;

    tar(new_tar_name1,1,vals,UID_FIELD_OFFSET,
     UID_FIELD_LEN,1,1,
    1,"./files/file.txt");
    test_archive(extractor, new_tar_name1, del);

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 3.3:       Testing with uid field with a random number\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name2 ="archive_uid_random.tar";

    for(int i=0;i<UID_FIELD_LEN;i++) vals[i]=65;

    tar(new_tar_name2,1,vals,UID_FIELD_OFFSET,
     UID_FIELD_LEN,1,1,
    1,"./files/file.txt");
    test_archive(extractor, new_tar_name2, del);

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
//int test_valid_tar(char *cmd, int del){
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 0.1:       Testing the valid archive.tar\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * f ="archive.tar"; 
    //TAR_HEADER*header;
    //create_tar_header(&header,"./files/assembly.pdf");
    //if(save_tar_data("archive.tar", header, "./files/assembly.pdf", 1,1)<0){
        //printf("Error saving tar data");
        //exit(-1);
    //}
    //int ret = test_archive(cmd,f,0);
    //if(ret==0){
        //printf("    - Tar file is extracted correctly\n");
    //}else if(ret == -1){
        //printf("    - Tar file did not extract correctly\n");
        //return -1;
    //}
//return 0;
//}