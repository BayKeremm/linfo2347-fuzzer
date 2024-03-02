#include "test_manager.h"
#include "tar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NAME_FIELD_LEN 100
#define NAME_FIELD_OFFSET 0
#define UID_FIELD_LEN 8
#define UID_FIELD_OFFSET 108
#define GID_FIELD_LEN 8
#define GID_FIELD_OFFSET 116
#define SIZE_FIELD_LEN 12
#define SIZE_FIELD_OFFSET 124
#define MTIME_FIELD_LEN 12
#define MTIME_FIELD_OFFSET 136
#define CHKSUM_FIELD_LEN 8
#define CHKSUM_FIELD_OFFSET 148
#define TYPEFLAG_FIELD_LEN 1
#define TYPEFLAG_FIELD_OFFSET 156
#define LINKNAME_FIELD_LEN 100
#define LINKNAME_FIELD_OFFSET 157
#define MAGIC_FIELD_LEN 6
#define MAGIC_FIELD_OFFSET 257

#define VERSION_FIELD_LEN 2
#define VERSION_FIELD_OFFSET 263

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

int test_name_field( char * extractor, int del) {
    int vals[NAME_FIELD_LEN];
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 1.1:       Testing with name field all NULLs\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name ="archive_name_null.tar";
    for(int i=0;i<NAME_FIELD_LEN;i++) vals[i]=0;

    tar(new_tar_name,1,vals,NAME_FIELD_OFFSET,
     NAME_FIELD_LEN, 1,1,
    1,"../files/file.txt");

    test_archive(extractor, new_tar_name, del);


    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 1.2:       Testing with name field all EOFs\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name1 ="archive_name_eof.tar";
    for(int i=0;i<NAME_FIELD_LEN;i++) vals[i]=EOF;
    tar(new_tar_name1,1,vals,NAME_FIELD_OFFSET,
     NAME_FIELD_LEN,1,1,
    1,"../files/file.txt");
    test_archive(extractor, new_tar_name1, del);

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 1.3:       Testing with name field non-ASCII value\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name2 ="archive_name_non_ascii.tar";
    for(int i=0;i<NAME_FIELD_LEN;i++) vals[i]=257;
    tar(new_tar_name2,1,vals,NAME_FIELD_OFFSET,
     NAME_FIELD_LEN,1,1,
    1,"../files/file.txt");
    test_archive(extractor, new_tar_name2, del);
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
    1,"../files/file.txt");

    test_archive(extractor, new_tar_name, del);


    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 3.2:       Testing with uid field all EOFs\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name1 ="archive_uid_EOF.tar";

    for(int i=0;i<UID_FIELD_LEN;i++) vals[i]=EOF;

    tar(new_tar_name1,1,vals,UID_FIELD_OFFSET,
     UID_FIELD_LEN,1,1,
    1,"../files/file.txt");

    test_archive(extractor, new_tar_name1, del);

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 3.3:       Testing with uid field with a random number\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name2 ="archive_uid_random.tar";

    for(int i=0;i<UID_FIELD_LEN;i++) vals[i]=65;

    tar(new_tar_name2,1,vals,UID_FIELD_OFFSET,
     UID_FIELD_LEN,1,1,
    1,"../files/file.txt");
    test_archive(extractor, new_tar_name2, del);

    return 0;
}

int test_gid_field( char * extractor, int del) {
    int vals[GID_FIELD_LEN];
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 4.1:       Testing with gid field all NULLs\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name ="archive_gid_null.tar";

    for(int i=0;i<GID_FIELD_LEN;i++) vals[i]=0;

    tar(new_tar_name,1,vals,GID_FIELD_OFFSET,GID_FIELD_LEN, 1,1,1,"../files/file.txt");

    test_archive(extractor, new_tar_name, del);

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 4.2:       Testing with gid field all EOFs\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name1 ="archive_gid_EOF.tar";

    for(int i=0;i<GID_FIELD_LEN-1;i++) vals[i]=EOF;
    vals[GID_FIELD_LEN-1]=0;

    tar(new_tar_name1,1,vals,GID_FIELD_OFFSET,GID_FIELD_LEN,1,1,1,"../files/file.txt");

    test_archive(extractor, new_tar_name1, del);

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 4.3:       Testing with gid field with non octal values\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name2 ="archive_gid_noctal.tar";

    for(int i=0;i<GID_FIELD_LEN-1;i++) vals[i]='8';
    vals[GID_FIELD_LEN-1]=0;

    tar(new_tar_name2,1,vals,GID_FIELD_OFFSET,GID_FIELD_LEN,1,1,1,"../files/file.txt");

    test_archive(extractor, new_tar_name2, del);

    return 0;
}

int test_two_files( char * extractor ,int delete_after){
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step X.1:       Testing extraction of 2 files without padding\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name ="archive_multiple_without_padding.tar";
    tar(new_tar_name, 0, NULL, 0,
     0, 0,1,
     2, "../files/file.txt", "../files/assembly.pdf" );
    //printf("Command: %s",extractor);
    test_archive(extractor, new_tar_name, delete_after);

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step X.2:       Testing extraction of 2 files without ending blocks \n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name1 ="archive_multiple_without_ending_blocks.tar";
    tar(new_tar_name1, 0, NULL, 0,
     0, 1,0,
     2, "../files/assembly.pdf", "../files/file.txt" );
    //printf("Command: %s",extractor);
    test_archive(extractor, new_tar_name1, delete_after);

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step X.3:       Testing extraction of 2 empty files \n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name2 ="archive_multiple_empty_files.tar";
    tar(new_tar_name2, 0, NULL, 0,
     0, 1,1,
     2, "../files/empty0", "../files/empty1" );
    //printf("Command: %s",extractor);
    test_archive(extractor, new_tar_name2, delete_after);

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step X.4:       Testing extraction of mismatched header and file data\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name3 ="archive_multiple_mismatched.tar";
    tar(new_tar_name3, 0, NULL, 0,
     0, 0,0,
     1, "../files/empty0" );
    write_to_tar(new_tar_name3, "../files/file.txt",0, 0);
    test_archive(extractor, new_tar_name3, delete_after);
    return 0;
}

int test_size_field( char * extractor ,int delete_after){
    int vals[SIZE_FIELD_LEN];
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 5.1:       Testing with size field all NULLs\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name1 ="archive_size_null.tar";

    for(int i=0;i<SIZE_FIELD_LEN;i++) vals[i]=0;

    tar(new_tar_name1,1,vals,SIZE_FIELD_OFFSET,
    SIZE_FIELD_LEN, 1,1,
    1,"../files/file.txt");

    test_archive(extractor, new_tar_name1, delete_after);

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 5.2:       Testing with size field all EOFs\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name2 ="archive_size_eof.tar";

    for(int i=0;i<SIZE_FIELD_LEN;i++) vals[i]=EOF;

    tar(new_tar_name2,1,vals,SIZE_FIELD_OFFSET,
    SIZE_FIELD_LEN, 1,1,
    1,"../files/file.txt");

    test_archive(extractor, new_tar_name2, delete_after);

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 5.3:       Testing with size field random number\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name3 ="archive_size_random.tar";

    for(int i=0;i<SIZE_FIELD_LEN;i++) vals[i]=62;
    vals[SIZE_FIELD_LEN-1] = '\0';

    tar(new_tar_name3,1,vals,SIZE_FIELD_OFFSET,
    SIZE_FIELD_LEN, 1,1,
    1,"../files/file.txt");

    test_archive(extractor, new_tar_name3, delete_after);
    return 0;
}

int test_mtime_field( char * extractor ,int delete_after){
    int vals[MTIME_FIELD_LEN];
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 6.1:       Testing with mtime field all NULLs\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name1 ="archive_mtime_null.tar";

    for(int i=0;i<MTIME_FIELD_LEN;i++) vals[i]=0;

    tar(new_tar_name1,1,vals,MTIME_FIELD_OFFSET,
    MTIME_FIELD_LEN, 1,1,
    1,"../files/file.txt");

    test_archive(extractor, new_tar_name1, delete_after);

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 6.2:       Testing with mtime field all EOFs\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name2 ="archive_mtime_eof.tar";

    for(int i=0;i<MTIME_FIELD_LEN;i++) vals[i]=EOF;

    tar(new_tar_name2,1,vals,MTIME_FIELD_OFFSET,
    MTIME_FIELD_LEN, 1,1,
    1,"../files/file.txt");

    test_archive(extractor, new_tar_name2, delete_after);

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 6.3:       Testing with mtime field random number\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name3 ="archive_mtime_random.tar";

    for(int i=0;i<MTIME_FIELD_LEN;i++) vals[i]=255;
    vals[MTIME_FIELD_LEN-1] = '\0';

    tar(new_tar_name3,1,vals,MTIME_FIELD_OFFSET,
    MTIME_FIELD_LEN, 1,1,
    1,"../files/file.txt");

    test_archive(extractor, new_tar_name3, delete_after);

    return 0;
}

int test_chksum_field( char * extractor ,int delete_after){

    int vals[CHKSUM_FIELD_LEN];
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 7.1:       Testing with chksum field all NULLs\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name1 ="archive_chksum_null.tar";

    for(int i=0;i<CHKSUM_FIELD_LEN;i++) vals[i]=0;

    tar(new_tar_name1,1,vals,CHKSUM_FIELD_OFFSET,
    CHKSUM_FIELD_LEN, 1,1,
    1,"../files/file.txt");

    test_archive(extractor, new_tar_name1, delete_after);

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 7.2:       Testing with chksum field all EOFs\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name2 ="archive_chksum_eof.tar";

    for(int i=0;i<CHKSUM_FIELD_LEN;i++) vals[i]=EOF;

    tar(new_tar_name2,1,vals,CHKSUM_FIELD_OFFSET,
    CHKSUM_FIELD_LEN, 1,1,
    1,"../files/file.txt");

    test_archive(extractor, new_tar_name2, delete_after);

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 7.3:       Testing with chksum field random number\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name3 ="archive_chksum_random.tar";

    for(int i=0;i<CHKSUM_FIELD_LEN;i++) vals[i]=199;
    vals[CHKSUM_FIELD_LEN-1] = '\0';

    tar(new_tar_name3,1,vals,CHKSUM_FIELD_OFFSET,
    CHKSUM_FIELD_LEN, 1,1,
    1,"../files/file.txt");

    test_archive(extractor, new_tar_name3, delete_after);

    return 0;
}

int test_typeflag_field( char * extractor ,int delete_after){

    int vals[TYPEFLAG_FIELD_LEN];
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 8.1:       Testing with typeflag field all NULLs\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name1 ="archive_typeflag_null.tar";

    for(int i=0;i<TYPEFLAG_FIELD_LEN;i++) vals[i]=0;

    tar(new_tar_name1,1,vals,TYPEFLAG_FIELD_OFFSET,
    TYPEFLAG_FIELD_LEN, 1,1,
    1,"../files/file.txt");

    test_archive(extractor, new_tar_name1, delete_after);

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 8.2:       Testing with typeflag field all EOFs\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name2 ="archive_typeflag_eof.tar";

    for(int i=0;i<TYPEFLAG_FIELD_LEN;i++) vals[i]=EOF;

    tar(new_tar_name2,1,vals,TYPEFLAG_FIELD_OFFSET,
    TYPEFLAG_FIELD_LEN, 1,1,
    1,"../files/file.txt");

    test_archive(extractor, new_tar_name2, delete_after);

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 8.3:       Testing with typeflag field 'x' \n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name3 ="archive_typeflag_x.tar";

    for(int i=0;i<TYPEFLAG_FIELD_LEN;i++) vals[i]='x';

    tar(new_tar_name3,1,vals,TYPEFLAG_FIELD_OFFSET,
    TYPEFLAG_FIELD_LEN, 1,1,
    1,"../files/file.txt");

    test_archive(extractor, new_tar_name3, delete_after);

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 8.4:       Testing with typeflag field 'k'\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name4 ="archive_typeflag_k.tar";

    for(int i=0;i<TYPEFLAG_FIELD_LEN;i++) vals[i]='k';

    tar(new_tar_name4,1,vals,TYPEFLAG_FIELD_OFFSET,
    TYPEFLAG_FIELD_LEN, 1,1,
    1,"../files/file.txt");

    test_archive(extractor, new_tar_name4, delete_after);

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 8.5:       Testing with typeflag field '5'\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name5 ="archive_typeflag_5.tar";

    for(int i=0;i<TYPEFLAG_FIELD_LEN;i++) vals[i]='5';

    tar(new_tar_name5,1,vals,TYPEFLAG_FIELD_OFFSET,
    TYPEFLAG_FIELD_LEN, 1,1,
    1,"../files/file.txt");

    test_archive(extractor, new_tar_name5, delete_after);

    return 0;
}

int test_linkname_field( char * extractor, int del) {
    int vals[LINKNAME_FIELD_LEN];
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 9.1:       Testing with linkname field all NULLs\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name ="archive_linkname_null.tar";
    for(int i=0;i<LINKNAME_FIELD_LEN;i++) vals[i]=0;

    tar(new_tar_name,1,vals,LINKNAME_FIELD_OFFSET,
     LINKNAME_FIELD_LEN, 1,1,
    1,"../files/file.txt");

    test_archive(extractor, new_tar_name, del);


    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 9.2:       Testing with linkname field all EOFs\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name1 ="archive_linkname_eof.tar";
    for(int i=0;i<LINKNAME_FIELD_LEN;i++) vals[i]=EOF;
    tar(new_tar_name1,1,vals,LINKNAME_FIELD_OFFSET,
     LINKNAME_FIELD_LEN,1,1,
    1,"../files/file.txt");
    test_archive(extractor, new_tar_name1, del);

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 9.3:       Testing with linkname field non-ASCII value\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name2 ="archive_linkname_non_ascii.tar";
    for(int i=0;i<LINKNAME_FIELD_LEN;i++) vals[i]=201;
    vals[LINKNAME_FIELD_LEN-1] = '\0';
    tar(new_tar_name2,1,vals,LINKNAME_FIELD_OFFSET,
     LINKNAME_FIELD_LEN,1,1,
    1,"../files/file.txt");
    test_archive(extractor, new_tar_name2, del);
    return 0;
}

int test_magic_field( char * extractor, int del) {
    int vals[MAGIC_FIELD_LEN];
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 10.1:       Testing with magic field all NULLs\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name ="archive_magic_null.tar";
    for(int i=0;i<MAGIC_FIELD_LEN;i++) vals[i]=0;

    tar(new_tar_name,1,vals,MAGIC_FIELD_OFFSET,
     MAGIC_FIELD_LEN, 1,1,
    1,"../files/file.txt");

    test_archive(extractor, new_tar_name, del);


    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 10.2:       Testing with magic field all EOFs\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name1 ="archive_magic_eof.tar";
    for(int i=0;i<MAGIC_FIELD_LEN;i++) vals[i]=EOF;
    tar(new_tar_name1,1,vals,MAGIC_FIELD_OFFSET,
     MAGIC_FIELD_LEN,1,1,
    1,"../files/file.txt");
    test_archive(extractor, new_tar_name1, del);

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 10.3:       Testing with magic field non-ASCII value\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name2 ="archive_magic_non_ascii.tar";
    for(int i=0;i<MAGIC_FIELD_LEN;i++) vals[i]=202;
    vals[MAGIC_FIELD_LEN-1] = '\0';
    tar(new_tar_name2,1,vals,MAGIC_FIELD_OFFSET,
     MAGIC_FIELD_LEN,1,1,
    1,"../files/file.txt");
    test_archive(extractor, new_tar_name2, del);
    return 0;
}

int test_version_field( char * extractor, int del) {
    int vals[VERSION_FIELD_LEN];
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 10.1:       Testing with version field all NULLs\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name ="archive_version_null.tar";
    for(int i=0;i<VERSION_FIELD_LEN;i++) vals[i]=0;

    tar(new_tar_name,1,vals,VERSION_FIELD_OFFSET,
     VERSION_FIELD_LEN, 1,1,
    1,"../files/file.txt");

    test_archive(extractor, new_tar_name, del);


    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 10.2:       Testing with version field all EOFs\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name1 ="archive_version_eof.tar";
    for(int i=0;i<VERSION_FIELD_LEN;i++) vals[i]=EOF;
    tar(new_tar_name1,1,vals,VERSION_FIELD_OFFSET,
     VERSION_FIELD_LEN,1,1,
    1,"../files/file.txt");
    test_archive(extractor, new_tar_name1, del);

    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    printf("    Step 10.3:       Testing with version field non-ASCII value\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    char * new_tar_name2 ="archive_version_non_ascii.tar";
    for(int i=0;i<VERSION_FIELD_LEN;i++) vals[i]=231;
    vals[VERSION_FIELD_LEN-1] = '\0';
    tar(new_tar_name2,1,vals,VERSION_FIELD_OFFSET,
     VERSION_FIELD_LEN,1,1,
    1,"../files/file.txt");
    test_archive(extractor, new_tar_name2, del);
    return 0;
}
//int test_valid_tar(char *cmd, int del){
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    //printf("    Step 0.1:       Testing the valid archive.tar\n");
    //printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
    //char * f ="archive.tar"; 
    //TAR_HEADER*header;
    //create_tar_header(&header,"../files/assembly.pdf");
    //if(save_tar_data("archive.tar", header, "../files/assembly.pdf", 1,1)<0){
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