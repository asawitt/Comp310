#include "assignment2.h"
void *ptr;
int shm_fd;
int i;
int duration;
int previousId;
int printerId;
int previousOut;
void main(int argc, char* argv[]){
	if (argc >= 2){
		printerId = atoi(argv[1]);
	}
	else {
		printerId = 1;
	}
	dataPacketStruct packet;
	//Setup Shared Memory 
	shm_fd = shm_open(NAME, O_CREAT|O_RDWR, 0666);
        ftruncate(shm_fd,sizeof(dataPacketStruct));
        ptr = mmap(0,sizeof(dataPacketStruct), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
        //initialize sempahore and put it in shared memory
	sem_init(&packet.semaphore1, 1, 25);
	packet.in = 0;
	packet.size = 0;
	packet.out = 0;
	jobStruct *jobsBuffer = calloc(sizeof(jobStruct),25);
        memcpy(packet.buffer, jobsBuffer, sizeof(jobStruct) * 25);

	memcpy(ptr,&packet,sizeof(dataPacketStruct));	
	
	while (1) {
		memcpy(&packet,ptr,sizeof(dataPacketStruct));
		packet = (dataPacketStruct)packet;			
		if (packet.size != 0){  
			//printf("Packet size: %d\n", packet.size);
			//printf("Duration at packet.buffer[%d]: %d seconds \n", packet.out, packet.buffer[packet.out].duration);   
			printf("Printer %d has started printing job from Client: %d, from Buffer[%d]. The job will take %d seconds. \n", 
			printerId, packet.buffer[packet.out].clientId, packet.out, packet.buffer[packet.out].duration);
			duration  = packet.buffer[packet.out].duration;
			previousId = packet.buffer[packet.out].clientId; 
			previousOut = packet.out;
			packet.size -= 1;
			packet.out = (packet.out + 1) % 25;
			memcpy(ptr,&packet,sizeof(dataPacketStruct));

			sem_post(&packet.semaphore1);
			sleep(duration);	
			printf("Printer %d has finished its job from Buffer[%d], the job took %d seconds \n",
			printerId, previousOut, duration);
		}
		
			
        }
}

