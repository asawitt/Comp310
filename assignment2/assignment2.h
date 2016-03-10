#ifndef assignment2Header
#define assignment2Header
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/unistd.h>
//Name of the shared memory object
const char* NAME = "COMP310Assignment2";
//memory size
const int MEMORY_SIZE = 100000;
#endif
