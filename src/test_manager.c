#include "test_manager.h"
#include "tar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TEXT_FILE "./files/file.txt"
#define PDF_FILE "./files/assembly.pdf"
#define EMPTY_FILE_0 "./files/empty0"
#define EMPTY_FILE_1 "./files/empty0"

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

static int test = 0;
int test_field(char * extractor, int delete_after, int offset, int len){

    int vals[len];

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 0:       Testing with field all EOFs\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char new_tar_name0[50];
    test++;
    snprintf(new_tar_name0, sizeof(new_tar_name0), "success_test_%d.tar", test);

    for(int i=0;i<len;i++) vals[i]=0;

    tar(new_tar_name0,1,vals,offset,
     len, 1,1,
    1,TEXT_FILE);

    test_archive(extractor, new_tar_name0, delete_after);


    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 1:       Testing with field all EOFs\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");

    char new_tar_name1[50];
    test++;
    snprintf(new_tar_name1, sizeof(new_tar_name1), "success_test_%d.tar", test);

    for(int i=0;i<len;i++) vals[i]=EOF;
    tar(new_tar_name1,1,vals,offset,
     len,1,1,
    1,TEXT_FILE);
    test_archive(extractor, new_tar_name1, delete_after);

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 2:       Testing with field non-ASCII value\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");

    char new_tar_name2[50];
    test++;
    snprintf(new_tar_name2, sizeof(new_tar_name2), "success_test_%d.tar", test);

    for(int i=0;i<len;i++) vals[i]=257;
    tar(new_tar_name2,1,vals,offset,
     len,1,1,
    1,TEXT_FILE);
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
    1,TEXT_FILE);
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
    1,TEXT_FILE);
    test_archive(extractor, new_tar_name4, delete_after);

    char new_tar_name5[50];
    test++;
    snprintf(new_tar_name5, sizeof(new_tar_name5), "success_test_%d.tar", test);

    for(int i=0;i<len;i++) vals[i]='\n';

    tar(new_tar_name5,1,vals,offset,
     len,1,1,
    1,TEXT_FILE);
    test_archive(extractor, new_tar_name5, delete_after);

    char new_tar_name6[50];
    test++;
    snprintf(new_tar_name6, sizeof(new_tar_name6), "success_test_%d.tar", test);

    for(int i=0;i<len;i++) vals[i]='\\';

    tar(new_tar_name6,1,vals,offset,
     len,1,1,
    1,TEXT_FILE);
    test_archive(extractor, new_tar_name6, delete_after);


    return 0;
}

int test_two_files( char * extractor ,int delete_after){
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 1:       Testing extraction of 2 files without padding\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name ="success_multiple_without_padding.tar";
    tar(new_tar_name, 0, NULL, 0,
     0, 0,1,
     2, TEXT_FILE, PDF_FILE );
    //printf("Command: %s",extractor);
    test_archive(extractor, new_tar_name, delete_after);

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 2:       Testing extraction of 2 files without ending blocks \n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name1 ="success_multiple_without_ending_blocks.tar";
    tar(new_tar_name1, 0, NULL, 0,
     0, 1,0,
     2, PDF_FILE, TEXT_FILE );
    //printf("Command: %s",extractor);
    test_archive(extractor, new_tar_name1, delete_after);

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 3:       Testing extraction of 2 empty files \n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name2 ="success_multiple_empty_files.tar";
    tar(new_tar_name2, 0, NULL, 0,
     0, 1,1,
     2, EMPTY_FILE_0, EMPTY_FILE_1 );
    //printf("Command: %s",extractor);
    test_archive(extractor, new_tar_name2, delete_after);

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 4:       Testing extraction of mismatched header and file data\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name3 ="success_multiple_mismatched.tar";
    tar(new_tar_name3, 0, NULL, 0,
     0, 0,0,
     1, EMPTY_FILE_0 );
    write_to_tar(new_tar_name3, TEXT_FILE,0, 0);
    test_archive(extractor, new_tar_name3, delete_after);
    return 0;
}

//int test_name_field( char * extractor, int del) {
    //int vals[NAME_FIELD_LEN];
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 1.1:       Testing with name field all NULLs\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name ="success_name_null.tar";
    //for(int i=0;i<NAME_FIELD_LEN;i++) vals[i]=0;

    //tar(new_tar_name,1,vals,NAME_FIELD_OFFSET,
     //NAME_FIELD_LEN, 1,1,
    //1,TEXT_FILE);

    //test_archive(extractor, new_tar_name, del);


    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 1.2:       Testing with name field all EOFs\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name1 ="success_name_eof.tar";
    //for(int i=0;i<NAME_FIELD_LEN;i++) vals[i]=EOF;
    //tar(new_tar_name1,1,vals,NAME_FIELD_OFFSET,
     //NAME_FIELD_LEN,1,1,
    //1,TEXT_FILE);
    //test_archive(extractor, new_tar_name1, del);

    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 1.3:       Testing with name field non-ASCII value\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name2 ="success_name_non_ascii.tar";
    //for(int i=0;i<NAME_FIELD_LEN;i++) vals[i]=257;
    //tar(new_tar_name2,1,vals,NAME_FIELD_OFFSET,
     //NAME_FIELD_LEN,1,1,
    //1,TEXT_FILE);
    //test_archive(extractor, new_tar_name2, del);

    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 1.3:       Testing with name field \\ \n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name3 ="success_name_weird.tar";
    //for(int i=0;i<NAME_FIELD_LEN;i++) vals[i]='\n';
    //tar(new_tar_name3,1,vals,NAME_FIELD_OFFSET,
     //NAME_FIELD_LEN,1,1,
    //1,TEXT_FILE);
    //test_archive(extractor, new_tar_name3, del);
    //return 0;
//}
//int test_uid_field( char * extractor, int del) {
    //// writing 62 to gid (position 117) crashes for gid
    //// writing non NULL to 116 crashes for uid, so eofs and random number is causedf from the same crash
    //int vals[UID_FIELD_LEN];
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 3.1:       Testing with uid field all NULLs\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name ="success_uid_null.tar";

    //for(int i=0;i<UID_FIELD_LEN;i++) vals[i]=0;

    //tar(new_tar_name,1,vals,UID_FIELD_OFFSET,
     //UID_FIELD_LEN, 1,1,
    //1,TEXT_FILE);

    //test_archive(extractor, new_tar_name, del);


    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 3.2:       Testing with uid field all EOFs\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name1 ="success_uid_EOF.tar";

    //for(int i=0;i<UID_FIELD_LEN;i++) vals[i]=EOF;

    //tar(new_tar_name1,1,vals,UID_FIELD_OFFSET,
     //UID_FIELD_LEN,1,1,
    //1,TEXT_FILE);

    //test_archive(extractor, new_tar_name1, del);

    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 3.3:       Testing with uid field with a random number\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name2 ="success_uid_random.tar";

    //for(int i=0;i<UID_FIELD_LEN;i++) vals[i]=65;

    //tar(new_tar_name2,1,vals,UID_FIELD_OFFSET,
     //UID_FIELD_LEN,1,1,
    //1,TEXT_FILE);
    //test_archive(extractor, new_tar_name2, del);

    //return 0;
//}

//int test_gid_field( char * extractor, int del) {
    //int vals[GID_FIELD_LEN];
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 4.1:       Testing with gid field all NULLs\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name ="success_gid_null.tar";

    //for(int i=0;i<GID_FIELD_LEN;i++) vals[i]=0;

    //tar(new_tar_name,1,vals,GID_FIELD_OFFSET,GID_FIELD_LEN, 1,1,1,TEXT_FILE);

    //test_archive(extractor, new_tar_name, del);

    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 4.2:       Testing with gid field all EOFs\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name1 ="success_gid_EOF.tar";

    //for(int i=0;i<GID_FIELD_LEN-1;i++) vals[i]=EOF;
    //vals[GID_FIELD_LEN-1]=0;

    //tar(new_tar_name1,1,vals,GID_FIELD_OFFSET,GID_FIELD_LEN,1,1,1,TEXT_FILE);

    //test_archive(extractor, new_tar_name1, del);

    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 4.3:       Testing with gid field with non octal values\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name2 ="success_gid_noctal.tar";

    //for(int i=0;i<GID_FIELD_LEN-1;i++) vals[i]='8';
    //vals[GID_FIELD_LEN-1]=0;

    //tar(new_tar_name2,1,vals,GID_FIELD_OFFSET,GID_FIELD_LEN,1,1,1,TEXT_FILE);

    //test_archive(extractor, new_tar_name2, del);

    //return 0;
//}


//int test_size_field( char * extractor ,int delete_after){
    //int vals[SIZE_FIELD_LEN];
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 5.1:       Testing with size field all NULLs\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name1 ="success_size_null.tar";

    //for(int i=0;i<SIZE_FIELD_LEN;i++) vals[i]=0;

    //tar(new_tar_name1,1,vals,SIZE_FIELD_OFFSET,
    //SIZE_FIELD_LEN, 1,1,
    //1,TEXT_FILE);

    //test_archive(extractor, new_tar_name1, delete_after);

    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 5.2:       Testing with size field all EOFs\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name2 ="success_size_eof.tar";

    //for(int i=0;i<SIZE_FIELD_LEN;i++) vals[i]=EOF;

    //tar(new_tar_name2,1,vals,SIZE_FIELD_OFFSET,
    //SIZE_FIELD_LEN, 1,1,
    //1,TEXT_FILE);

    //test_archive(extractor, new_tar_name2, delete_after);

    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 5.3:       Testing with size field random number\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name3 ="success_size_random.tar";

    //for(int i=0;i<SIZE_FIELD_LEN;i++) vals[i]=62;
    //vals[SIZE_FIELD_LEN-1] = '\0';

    //tar(new_tar_name3,1,vals,SIZE_FIELD_OFFSET,
    //SIZE_FIELD_LEN, 1,1,
    //1,TEXT_FILE);

    //test_archive(extractor, new_tar_name3, delete_after);
    //return 0;
//}

//int test_mtime_field( char * extractor ,int delete_after){
    //int vals[MTIME_FIELD_LEN];
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 6.1:       Testing with mtime field all NULLs\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name1 ="success_mtime_null.tar";

    //for(int i=0;i<MTIME_FIELD_LEN;i++) vals[i]=0;

    //tar(new_tar_name1,1,vals,MTIME_FIELD_OFFSET,
    //MTIME_FIELD_LEN, 1,1,
    //1,TEXT_FILE);

    //test_archive(extractor, new_tar_name1, delete_after);

    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 6.2:       Testing with mtime field all EOFs\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name2 ="success_mtime_eof.tar";

    //for(int i=0;i<MTIME_FIELD_LEN;i++) vals[i]=EOF;

    //tar(new_tar_name2,1,vals,MTIME_FIELD_OFFSET,
    //MTIME_FIELD_LEN, 1,1,
    //1,TEXT_FILE);

    //test_archive(extractor, new_tar_name2, delete_after);

    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 6.3:       Testing with mtime field random number\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name3 ="success_mtime_random.tar";

    //for(int i=0;i<MTIME_FIELD_LEN;i++) vals[i]=255;
    //vals[MTIME_FIELD_LEN-1] = '\0';

    //tar(new_tar_name3,1,vals,MTIME_FIELD_OFFSET,
    //MTIME_FIELD_LEN, 1,1,
    //1,TEXT_FILE);

    //test_archive(extractor, new_tar_name3, delete_after);

    //return 0;
//}

//int test_chksum_field( char * extractor ,int delete_after){

    //int vals[CHKSUM_FIELD_LEN];
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 7.1:       Testing with chksum field all NULLs\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name1 ="success_chksum_null.tar";

    //for(int i=0;i<CHKSUM_FIELD_LEN;i++) vals[i]=0;

    //tar(new_tar_name1,1,vals,CHKSUM_FIELD_OFFSET,
    //CHKSUM_FIELD_LEN, 1,1,
    //1,TEXT_FILE);

    //test_archive(extractor, new_tar_name1, delete_after);

    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 7.2:       Testing with chksum field all EOFs\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name2 ="success_chksum_eof.tar";

    //for(int i=0;i<CHKSUM_FIELD_LEN;i++) vals[i]=EOF;

    //tar(new_tar_name2,1,vals,CHKSUM_FIELD_OFFSET,
    //CHKSUM_FIELD_LEN, 1,1,
    //1,TEXT_FILE);

    //test_archive(extractor, new_tar_name2, delete_after);

    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 7.3:       Testing with chksum field random number\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name3 ="success_chksum_random.tar";

    //for(int i=0;i<CHKSUM_FIELD_LEN;i++) vals[i]=199;
    //vals[CHKSUM_FIELD_LEN-1] = '\0';

    //tar(new_tar_name3,1,vals,CHKSUM_FIELD_OFFSET,
    //CHKSUM_FIELD_LEN, 1,1,
    //1,TEXT_FILE);

    //test_archive(extractor, new_tar_name3, delete_after);

    //return 0;
//}

//int test_typeflag_field( char * extractor ,int delete_after){

    //int vals[TYPEFLAG_FIELD_LEN];
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 8.1:       Testing with typeflag field all NULLs\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name1 ="success_typeflag_null.tar";

    //for(int i=0;i<TYPEFLAG_FIELD_LEN;i++) vals[i]=0;

    //tar(new_tar_name1,1,vals,TYPEFLAG_FIELD_OFFSET,
    //TYPEFLAG_FIELD_LEN, 1,1,
    //1,TEXT_FILE);

    //test_archive(extractor, new_tar_name1, delete_after);

    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 8.2:       Testing with typeflag field all EOFs\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name2 ="success_typeflag_eof.tar";

    //for(int i=0;i<TYPEFLAG_FIELD_LEN;i++) vals[i]=EOF;

    //tar(new_tar_name2,1,vals,TYPEFLAG_FIELD_OFFSET,
    //TYPEFLAG_FIELD_LEN, 1,1,
    //1,TEXT_FILE);

    //test_archive(extractor, new_tar_name2, delete_after);

    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 8.3:       Testing with typeflag field 1 \n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name3 ="success_typeflag_x.tar";

    //for(int i=0;i<TYPEFLAG_FIELD_LEN;i++) vals[i]=1;

    //tar(new_tar_name3,1,vals,TYPEFLAG_FIELD_OFFSET,
    //TYPEFLAG_FIELD_LEN, 1,1,
    //1,TEXT_FILE);

    //test_archive(extractor, new_tar_name3, delete_after);

    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 8.4:       Testing with typeflag field -123\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name4 ="success_typeflag_k.tar";

    //for(int i=0;i<TYPEFLAG_FIELD_LEN;i++) vals[i]=-123;

    //tar(new_tar_name4,1,vals,TYPEFLAG_FIELD_OFFSET,
    //TYPEFLAG_FIELD_LEN, 1,1,
    //1,TEXT_FILE);

    //test_archive(extractor, new_tar_name4, delete_after);

    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 8.5:       Testing with typeflag field \\ \n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name5 ="success_typeflag_5.tar";

    //for(int i=0;i<TYPEFLAG_FIELD_LEN;i++) vals[i]='\\';

    //tar(new_tar_name5,1,vals,TYPEFLAG_FIELD_OFFSET,
    //TYPEFLAG_FIELD_LEN, 1,1,
    //1,TEXT_FILE);

    //test_archive(extractor, new_tar_name5, delete_after);

    //return 0;
//}

//int test_linkname_field( char * extractor, int del) {
    //int vals[LINKNAME_FIELD_LEN];
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 9.1:       Testing with linkname field all NULLs\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name ="success_linkname_null.tar";
    //for(int i=0;i<LINKNAME_FIELD_LEN;i++) vals[i]=0;

    //tar(new_tar_name,1,vals,LINKNAME_FIELD_OFFSET,
     //LINKNAME_FIELD_LEN, 1,1,
    //1,TEXT_FILE);

    //test_archive(extractor, new_tar_name, del);


    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 9.2:       Testing with linkname field all EOFs\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name1 ="success_linkname_eof.tar";
    //for(int i=0;i<LINKNAME_FIELD_LEN;i++) vals[i]=EOF;
    //tar(new_tar_name1,1,vals,LINKNAME_FIELD_OFFSET,
     //LINKNAME_FIELD_LEN,1,1,
    //1,TEXT_FILE);
    //test_archive(extractor, new_tar_name1, del);

    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 9.3:       Testing with linkname field non-ASCII value\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name2 ="success_linkname_non_ascii.tar";
    //for(int i=0;i<LINKNAME_FIELD_LEN;i++) vals[i]=201;
    //vals[LINKNAME_FIELD_LEN-1] = '\0';
    //tar(new_tar_name2,1,vals,LINKNAME_FIELD_OFFSET,
     //LINKNAME_FIELD_LEN,1,1,
    //1,TEXT_FILE);
    //test_archive(extractor, new_tar_name2, del);
    //return 0;
//}

//int test_magic_field( char * extractor, int del) {
    //int vals[MAGIC_FIELD_LEN];
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 10.1:       Testing with magic field all NULLs\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name ="success_magic_null.tar";
    //for(int i=0;i<MAGIC_FIELD_LEN;i++) vals[i]=0;

    //tar(new_tar_name,1,vals,MAGIC_FIELD_OFFSET,
     //MAGIC_FIELD_LEN, 1,1,
    //1,TEXT_FILE);

    //test_archive(extractor, new_tar_name, del);


    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 10.2:       Testing with magic field all EOFs\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name1 ="success_magic_eof.tar";
    //for(int i=0;i<MAGIC_FIELD_LEN;i++) vals[i]=EOF;
    //tar(new_tar_name1,1,vals,MAGIC_FIELD_OFFSET,
     //MAGIC_FIELD_LEN,1,1,
    //1,TEXT_FILE);
    //test_archive(extractor, new_tar_name1, del);

    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 10.3:       Testing with magic field non-ASCII value\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name2 ="success_magic_non_ascii.tar";
    //for(int i=0;i<MAGIC_FIELD_LEN;i++) vals[i]=202;
    //vals[MAGIC_FIELD_LEN-1] = '\0';
    //tar(new_tar_name2,1,vals,MAGIC_FIELD_OFFSET,
     //MAGIC_FIELD_LEN,1,1,
    //1,TEXT_FILE);
    //test_archive(extractor, new_tar_name2, del);
    //return 0;
//}

//int test_version_field( char * extractor, int del) {
    //int vals[VERSION_FIELD_LEN];
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 11.1:       Testing with version field all NULLs\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name ="success_version_null.tar";
    //for(int i=0;i<VERSION_FIELD_LEN;i++) vals[i]=0;

    //tar(new_tar_name,1,vals,VERSION_FIELD_OFFSET,
     //VERSION_FIELD_LEN, 1,1,
    //1,TEXT_FILE);

    //test_archive(extractor, new_tar_name, del);


    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 11.2:       Testing with version field all EOFs\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name1 ="success_version_eof.tar";
    //for(int i=0;i<VERSION_FIELD_LEN;i++) vals[i]=EOF;
    //tar(new_tar_name1,1,vals,VERSION_FIELD_OFFSET,
     //VERSION_FIELD_LEN,1,1,
    //1,TEXT_FILE);
    //test_archive(extractor, new_tar_name1, del);

    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 11.3:       Testing with version field non-ASCII value\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name2 ="success_version_non_ascii.tar";
    //for(int i=0;i<VERSION_FIELD_LEN;i++) vals[i]=231;
    //vals[VERSION_FIELD_LEN-1] = '\0';
    //tar(new_tar_name2,1,vals,VERSION_FIELD_OFFSET,
     //VERSION_FIELD_LEN,1,1,
    //1,TEXT_FILE);
    //test_archive(extractor, new_tar_name2, del);
    //return 0;
//}

//int test_uname_field( char * extractor, int del) {
    //int vals[UNAME_FIELD_LEN];
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 12.1:       Testing with uname field all NULLs\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name ="success_uname_null.tar";
    //for(int i=0;i<UNAME_FIELD_LEN;i++) vals[i]=0;

    //tar(new_tar_name,1,vals,UNAME_FIELD_OFFSET,
     //UNAME_FIELD_LEN, 1,1,
    //1,TEXT_FILE);

    //test_archive(extractor, new_tar_name, del);


    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 12.2:       Testing with uname field all EOFs\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name1 ="success_uname_eof.tar";
    //for(int i=0;i<UNAME_FIELD_LEN;i++) vals[i]=EOF;
    //tar(new_tar_name1,1,vals,UNAME_FIELD_OFFSET,
     //UNAME_FIELD_LEN,1,1,
    //1,TEXT_FILE);
    //test_archive(extractor, new_tar_name1, del);

    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 12.3:       Testing with uname field non-ASCII value\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name2 ="success_uname_non_ascii.tar";
    //for(int i=0;i<UNAME_FIELD_LEN;i++) vals[i]=201;
    //vals[UNAME_FIELD_LEN-1] = '\0';
    //tar(new_tar_name2,1,vals,UNAME_FIELD_OFFSET,
     //UNAME_FIELD_LEN,1,1,
    //1,TEXT_FILE);
    //test_archive(extractor, new_tar_name2, del);
    //return 0;
//}

//int test_gname_field( char * extractor, int del) {
    //int vals[GNAME_FIELD_LEN];
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 13.1:       Testing with gname field all NULLs\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name ="success_gname_null.tar";
    //for(int i=0;i<GNAME_FIELD_LEN;i++) vals[i]=0;

    //tar(new_tar_name,1,vals,GNAME_FIELD_OFFSET,
     //GNAME_FIELD_LEN, 1,1,
    //1,TEXT_FILE);

    //test_archive(extractor, new_tar_name, del);


    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 13.2:       Testing with gname field all EOFs\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name1 ="success_gname_eof.tar";
    //for(int i=0;i<GNAME_FIELD_LEN;i++) vals[i]=EOF;
    //tar(new_tar_name1,1,vals,GNAME_FIELD_OFFSET,
     //GNAME_FIELD_LEN,1,1,
    //1,TEXT_FILE);
    //test_archive(extractor, new_tar_name1, del);

    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 13.3:       Testing with gname field non-ASCII value\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * new_tar_name2 ="success_gname_non_ascii.tar";
    //for(int i=0;i<GNAME_FIELD_LEN;i++) vals[i]=201;
    //vals[GNAME_FIELD_LEN-1] = '\0';
    //tar(new_tar_name2,1,vals,GNAME_FIELD_OFFSET,
     //GNAME_FIELD_LEN,1,1,
    //1,TEXT_FILE);
    //test_archive(extractor, new_tar_name2, del);
    //return 0;
//}
////int test_valid_tar(char *cmd, int del){
    ////printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    ////printf("    Step 0.1:       Testing the valid archive.tar\n");
    ////printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    ////char * f ="success.tar"; 
    ////TAR_HEADER*header;
    ////create_tar_header(&header,PDF_FILE);
    ////if(save_tar_data("success.tar", header, PDF_FILE, 1,1)<0){
        ////printf("Error saving tar data");
        ////exit(-1);
    ////}
    ////int ret = test_archive(cmd,f,0);
    ////if(ret==0){
        ////printf("    - Tar file is extracted correctly\n");
    ////}else if(ret == -1){
        ////printf("    - Tar file did not extract correctly\n");
        ////return -1;
    ////}
////return 0;
////}