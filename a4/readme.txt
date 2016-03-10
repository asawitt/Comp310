The only design decision I made was to have the free_blocks doubly-linked list have both free, and non-free blocks.
I added an inUse flag to determine whether any particular node has been allocated.

I also added a "printBlocks" method to the test file, which prints out the blocks and their contents/length, so that the progress of my_malloc can be 
followed by looking at the blocks, if so desired. I've commented the test file-output, so following along should be easy!

The program can be run by typing first "make" then "./test"


Thanks!
