#include "malloc.h"
void *my_malloc(int size){
	bytes_to_allocate = size + sizeof(mem_block);
	//If size = 0 error
	if (size == 0){
		return (void*)-1;
	}
	timesAllocated++;	
	
	//Checks if any blocks have been created yet
	if (head != NULL){
		closest_num_bytes = -1;
		fit = NULL;
		temp = head;
		//Loops through the list of Nodes (modified from assignment to include all nodes, and a free-flag.
		while(temp != NULL){
			//Checks if best-fit or closest-fit		
			if (my_policy == FIRSTFIT){
				//Checks if it fits
				if (bytes_to_allocate <= temp->length && temp->inUse == 0){
					fit = temp;
					break;				
				}
			}
			//policy = best_fit
			else if (my_policy==BESTFIT){
				//Checks if it fits better than previous fit
				if (temp->inUse == 0 && temp->length - bytes_to_allocate >= 0){
					if (closest_num_bytes == -1||closest_num_bytes > temp->length - bytes_to_allocate){
						closest_num_bytes = temp->length - bytes_to_allocate;
						fit = temp; 
					}
				}
			}
			else {
				//Error
			}
			
			temp = temp->next;
		}
		//Allocates the block and splits if it's not the exact right size
		//Splits
		if (fit != NULL){
			if (fit->length - bytes_to_allocate > FRAGLIMIT){
				
				split_node = fit + bytes_to_allocate;
				split_node->previous = fit;
				split_node->length = fit->length - size;
				split_node->inUse = 0;
				split_node->memory = fit->memory + bytes_to_allocate+sizeof(mem_block);
				if (fit->next == NULL){
					tail = split_node;
				}else {
					split_node->next = fit->next;
				}
				fit->next = split_node;
				fit->length = size;
			}	
			//Doesn't have to split
			fit->inUse = 1;
			return fit->memory;
		}
	}
	//Head is null, or no blocks can fit size
	if (bytes_to_allocate <= 128*1024){
		
		mem_address = sbrk(128*1024);	
	}
	else {
		mem_address = sbrk(bytes_to_allocate);
	}
	//Allocates 128kbytes per call (or more if size requested is larger);
	if (mem_address != (void*)-1){
		if (head != NULL){
			temp = mem_address;
			temp->previous = tail;
			temp->length = size;
			temp->inUse = 1;
			temp->memory = mem_address + sizeof(mem_block);
			temp->next = NULL;
			tail->next = temp;
			tail = temp;
			if (bytes_to_allocate<128*1024){
				split_node = mem_address+bytes_to_allocate;
				temp->next = split_node;
				split_node->previous = tail;
				split_node->length = 128*1024 - bytes_to_allocate;
				split_node->inUse = 0;
				split_node->next = NULL;
				split_node->memory = mem_address + bytes_to_allocate + sizeof(mem_block);
				tail = split_node; 
			}	
			return tail->memory;
		}
		//Head is null
		else {
			head = mem_address;
			head->previous = NULL;
			head->length = size;
			head->inUse = 1;
			head->memory = mem_address + sizeof(mem_block);
			head->next = NULL;
			tail = head;
			if (bytes_to_allocate < 128*1024){
				split_node = mem_address + bytes_to_allocate;
				split_node->previous = head;
				split_node->length = 128*1024 - bytes_to_allocate;
				split_node->inUse = 0;
				split_node->next = NULL;
				split_node->memory = mem_address + bytes_to_allocate + sizeof(mem_block);
				head->next = split_node;
				tail = split_node;
			} 
			return head->memory;
		}
	}
	//Shouldn't ever return here
	return (void *)-1;
}
void my_free(void *ptr){
	timesFreed++;
	temp = head;
	int tailLength = -tail->length - sizeof(mem_block);
	//Loops through all nodes in list
	while (temp != NULL){
		//If it finds one with the the ptr, free it
		if (temp->memory == ptr){
			temp->inUse = 0;
			//combines with the surrounding blocks if they're also not in use
			//if the next block isn't in use
			if (temp->next != NULL && temp->next->inUse == 0){
				//checks if next is the tail and sets tail to temp if so
				if (temp->next == tail){
					tail = temp;
				}
				//Changes nexts
				temp->length += temp->next->length + sizeof(mem_block);
				if (temp->next->next != NULL){
					temp->next = temp->next->next;
				}else temp -> next = NULL;
			}
			//If the previous block isn't in use
			if (temp->previous != NULL && temp->previous->inUse == 0){
				
				if (tail == temp){
					tail = temp->previous;
				}	
				temp->previous->next = temp->next;
				temp->previous->length += temp->length + sizeof(mem_block);
			}
			//Changes memory ptr if over 128 kb is free in the top block
			if (tail != NULL && tail->inUse == 0 && tail->length >= 128*1024){
				if (tail->previous != NULL){
					tail = tail->previous;
					tail->next = NULL;
					
				}else {
					tail = NULL;
					head = NULL;
				}
				sbrk(-tailLength);
			}				
			break;
		}
		temp = temp->next;
	}

}
void my_mallopt(int policy){
	if (policy == FIRSTFIT || policy == BESTFIT) my_policy = policy;
	//else error
}
void my_mallinfo(){
	char str[1024];
	int allocatedSpace, freeBlocks, usedBlocks;
	largestContiguousFreeSpace = 0;
	freeSpace = 0;
	numberOfBlocks = 0;
	allocatedSpace = 0;
	usedBlocks = 0;
	freeBlocks = 0;
	temp = head;
	while (temp != NULL){
		numberOfBlocks++;
		if(temp->inUse == 0){
			if (temp->length > largestContiguousFreeSpace){
				largestContiguousFreeSpace = temp->length;
			}
			freeSpace += temp->length;
			freeBlocks++;
		}
		else {
			allocatedSpace += temp->length;
			usedBlocks++;
		}
		temp = temp->next;	
	}
	sprintf(str, "~~~~~MEMORY INFORMATION~~~~~");
	puts(str);
	sprintf(str, "Number of blocks: %d", numberOfBlocks);
	puts(str);
	sprintf(str, "Number of blocks used: %d", usedBlocks);
	puts(str);
	sprintf(str, "Number of blocks free: %d", freeBlocks);
	puts(str);
	sprintf(str, "Allocated space: %d bytes", allocatedSpace);
	puts(str);
	sprintf(str, "Free Space: %d bytes", freeSpace);
	puts(str);
	sprintf(str, "Largest contiguous free space: %d bytes", largestContiguousFreeSpace);
	puts(str);
	sprintf(str, "Times allocated %d", timesAllocated);
	puts(str);
	sprintf(str, "Times freed: %d", timesFreed);
	puts(str);
}
