#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
void printHistory();
int getcmd(char *prompt, char *args[], int *background)
{
 	int length, i, j = 0;
 	char *token, *loc;
 	char *line;
 	size_t linecap = 0;
	printf("%s", prompt);
 	length = getline(&line, &linecap, stdin);
	for (i = 0;i<20;i++){
                args[i] = NULL;
        }
	i = 0;
	
 	if (length <= 1) {
 		exit(-1);
	}
	// Check if background is specified..
	if ((loc = index(line, '&')) != NULL) {
	 	*background = 1;
	 	*loc = ' ';
	}

	else {
 		*background = 0;	
	}
	
 	while ((token = strsep(&line, " \t\n")) != NULL) {
		for (j = 0; j < strlen(token); j++){
 			if (token[j] <= 32){
 				token[j] = '\0';
			}
		}
 		if (strlen(token) > 0){
 			args[i++] = token;
			
		}
 		
	}
 	return i;
}

	
int main(void)
{
	int *numberOfArgs = 0;
	char *commandHistory[10];
	char *argHistory[10][20];
	int numberOfArgsAtI[10];
	int i,j, execute = 0;
	
        int historyLength = 0;
	
	char *args[20];
 	int bg;
 	while(1) {
		execute = 1;
 		bg = 0;
 		int cnt = getcmd("\n>> ", args, &bg);
		//Checks for history commands in format "r\n" or "r x\n"
		if (!strcmp("r",args[0])){
			//Searches commandHistory array for most recently added command which starts with the given character (args[1])
			int historyNumber = 1;
			if (args[1] != NULL){
				char commandChar = *args[1];	
				historyNumber = 1;
				for (i = historyLength-1;i>=0;i--){
					if (commandChar == commandHistory[i][0]){
						printf("Found in i: %d\n", i+1);	
						historyNumber = i;				
						break;
					}
				}
			}
			else {
				historyNumber = historyLength - 1;
			}
			
					
			args[0] = commandHistory[historyNumber];
			cnt = numberOfArgsAtI[historyNumber] + 1;
			printf("%s", args[0]);
			for(j=1;j<cnt;j++){
				args[j] = argHistory[historyNumber][j-1];
				printf(" %s", args[j]);
			}
			printf("\n");
			for (j = cnt;j<20;j++){
				args[j] = NULL;
			}	
		}
		else if (!strcmp("history", args[0])){
			execute = 0;
			printHistory(commandHistory,argHistory,historyLength, numberOfArgsAtI);
		}
		if (execute) {
			numberOfArgsAtI[historyLength] = cnt - 1;
			//Executes the given command. Wasn't sure if it should be in it's own method, so I left it in Main. 
			//Stores for the history
		

		
			char *command = args[0];	
			commandHistory[historyLength] = command;
		
		
			for (i=0;i<cnt-1;i++){
				args[i] = args[i+1];
			}
			for (i=0;i<cnt;i++){
				argHistory[historyLength][i] = args[i];
			}
			historyLength += 1;
			
		
			int childPid = fork();
			if (!childPid){
				
				execvp(command, args);
			}
			else {
				
				if (!bg){
					wait(childPid);
				}
			}
		}	
 	}
}

	

void printHistory(char** commands, char *arguments[10][20], int length, int numberOfArgsAtI[10]){
	int i, j;
	for (i=0;i<length;i++){
		
		printf("%d) %s",(i+1), commands[i]);
		for (j = 0;j<numberOfArgsAtI[i];j++){
			printf(" %s", arguments[i][j]);
		}
		printf("\n");
	}
}
int freecmd(){
 	// you need to implement this one. Might need some modifications to
 	// getcmd() provided with the assignmenti
}
