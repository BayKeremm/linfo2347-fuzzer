#ifndef _FUZZER_H_
#define _FUZZER_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "helper.h"

int write_bytes(char * fileName, unsigned int offset ,char * byteSequence,char * newFileName);

#endif