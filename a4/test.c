#include "malloc.h"
void printBlocks(){
	int counter;
	counter = 0;
	temp = head;
	printf("CURRENT BLOCKS\n");
	printf("Head: size: %d, inUse: %d\n", head->length, head->inUse);
	while(temp!= NULL){
		printf("Block #: %d, size: %d, inUse: %d\n", counter, temp->length, temp->inUse);
		counter++;
		temp = temp->next;
	}
	printf("Tail: size: %d, inUse: %d\n", tail->length, tail->inUse);

}
int main(){
	void *ptr, *ptr2, *ptr3, *ptr4;
	printf("Start out in first-fit mode\n");
	my_mallopt(1);
	printf("About to allocate 100 bytes\n");
	ptr3 = my_malloc(100);	
	printBlocks();
	printf("About to allocate 300 bytes, then 100 bytes, then 200 bytes, then 100 bytes\n");	
	ptr = my_malloc(300);
	my_malloc(100);
	ptr2 = my_malloc(200);
	my_malloc(100);
	printBlocks();
	
	printf("Now demonstrate allocating > 128 bytes\n");
	printf("About to allocate 150 kilobytes\n");
	ptr4 = my_malloc(1024*150);
	printBlocks();
	

	printf("Now let's demonstrate the best-fit mode\n");
	my_mallopt(2);
	printf("Now we free the 300-byte block (block 1), and the 200 byte block (block 3).\n");
	my_free(ptr);
	my_free(ptr2);
	printBlocks();
	printf("Now we allocate 150 bytes, which will fit better into block 3 than 1. Note that we can't allocate 200 bytes because of mem_block size\n");
	printf("Also note that it doesn't fragment the block, because the leftover space is less than the 8 kb fragmentation limit\n");
	my_malloc(150);
	printBlocks();
	
	printf("Now lets demonstrate that freeing adjacent blocks will combine them. We now free block 0, adjacent to free-block 1\n");
	printf("Note that the size is now 432 = 300 + 100 + 32(size of mem_block))\n");
	my_free(ptr3);
	printBlocks();
	printf("Finally let's demonstrate that freeing the last > 128kb block will combine it with the previous free block, and remove both from memory\n");
	printf("Freeing tail (Block 5)\n");
	my_free(ptr4);
	printBlocks();
	my_mallinfo();
	return 0;
}

