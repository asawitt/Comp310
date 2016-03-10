#include "assignment2.h"
void *ptr;
int shm_fd;
const char nullChar = '\0';

void setupSharedMem(void *memptr){
        shm_fd = shm_open(NAME, O_CREAT|O_RDWR, 0666);
        ftruncate(shm_fd,MEMORY_SIZE);
        memptr = mmap(0,MEMORY_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
}

void main(){
	char* message;
        //Setup Shared Memory
        
	shm_fd = shm_open(NAME, O_CREAT|O_RDWR, 0666);
        ftruncate(shm_fd,MEMORY_SIZE);
        ptr = mmap(0,MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	//setupSharedMem(ptr);
        //initialize sempahore and put it in shared memory
	
        while (1) {
		
                //test read from shared mem
		if (((char*)ptr)[0] != '\0'){    
			message = (char*) ptr;	
			printf("Message: %s\n", message);
			sprintf(ptr,"%c", nullChar);
		}
		sleep(4);
		printf("Still running\n");	
                //take a job
                //print a message (duration of job, job ID, source of job
                //sleep for job duration        
        }
}

