#include "assignment2.h"
#include <errno.h>
int shm_fd;
void* ptr;
int justWokeUp = 0;
int clientId;
int duration;
void main(int argc, char* argv[]){
	//attach to shared memory
        shm_fd = shm_open(NAME, O_RDWR, 0666);
        ptr = mmap(0,sizeof(dataPacketStruct), PROT_WRITE, MAP_SHARED, shm_fd, 0);	
	dataPacketStruct packetForMemory;
	memcpy(&packetForMemory, ptr, sizeof(dataPacketStruct));
	clientId = atoi(argv[1]);
	duration = atoi(argv[2]);
	if (packetForMemory.size >= 25){ 
		
		printf("Client %d has a %d second job to print. Buffer full. Sleeps.\n", clientId, duration);
		justWokeUp = 1;	
	}
	else {
		 printf("Client %d has a %d second job to print. Puts request in Buffer[%d].\n", clientId, duration, packetForMemory.in);

	}
	sem_wait(&packetForMemory.semaphore1);
	if (justWokeUp) printf("Client %d wakes up, puts request in Buffer[%d]", clientId, duration); 
	if (ptr == (void*)-1){
		 printf("mmap error in client process: %s\n", strerror(errno));	
	}
	packetForMemory.buffer[packetForMemory.in].clientId = clientId;
	packetForMemory.buffer[packetForMemory.in].duration = duration;
	packetForMemory.size += 1;
	
	packetForMemory.in = (packetForMemory.in + 1) % 25;
	memcpy(ptr,&packetForMemory,sizeof(dataPacketStruct));		
}
