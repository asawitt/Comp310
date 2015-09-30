#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
void printHistory();
void printBackgroundJobs();
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
	int reverseAssociatedPID[1000];
	int bgAssociatedIndexInCommandArray[1000];
	int numberOfBgProcesses = 0;
	int backgroundProcesses[1000];
	int *numberOfArgs = 0;
	char *commandHistory[1000];
	char *argHistory[1000][20];
	int numberOfArgsAtI[1000];
	int i,j, execute = 0;
	int historyLengthLess10 = 0;
        int historyLength = 0;
	int found = 0;
	char *args[20];
 	int bg;
	char username[50];
	char *path;
 	while(1) {
		execute = 1;
 		bg = 0;
		found = 0;
 		int cnt = getcmd("\n>> ", args, &bg);
		//Checks for built in "cd" command (and executes)
		if (!strcmp("cd",args[0])){
			//Makes sure command wasn't just "cd" without a destination directory
			if(args[1] != NULL){
				path = args[1];	
			}
			//If the command was just "cd"
			else {
				
				getlogin_r(username,50);
				strcpy(path,"/home/");
				strcat(path,username);
				
			}
			chdir(path);
			execute=0;
		}
		//Checks for built in command "pwd"
		if (!strcmp("pwd",args[0])){
			char cwd[100];
			if (args[1] != NULL){
				printf("\"pwd\" takes no arguments. Current working directory is: \n");
			}
			getcwd(cwd,100);
			printf("%s\n",cwd);
			execute = 0;
			
		}
		//checks for built in command "exit"
		else if (!strcmp("exit",args[0])){
			printf("Bye!\n");
			exit(-1);
		}
		//checks for built in command "jobs"
		else if (!strcmp("jobs",args[0])){
			printBackgroundJobs(backgroundProcesses, numberOfBgProcesses, commandHistory, argHistory, numberOfArgsAtI, bgAssociatedIndexInCommandArray, reverseAssociatedPID);
			execute = 0;
		}
		//Checks for history commands in format "r\n" or "r x\n"
		else if (!strcmp("r",args[0])){
			//Searches commandHistory array for most recently added command which starts with the given character (args[1])
			int historyNumber = 1;
			if (args[1] != NULL){
				char commandChar = *args[1];	
				historyNumber = 1;
				historyLengthLess10 = historyLength - 10;
				if (historyLengthLess10 < 0) historyLengthLess10 = 0;
				for (i = historyLength-1;i>=historyLengthLess10;i--){
					if (commandChar == commandHistory[i][0]){	
						historyNumber = i;
						found = 1;				
						break;
					}
				}
				//Error message for invalid commands
				if (found == 0){
					printf("Command not found; please enter commands in the form \"r x\", where x is the first letter of one of the last 10 commands entered. These last 10 commands can be viewed with the command \"history\"");
					execute = 0;
				}
			}
			else {
				historyNumber = historyLength - 1;
			}
			
					
			args[0] = commandHistory[historyNumber];
			cnt = numberOfArgsAtI[historyNumber] + 1;
			if (execute){
				printf("%s", args[0]);
				for(j=1;j<cnt;j++){
					args[j] = argHistory[historyNumber][j-1];
					printf(" %s", args[j]);
				}
			}
			printf("\n");
			for (j = cnt;j<20;j++){
				args[j] = NULL;
			}	
		}
		//Checks for "history" command
		else if (!strcmp("history", args[0])){
			execute = 0;
			printHistory(commandHistory,argHistory,historyLength, numberOfArgsAtI);
		}
		//Checks for "fg" command 
		else if (!strcmp("fg",args[0])){
			
			execute = 0;
			if(args[1] != NULL){
				int arg1 = atoi(args[1]);
				int status = waitpid(reverseAssociatedPID[arg1],NULL,WNOHANG);
				if (status != 0) {
					printf("That job has already finished, or doesn't exist, try the command with a process currently executing in the background \n");
				}
				else {
					printf("Waiting for Child Process: %d, with PID: %d\n", arg1, reverseAssociatedPID[arg1]);
					waitpid(reverseAssociatedPID[arg1], NULL, 0);
				}
			}
			else {
				printf("Please enter a second argument denoting the number of the job, as found after executing the \"jobs\" command\n");
			}
			
		}
		//Main command execution
		if (execute) {
			numberOfArgsAtI[historyLength] = cnt - 1; //count - 1 because it doesn't include the actual command, only arguments

			
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
				//prints a new line, so that any bg children that print don't interfere with prompt in shell
				if (!bg){
					printf("\n");
				}
				execvp(command, args);
				
			}
			else {
				
				if (!bg){
					int status;
					waitpid(childPid, &status, 0);
				}
				else {
					bgAssociatedIndexInCommandArray[numberOfBgProcesses] = historyLength-1;
					backgroundProcesses[numberOfBgProcesses] = childPid;
					numberOfBgProcesses++;
				}
			}
		}	
 	}
}

	

void printHistory(char** commands, char *arguments[1000][20], int length, int numberOfArgsAtI[1000]){
	int i, j;
	i = length - 10;
	if (i<0) i=0;
	for (i;i<length;i++){
		
		printf("%d) %s",(i+1), commands[i]);
		for (j = 0;j<numberOfArgsAtI[i];j++){
			printf(" %s", arguments[i][j]);
		}
		printf("\n");
	}
}
void printBackgroundJobs(int bgProcesses[1000],int length, char** commands, char *arguments[1000][20], int numberOfArgsAtI[1000], int associatedIndex[1000], int reverseAssociatedPID[1000]){
	int i, j;
	int status;
	int numberOfJobs = 0;
	for (i=0;i<length;i++){
		status = waitpid(bgProcesses[i],NULL,WNOHANG);
		if (status == 0) {
			
			numberOfJobs++;
			reverseAssociatedPID[numberOfJobs] = bgProcesses[i];
			printf("ReverseAssociatedIndex of: %d is %d\n", numberOfJobs, bgProcesses[i]);
			printf("[%d) pid: %d]:", numberOfJobs, bgProcesses[i]);
			printf(" %s", commands[associatedIndex[numberOfJobs-1]]);
			for (j = 0;j<numberOfArgsAtI[associatedIndex[numberOfJobs-1]];j++){
				printf(" %s", arguments[associatedIndex[numberOfJobs-1]][j]);
			}
			printf("\n");
		}
	}
}
int freecmd(){
 	// you need to implement this one. Might need some modifications to
 	// getcmd() provided with the assignment
}
