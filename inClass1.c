#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
void main( int argc, char*args[]){
	char line[64];
	
	while (1){
		print(":");
		getLine(&line,100, stdin);
		if (strlen(line) > 0){
			if (fork()==0){
				execl(args[0], args);
				
			}
		}
	}


}
	
