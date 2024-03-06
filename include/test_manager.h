#ifndef _TEST_MANAGER_H_
#define _TEST_MANAGER_H_
#include "tar.h"

int test_field( char * extractor,int delete_after, int offset, int len);
int test_two_files( char * extractor ,int delete_after);
int test_archive(char * extractor, char * filename, char delete_after);
int test_specific( char * extractor ,int delete_after);

#endif