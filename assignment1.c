#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
int getcmd(char *prompt, char *args[], int *background)
{
 	int length, i, j = 0;
 	char *token, *loc;
 	char *line;
 	size_t linecap = 0;
 	printf("%s", prompt);
 	length = getline(&line, &linecap, stdin);
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
	i = 0;
 	while ((token = strsep(&line, " \t\n")) != NULL) {
		for (j = 0; j < strlen(token); j++){
 			if (token[j] <= 32){
 				token[j] = '\0';
			}
 			if (strlen(token) > 0){
 				args[i++] = token;
			}
		}
 		
	}
 	return i;
}
int main(void)
{
	char *args[20];
 	int bg;
 	while(1) {
 		bg = 0;
 		int cnt = getcmd("\n>> ", args, &bg);
		printf("Here: %s", args[0]);		
 		/* the steps are:
		(1) fork a child process using fork()
		(2) the child process will invoke execvp()
		(3) if background == 0, the parent will wait,
		otherwise gets the next command... */
 	}
}
int freecmd(){
 	// you need to implement this one. Might need some modifications to
 	// getcmd() provided with the assignmenti
}
