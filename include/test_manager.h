#ifndef _TEST_MANAGER_H_
#define _TEST_MANAGER_H_
#include "tar.h"

/**
    Test the archive by applying modifications to its fields
    @param extractor: The extractor to use
    @param tar_name: The name of the tar file to test
    @param delete_after: if the tar file should be deleted after the test if not successfuld
    @param offset: The offset of the field to modify
    @param len: The length of the field to modify
    @returns: 0 if the test was did not produce any error
    
*/
int test_field( char * extractor,int delete_after, int offset, int len);

/**
    Test an archive which contains two files
    @param extractor: The extractor to use
    @param delete_after: if the tar file should be deleted after the test if not successful
    @returns: 0 if the test was did not produce any error
    
*/
int test_two_files( char * extractor ,int delete_after);

/**
    Test the archive with a specific file
    @param extractor: The extractor to use
    @param delete_after: if the tar file should be deleted after the test if not successfuld
    @returns: 0 if the test was did not produce any error
    
*/
int test_archive(char * extractor, char * filename, char delete_after);

/**
    Test the archive with specific modifications
    @param extractor: The extractor to use
    @param delete_after: if the tar file should be deleted after the test if not successfuld
    @returns: 0 if the test was did not produce any error
    
*/
int test_specific( char * extractor ,int delete_after);

#endif