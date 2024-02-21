#ifndef _TEST_MANAGER_H_
#define _TEST_MANAGER_H_
#include "fuzzer.h"
#include "tar.h"

int test_name_field(char * tarName, char * cmd);

int test_archive(char * name);

#endif