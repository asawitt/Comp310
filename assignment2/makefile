all: server1 client1 
	
server1: server.c
	gcc -o server server.c -lrt

client1: client.c
	gcc -o client client.c -lrt

clean: 
	rm *.o

