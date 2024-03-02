#ifndef _TEST_MANAGER_H_
#define _TEST_MANAGER_H_
#include "tar.h"

//int test_valid_tar( char * extractor ,int delete_after);
int test_name_field( char * extractor,int delete_after);
int test_uid_field( char * extractor ,int delete_after);
int test_gid_field( char * extractor ,int delete_after);
int test_two_files( char * extractor ,int delete_after);
int test_size_field( char * extractor ,int delete_after);
int test_mtime_field( char * extractor ,int delete_after);
int test_chksum_field( char * extractor ,int delete_after);
int test_typeflag_field( char * extractor ,int delete_after);
int test_linkname_field( char * extractor ,int delete_after);
int test_magic_field( char * extractor ,int delete_after);
int test_version_field( char * extractor ,int delete_after);

int test_archive(char * extractor, char * filename, char delete_after);

#endif