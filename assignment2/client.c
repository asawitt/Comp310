#include "assignment2.h"
#include <errno.h>
int shm_fd;

void main(){
	void* ptr;
	char* message_0 = "Fourth message tho";
        //attach to shared memory
	shm_fd = shm_open(NAME, O_RDWR, 0666);
	ptr = mmap(0,MEMORY_SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (ptr == (void*)-1){
		 printf("mmap error in client process: %s\n", strerror(errno));	
	}
		
	sprintf(ptr, "%s",message_0);
	ptr+=strlen(message_0);
}
