#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define NUM_DATA_POINTERS 50
#define MAXEXTENSION 4
#define MAXFILENAME 16
#define MAXOPENFILES 50 
#define MAX_BLOCKS 10000 
#define BLOCK_SIZE 1024
#define DISK "sfs_disk.disk"
#define MAX_FILES 200
#define MAX_INODES 1000
#define READ_WRITE 2
#define READ_ONLY 1
#define WRITE_ONLY 0


void mksfs(int fresh);
int sfs_getnextfilename(char *fname);
int sfs_getfilesize(const char* path);
int sfs_fopen(char *name);
int sfs_fclose(int fileID);
int sfs_fread(int fileID, char *buf, int length);
int sfs_fwrite(int fileID, const char *buf, int length);
int sfs_fseek(int fileID, int loc);
int sfs_remove(char *file);

typedef struct superBlock {
	unsigned int magic;
	unsigned int blockSize;
	unsigned int fileSystemSize;
	unsigned int rootDirectoryInode;
} SuperBlock;                                                                        
SuperBlock superBlock;

typedef struct iNode {
	short mode;
	int size; 
	int dataPointers[NUM_DATA_POINTERS];
	void *inodePtr;
} INode;


typedef struct fileDescriptorEntry {
	char name[50];
	unsigned int rw_ptr;//points to the byte in the block to be written to/read from
	unsigned int rw_ptr_block;//points to the block to be written to/read from
	int inodeIndex; 
} FileDescriptor;
typedef struct fdt {
	int size;
	FileDescriptor entry[MAXOPENFILES];
	int firstOpenIndex;
} FileDescriptorTable;
/*****************************************************/
FileDescriptorTable *fileDescriptorTable; 
/*****************************************************/

typedef struct directoryEntry {
	char name[(MAXFILENAME+MAXEXTENSION)+1];
	INode inode;
} DirectoryEntry;
typedef struct rd {
	int size;
	DirectoryEntry entry[MAX_INODES];
	int firstOpenIndex; 
} Directory;
/*****************************************************/
Directory rootDirectory;
/*****************************************************/
typedef struct fb {
	short freeBlocks[MAX_BLOCKS];
	short numberFree;
}FreeBlocks;
FreeBlocks freeBlocks;
int sfsGetNextIndex;
int init_fresh_disk(char *filename, int block_size, int num_blocks);
int init_disk(char *filename, int block_size, int num_blocks);
int read_blocks(int start_address, int nblocks, void *buffer);
int write_blocks(int start_address, int nblocks, void *buffer);
