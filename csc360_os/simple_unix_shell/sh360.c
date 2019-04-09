/*
* Name: Hannah Raju
* ID: V00797276
* Date: 6 June 2018
* Details: CSC 360 Assignment 01
*
* References:
* - https://brennan.io/2015/01/16/write-a-shell-in-c/
* - Appendix_(a-f).c, provided.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define MAX_INPUT_LINE 80
#define MAX_RC_LEN 12
#define MAX_ARG_LEN 8
#define MAX_PROMPT_LEN 10


char RC_DIRS[MAX_RC_LEN][MAX_INPUT_LINE];
int RC_NUM;

/*
* Function definitions:
*/
void read_rc();
void read_args(char*, char*[] );
int execute(char*[], int );
int process_launch(char*[] , int);
void append_arg(char*[] );
void ch_dir(char*[] );
void get_pwd();
int* find_arrows(char*[] );
int OR_process(char* [], char[], char[]);
int PP_process(char*[]);
void set_path();


int main(int argc, char* argv[]){
	
	char input [MAX_INPUT_LINE];
	int line_length;
	char* args_list[MAX_INPUT_LINE];
	char* rc[MAX_RC_LEN];
	int num_args;
	int flag;
	char prompt [MAX_PROMPT_LEN];

	// read rc file 
	read_rc();
	
	strcpy(prompt,RC_DIRS[0]);
	
	// main command loop
	for(;;){
		
		flag = -1;

		fprintf(stdout, "%s", prompt);
		fgets(input, MAX_INPUT_LINE, stdin);
		line_length = strlen(input);
		
		// if no input provided, keep prompting user
		if (line_length == 1){
			continue;
		}

		// if input longer than 80 characters, invalid\n
		if(line_length > 80){
			fprintf(stderr, "Invalid input: must be less than 80 characters.\n");
			continue;
		}

		//Replace newline with null character
		if(input[line_length-1] == '\n'){
			input[line_length-1] = '\0';		
		}
	
		
		// else parse args
		read_args(input, args_list);

		//set OR/PP flags
		if(strcmp(args_list[0], "OR") == 0){
			flag = 1;
		}
		if(strcmp(args_list[0], "PP") == 0){
			flag = 2;
		}

		// execute command/process
		execute(args_list, flag);
	}
	
	return 0;
}

/*
* READ_RC(): This function parses the .sh360rc file and stores the prompt and paths specified in 
* the global array RC_DIRS.
* References: https://www.tutorialspoint.com/cprogramming/c_file_io.htm
*/
void read_rc(){
	
	char line[MAX_INPUT_LINE];
	int i = 0;
	RC_NUM = 0;

	FILE* f = fopen(".sh360rc", "r");
	
	// Read in lines of .sh360rc file
	while(fgets(line, sizeof(line), f)){

		RC_NUM++;
		// get rid of newline characters
		if(line, line[strlen(line)-1] == '\n'){
			line[strlen(line)-1] = '\0';		
		}

		strcpy(RC_DIRS[i],line);
		i++;

	}

	strcpy(RC_DIRS[i], "\0");
	fclose(f);

}

/*
* 	READ_ARGS: takes the input arguments from standard in and strips by whitespace
* 	to determine the individual arguments.
*/
void read_args(char* input, char* args_list[]){
	
	int i = 0;

	// make pointer to first element
	char *ptr = strtok(input, " ");
	
	// walk through rest of args
	while( ptr != NULL ){
		
		// add it to list of args
		args_list[i] = ptr;
		i++;
		
		// update pointer
		ptr = strtok(NULL, " ");
	}
	
	// append null character to define end of argslist
	args_list[i] = 0;
	

}

/*
* EXECUTE: Takes in status (int) as a parameter and calls the
* associated function.
*/
int execute(char* args_list[], int flag){

	// exit
	if (strcmp(args_list[0], "exit")==0){
		exit(0);
		
	}
	// if "cd", run ch_dir
	else if(strcmp(args_list[0], "cd")==0){
		ch_dir(args_list);
	}
	
	// run pwd
	else if(strcmp(args_list[0], "pwd") == 0){
		get_pwd();
	
	}

	
	// else spawn process
	else{
		
		process_launch(args_list, flag);	
	}		
}
/*
* PROCESS_LAUNCH(): This function determines whether the command contains output redirection (OR),
* piping (PP), or neither. Sets flag to 1 or 2 for OR and PP respectively. If the command is neither
* OR or PP, this function calls fork() and proceeds to run the specified process.
*/	
int process_launch(char* args_list[], int flag){

	char pre[MAX_INPUT_LINE];
	char arg[MAX_INPUT_LINE];	

	// if OR --> note only 1 arrow
	if(flag == 1){
		flag = -1;
		OR_process(args_list, pre, arg);
		return 0;
	}
	
	// if PP --> note could be multiple arrows
	else if(flag == 2){
		flag = -1;
		PP_process(args_list);
		return 0;
	}
		
	
	else{

		int pid;
		int status;
		char* envp[] = {0};
		
		// set arg to original input
		strcpy(arg, args_list[0]);

		// append first arg with rc line until binary file found

		int i;
	
		for(i=1; i<RC_NUM; i++){

			set_path(i, args_list, pre, arg);

			if((pid=fork())==0){

				if(execve(args_list[0], args_list, envp )== -1){

					continue;
				}
			}
			waitpid(pid, &status, 0);
			return 0;
		}
		fprintf(stderr, "%s: command not found\n", arg);

	
	}

	return 1;
}
/*
* SET_PATH(): This function prepends the specified path in the RC file to the first given argument.
*/
void set_path(int i, char* list[], char p[], char a[]){

	// set argslist 0 back to original
	list[0] = a;

	// set pre to current rc line
	strcpy(p,RC_DIRS[i]);

	// replace null character with / then re-add null 
	p[strlen(p)] = '/';


	// append args 0 to pre
	strcat(p, list[0]);

	// reassign pre to args 0
	list[0] = p;
}
/*
* FIND_ARROWS(): This function iterates throught the tokenized arguments and determines
* the indices containing '->' tokens.
*/
int* find_arrows(char* args_list[]){

	int arrow_locs[MAX_INPUT_LINE];
	int i;
	int j = 0;
	for(i=0; args_list[i]!=NULL; i++){
		if(strcmp(args_list[i], "->") == 0){
			arrow_locs[j] = i;
			j++;
		}
	}
	arrow_locs[j] = -1;

	return arrow_locs;
} 


/*
* OR_PROCESS(): This function is called to handle output redirection commands.
*/
int OR_process(char* args_list[], char pre[], char arg[]){

	int pid;
	int status;
	char* envp[] = {0};
	int fd;

	// get arrows
	int* arrows;
	arrows = find_arrows(args_list);
	int arrow_location = arrows[0];

	// Check if -> given
	if(arrows[0] == -1){
		fprintf(stderr, "OR: -> token missing from argument\n");
		return(1);
	}
	// Check if filename given
	if(args_list[arrow_location+1] == NULL){
		fprintf(stderr, "OR: filename missing from argument\n");
		return(1);
	}

	// Make new args_list to only contain args
	char* args[MAX_ARG_LEN];
	int i;
	int j = 0;
	for(i=1; i<arrow_location; i++){
		args[j] = args_list[i];
		j++;
	}
	
	args[j] = NULL;

	// set arg to be original input
	strcpy(arg, args[0]);

	char* filename = args_list[arrow_location+1];

	// loop over rc args
	for(i=1; i<RC_NUM; i++){

		set_path (i, args, pre, arg);

		if((pid=fork())==0){

			fd = open(filename, O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
			if(fd == -1){
				fprintf(stderr, "cannot open %s for writing\n", filename);
				return 1;
			}

			dup2(fd, 1);
			dup2(fd, 2);
			if(execve(args[0], args, envp )== -1){
				continue;
			}
		}
		waitpid(pid, &status, 0);
		return 0;
	}
	fprintf(stderr, "%s: command not found\n", args[0]);
	return 1;

}

/*
* PP_PROCESS(): This function is called to handle pipe commands.
* - RC file pathnames not given here, so have to enter them manually
* - also cannot handle more than one pipe
*/
int PP_process(char* args_list[]){
	char* head[MAX_ARG_LEN];
    	char* tail[MAX_ARG_LEN];
	char *envp[] = { 0 };
   	int status;
    	int pid_head, pid_tail;
    	int fd[2];

	// get arrows
	int* arrows;
	arrows = find_arrows(args_list);
	
	// Make new args_list to only contain args
	int i;
	int j = 0;

	// head
	for(i=1; i<arrows[0]; i++){
		head[j] = args_list[i];
		j++;
	}
	head[j] = NULL;

	// tail
	j = 0;
	i++;
	for(i; args_list[i]!=NULL;i++){
		tail[j] = args_list[i];
		j++;
	}
	tail[j] = NULL;
	
	pipe(fd);


    	if ((pid_head = fork()) == 0) {

        	dup2(fd[1], 1);
        	close(fd[0]);
        	if(execve(head[0], head, envp)==-1){
			fprintf(stderr, "%s: command not found\n", head[0]);
		}
    	}

    	if ((pid_tail = fork()) == 0) {

        	dup2(fd[0], 0);
        	close(fd[1]);
        	if(execve(tail[0], tail, envp)==-1){
			fprintf(stderr, "%s: command not found\n", tail[0]);
		}
    	}
 	close(fd[0]);
    	close(fd[1]);

   	waitpid(pid_head, &status, 0);
    	waitpid(pid_tail, &status, 0);

	return 0; 
}

	
			
/*
* CH_DIR: The function called when "cd" is given, changes to specified
* directory or reports an error if one occurs. (BUILT-IN)
*/
void ch_dir(char* args_list[]){
	
	// make sure the arguments following the cd aren't null
	
	if(args_list[1] == NULL){
		fprintf(stderr, "Provide a path to desired directory.\n");
	}
	else{
	
		// chdir() returns 0 if successful, flag to check
		int cd_flag;
		cd_flag = chdir(args_list[1]);
		if(cd_flag != 0){
			fprintf(stderr, "sh360: cd: %s: No such file or directory.\n", args_list[1]);
		}
	}
}

/*
* GET_PWD: The function called when "pwd" is given, prints the current working 
*	directory to stdout.
* 
* Referenced https://stackoverflow.com/questions/16285623/pwd-to-get-path-to-the-current-file
* for help with this function.
*/
void get_pwd(){
	
	int max_path_length = 100;
	char pwd[100];
	getcwd(pwd, max_path_length);
	fprintf(stdout,"%s\n", pwd);
}


