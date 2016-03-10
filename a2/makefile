all: server1 client1 
	
server1: server.c assignment2.h
	gcc -o server server.c -lrt -pthread

client1: client.c assignment2.h
	gcc -o client client.c -lrt -pthread

clean: 
	rm *.o

