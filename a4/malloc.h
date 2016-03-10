#include <stdio.h>
#include <unistd.h>

#define FIRSTFIT 1
#define BESTFIT 2
#define FRAGLIMIT 1024*8
extern char* my_malloc_error;


typedef struct b mem_block;
struct b {
        int length;
        int inUse;
        mem_block *previous;
        mem_block *next;
        void *memory;
};
mem_block *temp;
mem_block *head;
mem_block *tail;
mem_block *split_node;
mem_block *fit;

int split;
int bytes_to_allocate;
int closest_num_bytes;
void *mem_address;
int my_policy;
int bytesAllocated;
int freeSpace;
int largestContiguousFreeSpace;
int numberOfBlocks;
int timesFreed;
int timesAllocated;
int counter;
void* my_malloc();
void my_free();
void my_mallinfo();
void my_mallopt();
