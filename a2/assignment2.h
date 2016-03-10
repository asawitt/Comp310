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
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>

//Name of the shared memory object
const char* NAME = "COMP310Assignment2";
//structure for job
typedef struct job {
	int clientId;
	int duration;
} jobStruct; 
//structure for everything
typedef struct dataPacket {
	jobStruct buffer[25];
	int size;
	int in;
	int out;
	sem_t semaphore1; //overflows
} dataPacketStruct;
#endif
