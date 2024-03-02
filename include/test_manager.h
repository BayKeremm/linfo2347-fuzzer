#ifndef _TEST_MANAGER_H_
#define _TEST_MANAGER_H_
#include "tar.h"

//int test_valid_tar( char * cmd ,int delete_after);
int test_name_field( char * cmd,int delete_after);
int test_uid_field( char * cmd ,int delete_after);
int test_gid_field( char * cmd ,int delete_after);
int test_two_files( char * cmd ,int delete_after);
int test_size_field( char * cmd ,int delete_after);

int test_archive(char * cmd, char * filename, char delete_after);

#endif