all: mymalloc.o test.o 
	gcc -o test mymalloc.o test.o
mymalloc.o: mymalloc.c
	gcc -Wall -c mymalloc.c
test.o: test.c
	gcc -Wall -c test.c
clean:
	rm *.o test


