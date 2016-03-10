#include "sfs_api.h"


void fillWithZeroes(){
	bzero(&superBlock, sizeof(SuperBlock));
	bzero(&rootDirectory, sizeof(Directory));
	bzero(&freeBlocks, sizeof(FreeBlocks));	
}
void setupSuperblock(SuperBlock s){
	s.magic = 18432;
	s.blockSize = BLOCK_SIZE;
	s.fileSystemSize = MAX_BLOCKS * BLOCK_SIZE;
	s.rootDirectoryInode = 0;
}
void mksfs(int fresh) {
	if (fresh == 1){
		//Makes a new disk if fresh flag is true
		printf("Initializing disk\n");
		init_fresh_disk(DISK,BLOCK_SIZE,MAX_BLOCKS);	
		fillWithZeroes();	
		//calloc for fdt
		fileDescriptorTable = (FileDescriptorTable*)calloc(1,sizeof(FileDescriptorTable));	
		//Write superblock 
		printf("Writing superblock\n");
		//setupSuperblock(superBlock);
		printf("Setting up superblock\n");
		write_blocks(0,1,&superBlock);
		//Write root directory
		bzero(&rootDirectory,sizeof(Directory));
		printf("Writing root directory\n");
		write_blocks(1,1, &rootDirectory);
		//Write free blocks
		printf("Writing free blocks\n");
		freeBlocks.numberFree = MAX_BLOCKS;
		freeBlocks.freeBlocks[0] = 1; //superblock
		freeBlocks.numberFree--;
		freeBlocks.freeBlocks[1] = 1; //root directory
		freeBlocks.numberFree--;
		freeBlocks.freeBlocks[MAX_BLOCKS-1] = 1; //free_blocks;		
		freeBlocks.numberFree--;
		//Write free blocks to disk
		printf("writing free blocks to disk \n");
		write_blocks(MAX_BLOCKS - 1, 1, &freeBlocks);

		
	}else{
		printf("Loading disk\n");
		//Pull disk data into memory
		init_disk(DISK, BLOCK_SIZE, MAX_BLOCKS);
	}
	//Initializes variable for sfs_getnextfilename;
	sfsGetNextIndex = 0;
	
	//Initializes variable for fileDescriptor table
	(*fileDescriptorTable).firstOpenIndex = 0;
	(*fileDescriptorTable).size = 0;
	//Initializes variable for rootDirectory
	rootDirectory.firstOpenIndex = 2;
	rootDirectory.size = 3;
	return;
}

int sfs_getnextfilename(char *fname) {
	if (sfsGetNextIndex >= rootDirectory.size){
		return 0;
	}
	strcpy(fname,rootDirectory.entry[sfsGetNextIndex].name);
	sfsGetNextIndex++;
	//returns index of the next directoryEntry in the directory
	return sfsGetNextIndex;
}


int sfs_getfilesize(const char* path) {
	//Checks if file exists
	int i;
	for (i = 0;i<rootDirectory.size;i++){
		if(strcmp(rootDirectory.entry[i].name, path) == 0){
			return rootDirectory.entry[i].inode.size;
		}
	}
	printf("sfs_getfilesize(%s) fail. Is %s a valid filename?\n", path, path);
	return -1;
}

int sfs_fopen(char *name) {
	int i, j,k,returnValue;
	//Checks if more files can be simultanesiously opened
	if ((*fileDescriptorTable).size >= MAXOPENFILES) {
		printf("sfs_fopen failed: error -1, max # of simultaneously open files reached\n");
		return -1;
	}
	//Checks if the name is too long
	if (strlen(name) > MAXFILENAME + MAXEXTENSION) {
		printf("Filename is too long, not creating file\n");
		return -1;
	}
	//Checks if the file exists

	for (i = 0;i<=MAX_INODES;i++){
                //File exists
		if(!strcmp(rootDirectory.entry[i].name, name)){
			//Checks if file is already open
			for (k = 0;k<MAXOPENFILES;k++){
				if (!strcmp((*fileDescriptorTable).entry[k].name, name)){
					printf("Cannot open file: %s, file already open.\n", name);
					return -1;
				}
			}
			printf("Opening %s\n", name);
			//Saves the value of the first open index that the new descriptor entry will be saved to
			returnValue = (*fileDescriptorTable).firstOpenIndex; 
			//Sets the INode for the File Descriptor Table
			(*fileDescriptorTable).entry[(*fileDescriptorTable).firstOpenIndex].inodeIndex  = i;
			//Sets the name of the fileDescriptorEntry to the name of the file
			strcpy((*fileDescriptorTable).entry[(*fileDescriptorTable).firstOpenIndex].name,name);
			//Sets read write pointer to the end of the file
			(*fileDescriptorTable).entry[(*fileDescriptorTable).firstOpenIndex].rw_ptr = rootDirectory.entry[i].inode.size; 
			//Sets firstOpenIndex in (*fileDescriptorTable).to next open Index
			for (j=0;j<MAXOPENFILES;j++){
				if (strlen((*fileDescriptorTable).entry[j].name) ==  0){
					(*fileDescriptorTable).firstOpenIndex = j;
					(*fileDescriptorTable).size++;
					return returnValue;
				}
			}
			//Last file open-able based on MAXOPENFILES
			(*fileDescriptorTable).firstOpenIndex = -1;
			(*fileDescriptorTable).size++;
			return returnValue;
			
                }
	}
	//If file doesn't exist yet
	
	//Check if rootDirectory has any free inodes;
        if (rootDirectory.size >= MAX_INODES) {
              	printf("Can't create a new file, max # of INodes reached.\n");
		return -1;
        }
	rootDirectory.size++;
	returnValue = (*fileDescriptorTable).firstOpenIndex;
	printf("Creating new file with name: %s at index: %d in the File Descriptor Table\n", name, returnValue);
	//Make a new INode for the file
	INode *inode = (INode*)calloc(sizeof(INode), 1);
	(*inode).mode = READ_WRITE;
	(*inode).size = 0;
	//Add INode to rootDirectory
	rootDirectory.entry[rootDirectory.firstOpenIndex].inode = *inode; 	
	//Set name of file in rootDirectory 
	memcpy(rootDirectory.entry[rootDirectory.firstOpenIndex].name,name,strlen(name));
	rootDirectory.entry[rootDirectory.firstOpenIndex].name[strlen(name)] = '\0';
	 //Change the name in the File Descriptor table
        memcpy((*fileDescriptorTable).entry[(*fileDescriptorTable).firstOpenIndex].name, name,strlen(name));
	(*fileDescriptorTable).entry[(*fileDescriptorTable).firstOpenIndex].name[strlen(name)] = '\0';
	//Change the inode index in the fileDescriptor Table
        (*fileDescriptorTable).entry[(*fileDescriptorTable).firstOpenIndex].inodeIndex = rootDirectory.firstOpenIndex;
	

	//change firstOpenIndex in root directory;
	for (j=0;j<MAX_INODES;j++){
        	if (strlen(rootDirectory.entry[j].name)<=0){
                	rootDirectory.firstOpenIndex = j;
			break;
                }
        }

	//Flush rootDirectory information to the disk
        write_blocks(1,1,&rootDirectory);
	//Sets firstOpenIndex in (*fileDescriptorTable).to next open Index

        for (j=0;j<MAXOPENFILES;j++){
        	if (strlen((*fileDescriptorTable).entry[j].name) ==  0){
        		(*fileDescriptorTable).firstOpenIndex = j;
			(*fileDescriptorTable).size++;
        		return returnValue;
        	}
        }	
	(*fileDescriptorTable).size ++;
	(*fileDescriptorTable).firstOpenIndex = -1;
	return returnValue;	
}

int sfs_fclose(int fileID){
	int j;

	//If file is not open or doesn't exist returns -1;
	if (strlen((*fileDescriptorTable).entry[fileID].name)== 0) {
		printf("No open file at index %d in the File Descriptor Table\n", fileID);
		return -1;
	}
	//File is currently open
	//Remove from File Descriptor Table
	printf("Closing file at FileDescriptorTable %d: %s\n", fileID, (*fileDescriptorTable).entry[fileID].name);
	strcpy((*fileDescriptorTable).entry[fileID].name, "");
	(*fileDescriptorTable).entry[fileID].inodeIndex = -1;
	(*fileDescriptorTable).size--;
	//Reset File Descriptor's next available index to the first available index
	//Sets firstOpenIndex in (*fileDescriptorTable).to next open Index
        for (j=0;j<MAXOPENFILES;j++){
                if (strlen((*fileDescriptorTable).entry[j].name) ==  0){
                        (*fileDescriptorTable).firstOpenIndex = j;
                	return 0;
		}
        }
	
	//Returns 0 if successful (but not here);
	return 0;
}

int sfs_fread(int fileID, char *buf, int length){
	int sizeToRead, i, sizeRead, rwptrblock, rwptr;
	int dataptrs[NUM_DATA_POINTERS];
	char *tempBuf = calloc(BLOCK_SIZE,1);
	//Makes sure file is open
	 if (strlen((*fileDescriptorTable).entry[fileID].name) == 0){
                printf("Error, can't read from a closed file. No file open at index %d in the File Descriptor Table\n", fileID);
                return -1;
        }
	printf("%d Byte(s) requested from fileID: %d\n",length,fileID);
	rwptr = (*fileDescriptorTable).entry[fileID].rw_ptr;
	rwptrblock = (*fileDescriptorTable).entry[fileID].rw_ptr_block; 

	//Sets sizeToRead
        sizeToRead = length;
	//makes sure length isn't longer than the size of the file (test file sometimes asks for longer, so no error.
	if (length > rootDirectory.entry[(*fileDescriptorTable).entry[fileID].inodeIndex].inode.size) {
		printf("Requested %d bytes from file %d: %s, but the file is only %d bytes long. Copying first %d bytes\n", 
		length, fileID,(*fileDescriptorTable).entry[fileID].name, 
		rootDirectory.entry[(*fileDescriptorTable).entry[fileID].inodeIndex].inode.size,
		rootDirectory.entry[(*fileDescriptorTable).entry[fileID].inodeIndex].inode.size);
		sizeToRead = rootDirectory.entry[(*fileDescriptorTable).entry[fileID].inodeIndex].inode.size;
	}
	sizeRead = 0;
	printf("Reading %d bytes from FileDescriptorTable %d: %s\n", sizeToRead, fileID, (*fileDescriptorTable).entry[fileID].name);
		
	//makes local copy of dataptrs for less-typing
	memcpy(dataptrs,rootDirectory.entry[(*fileDescriptorTable).entry[fileID].inodeIndex].inode.dataPointers,sizeof(int)*NUM_DATA_POINTERS);
	//Loops through each dataPointer in the associated INode and read the data in associated block up until size is reached
	
	while (sizeToRead > 0){
		tempBuf=calloc(1,BLOCK_SIZE);	
		if (rwptr >= BLOCK_SIZE) {
			rwptrblock += 1;
			rwptr = 0;
		}
		//reads the whole block into the temp buf, dealing with rwptr after being read
		read_blocks(rootDirectory.entry[(*fileDescriptorTable).entry[fileID].inodeIndex].inode.dataPointers[rwptrblock],1,tempBuf);
		tempBuf += rwptr;
		if (sizeToRead > BLOCK_SIZE - rwptr){
			memcpy(buf+sizeRead,tempBuf,BLOCK_SIZE-rwptr);
			sizeToRead -= (BLOCK_SIZE - rwptr);
			sizeRead += (BLOCK_SIZE - rwptr);
			rwptr+=BLOCK_SIZE-rwptr;
		}
		else {
			memcpy(buf+sizeRead,tempBuf,sizeToRead);
			sizeRead += sizeToRead;
			rwptr += sizeToRead;
			sizeToRead -= sizeToRead;
		}

	}	
	(*fileDescriptorTable).entry[fileID].rw_ptr_block = rwptrblock;
	sfs_fseek(fileID, rwptr);	
	return sizeRead;
	
	
}

int sfs_fwrite(int fileID, const char *buf, int length){
	if (strlen((*fileDescriptorTable).entry[fileID].name) == 0){
		printf("Error, No file open at index %d in the File Descriptor Table\n", fileID);
		return -1;
	} 
	printf("Writing %d bytes to file at ID: %d\n", length, fileID);
	int bytesWritten,additionalPointersNeeded, usedDataPointers, i, num_blocks, j, blockToWriteTo, k, dataPointerFree, numBytesToWrite, remainingSpaceInCurrentBlock, buf_bytes_left;
	void* tempBuf = calloc(BLOCK_SIZE, 1);
	usedDataPointers = 0;
	bytesWritten = 0;	
	buf_bytes_left = length;
	//Calculates the remaining space in the current block
	remainingSpaceInCurrentBlock = BLOCK_SIZE - (*fileDescriptorTable).entry[fileID].rw_ptr;
	//Calculates # of blocks necessary for given length
	num_blocks = (length/BLOCK_SIZE + (length%BLOCK_SIZE != 0)); //Calculates number of blocks, avoids double/int errors and math lib
	if (length%BLOCK_SIZE > remainingSpaceInCurrentBlock) num_blocks++; //Adds an extra one if the space in the first block isn't enough
	//since the math library must be compiled with -lm
	//calculates the number of pointers being used by the root directory
	for (i = 0;i<NUM_DATA_POINTERS;i++){
		if (rootDirectory.entry[(*fileDescriptorTable).entry[fileID].inodeIndex].inode.dataPointers[i]!= 0){
			usedDataPointers++;
		}
		else break;
	}
 	
	//Calculates the additional number if blocks necessary, given the current rw_ptr_block
	additionalPointersNeeded = (*fileDescriptorTable).entry[fileID].rw_ptr_block + num_blocks - usedDataPointers;	
	//Checks if the necessary num_blocks is more than available
	//printf("num_blocks = %d, freeBlocks.numberFree = %d\n", num_blocks, freeBlocks.numberFree);
	if (freeBlocks.numberFree < additionalPointersNeeded){
		printf("Write will take %d additional blocks, but there are only %d left. Exiting\n", additionalPointersNeeded, freeBlocks.numberFree);
		return -1;
	}
	//Checks if there are enough datapointers left in the rootDirectory's INode
	if (NUM_DATA_POINTERS - usedDataPointers - additionalPointersNeeded < 0){
                //Creates and links to a new INode
		INode *nInode = (INode*)calloc(1,sizeof(INode));
		(*nInode).mode = READ_WRITE;
		(*nInode).size = 0;
		rootDirectory.entry[(*fileDescriptorTable).entry[fileID].inodeIndex].inode.inodePtr = nInode;
		return -1;
	}
	//Writes to the fileID one block at a time
	for (i=1;i<= num_blocks;i++){
		//finds the next open block if the rwptr is at the end of the block
		if ((*fileDescriptorTable).entry[fileID].rw_ptr == BLOCK_SIZE || (*fileDescriptorTable).entry[fileID].rw_ptr == 0){
			//find the first free block to write to
			for (j=0;j<MAX_BLOCKS;j++){
				if (freeBlocks.freeBlocks[j] == 0){
					blockToWriteTo = j;
					//sets the datapointer at rwblockptr to the block which is being written to
					rootDirectory.entry[(*fileDescriptorTable).entry[fileID].inodeIndex].inode.dataPointers[(*fileDescriptorTable).entry[fileID].rw_ptr_block] = blockToWriteTo;
					(*fileDescriptorTable).entry[fileID].rw_ptr = 0;
					break;
				}
			}
		}else blockToWriteTo =  rootDirectory.entry[(*fileDescriptorTable).entry[fileID].inodeIndex].inode.dataPointers[(*fileDescriptorTable).entry[fileID].rw_ptr_block];
		
		//sets up a temporaryBuffer that contains the data from the block to be written to
		read_blocks(blockToWriteTo,1,tempBuf); 
		//We can write BLOCK_SIZE - rwptr bytes to the BLOCK
		if(buf_bytes_left > (BLOCK_SIZE - (*fileDescriptorTable).entry[fileID].rw_ptr)){
			numBytesToWrite = BLOCK_SIZE - (*fileDescriptorTable).entry[fileID].rw_ptr;
			 (*fileDescriptorTable).entry[fileID].rw_ptr_block += 1;
		}
		else numBytesToWrite = buf_bytes_left;
		
		//After writing numBytesToWrite there will be numBytesToWrite less bytes to write
		buf_bytes_left -= numBytesToWrite;
		//copy the numBytesToWrite to the tempBuf at offset rw_ptr
		memcpy(tempBuf + (*fileDescriptorTable).entry[fileID].rw_ptr,buf,numBytesToWrite);
		//write tempBuf to the block
		write_blocks(blockToWriteTo,1,tempBuf);
		//Erase the first numBytesToWrite from the buf
		//buf += numBytesToWrite; 
		//Add the number of bytes written to the rw_ptr
                (*fileDescriptorTable).entry[fileID].rw_ptr += numBytesToWrite;

		(*fileDescriptorTable).entry[fileID].name[20] = '\0';
		

		//Calculates the amt. written
		bytesWritten += numBytesToWrite;
		//Fills in the freeBlocks bitmap and changes size
		freeBlocks.freeBlocks[blockToWriteTo] = 1;
		freeBlocks.numberFree --;
	}
	//Flush everything to the disk
	write_blocks(1,1,&rootDirectory);
	write_blocks(MAX_BLOCKS-1,1,&freeBlocks);	
	//Change this incase rw_ptr wasn't at the end
	rootDirectory.entry[(*fileDescriptorTable).entry[fileID].inodeIndex].inode.size += bytesWritten;
	return bytesWritten;
}

int sfs_fseek(int fileID, int loc){
	int location;
	int block_location;
	//Checks if the file is open
        if (strlen((*fileDescriptorTable).entry[fileID].name) == 0){
                printf("Error, can't seek in closed file. No file open at index %d in the File Descriptor Table\n", fileID);
                return -1;
        }
	else {
		//My rw_ptr works as a function of blocks and location within the block, so this might look a bit strange.
		location = loc % BLOCK_SIZE; 
		block_location = loc/BLOCK_SIZE;
		(*fileDescriptorTable).entry[fileID].rw_ptr = location;
		(*fileDescriptorTable).entry[fileID].rw_ptr_block = block_location;
	}
	//returns 0 if successful...?
	return 0;
}

int sfs_remove(char *file) {
	int i, k, j, l;

	for (i=0;i<=MAX_INODES;i++){
                //File exists
                if(!strcmp(rootDirectory.entry[i].name, file)){
                        //Checks if file is open
                        for (k = 0;k<MAXOPENFILES;k++){
                                if (!strcmp((*fileDescriptorTable).entry[k].name, file)){
                                       printf("File is open, closing....\n"); 
					sfs_fclose(k);
					break;
                                }
                        }
			//Free and zero the blocks that the dataptrs attached to the file's INode point to
			for (j = 0;j<NUM_DATA_POINTERS;j++){
				if (rootDirectory.entry[i].inode.dataPointers[j] == 0) break;
				//Frees the block in the freeBlock Array and the dataptr in the rootDirectory inode for the file
				freeBlocks.freeBlocks[rootDirectory.entry[i].inode.dataPointers[j]] = 0;
				freeBlocks.numberFree++;
				rootDirectory.entry[i].inode.dataPointers[j] = 0;
			}		
			rootDirectory.entry[i].inode.size = 0;
			rootDirectory.entry[i].name[0] = '\0';
			write_blocks(1,1,&rootDirectory);
			write_blocks(MAX_BLOCKS-1, 1, &freeBlocks);
			
			//return 0 if successful
			return 0;
		}
	}
	printf("File doesn't exist, can't delete a file that doesn't exist\n");
	return -1; 
}
