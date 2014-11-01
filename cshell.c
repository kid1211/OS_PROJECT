/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------
Program/project name:

Aurthors: 
Xujie Zheng(xj721386) A00394753
Fengxiao Yuan(FengxiaYuan) A00394754
Jie Zhang(JieZhang0918) A00331569

Description: building a custom shell that include the following features
1.check if the command is found
2.listing the command that has been input command cmdhistory (this will print the cmd entered even it is an unsuccessful input, give a chance for the user to correct it)
3.ctrl+c or ctrl+z to end(quit) the shell
4.
5.
6.
7.
8.
9.
-------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/wait.h>

/*******all the function should be declare here with the short explanation of what this function do and how(by calling what and what should be the argument)************************/
int parse(char* input, char** arguments);//parse the line of input, sepreate them into arguments array, the return number is how many argument there is, and arguments[0] is command
void check_cmd(char* commands,char* dir, int depth);//function that check the comand if it is in the directory

//main function
void main(int argc, char *argv[]){
	char** arg_his= (char **) malloc(MAX_LOG*sizeof(char*));//store input from user
	int j;
	for(j=0;j<MAX_LOG;j++) arg_his[j] = (char*) malloc(MAX_LENGTH+1);//the +1 is for null
	lognumber=0;//initialize the global variable for the arguments history
	if(argc>1){
		fprintf(stderr, "this program %s does not take argument\n",argv[0]);
		exit(error);//return error/1 indicate taht this is a abnormal exit

	}

	//start of the main function
	while(1){//constantly running shell
		//varriable declaration
		char* input = (char *) malloc(MAX_LENGTH);
		char** arguments= (char **) malloc(MAX_ARGS*sizeof(char*));
   		
       		for(j=0;j<MAX_ARGS;j++) arguments[j] = (char*) malloc(MAX_LENGTH+1);//the +1 is for null
		int cargc;
		fprintf(stdout,"$ ");//shell command line symbol
		if(!fgets(input, MAX_LENGTH, stdin)) break;//if the argument is sending longer than MAX_LENGTH the program will quit
		strcpy(arg_his[lognumber],input);
		lognumber++;
		
	
		//initialize the arguments[]
		cmdfound=0;
		
		//parse the input
		cargc = parse(input, arguments);
		arguments[cargc+1] = NULL;//let the last argument be null
		
		//special case command
		if(strcmp(arguments[0],"cmdhistory")==0){//print the cmd history
			for(j = 0; j<lognumber;j++) fprintf(stdout,"%d %s",j,arg_his[j]);	
			cmdfound = 1;
		}
		if(strcmp(arguments[0],"cd")==0){
			chdir(arguments[1]);
			cmdfound =1;
		}
	
/*************************going to create a new process to run************************************************************************************************************************/		
		int rc= fork();
		if(rc<0) fprintf(stderr, "unable to create a new process,worst shell ever\n");
		else if(rc==0) {//excute the valid command and also check if the command is valid and tab feature
			char* cwd = getcwd(NULL,0);//get current directory and store it so that later on it can be retreat
			if(cwd==NULL) fprintf(stderr,"getcwd() error");
#if debug
			else fprintf(stdout,"current directory is %s\n",cwd);
#endif
			
			check_cmd(arguments[0],"/bin",0);
			if(!cmdfound) check_cmd(arguments[0],"/usr/bin",0);//for tab feature
			if(!cmdfound) fprintf(stderr,"command is not found\n");//cmd found maks cmdfound =1
			chdir(cwd);
			free(cwd);
			execvp(*arguments,arguments);
	
		}
		else if(rc>0){
			int wc = wait(NULL);//return what child process it has been waited
			//free up all the used space after before quit
			free(input); //freee the space before quit
			free(arguments);
		}
	}
	
	exit(1);//if the program quit this way, something is wrong

	
}
/***************************************all functions bodies should be written here***************************************************************************/
//parse the line of input, sepreate them into command catergory or argument and return both in pointers
int parse(char* input, char** arguments)
{
	int i=0;//counter for number of arguments
	arguments[i]=strtok(input," ");//no \n for the strtok so that the \n will not be removed
	
	
	while(arguments[i][strlen(arguments[i])-1] != '\n'){//since the last bit/interge of the string is \0 therefore the \n is the last bit -a bit
		
	
	
			//the first token  is in the input is the command and the rest are the argument
			i++;
			arguments[i] = strtok(NULL," ") ;	
	
	}
	arguments[i][strlen(arguments[i])-1] = '\0';//remove the enter from the input and replace it with \0
	return i;
}

//function that check the comand if it is in the directory
void check_cmd(char* commands,char* dir, int depth)
{		

	// part of the code is come from http://blog.csdn.net/zhuyi2654715/article/details/7605051
	DIR *dirpath;
	struct dirent *entry;
	struct stat statbuf;//stat buffer
	if((dirpath = opendir(dir))== NULL){
		fprintf(stderr, "the directory %s is not able to be open\n",dir);
		return;
	}

	chdir(dir);//change working directory

	while((entry = readdir(dirpath)) != NULL){//not the end of the directory
		lstat(entry->d_name, &statbuf);
		if (S_ISDIR(statbuf.st_mode)) {//check if it is still in the directory
			if (strcmp(entry->d_name, ".")==0||strcmp(entry->d_name, "..")==0) continue; 
			else if(strcmp(entry->d_name,commands)==1){//strcmp return 0 means it is a match
				cmdfound =1; //indicate that the command is found
#if debug
			fprintf(stdout,"%sis in folder %s\n",entry->d_name,dir);
#endif			

			}
		check_cmd(commands,entry->d_name,depth+4);//go inside the folder
		}
		else if(strcmp(entry->d_name,commands)==0){
			cmdfound =1; //indicate that the command is found
#if debug
			 fprintf(stdout,"%s is in foloder %s\n",entry->d_name,dir);
#endif

		}
	}
	//change the working directory back to home and close it before return
	chdir("..");
	closedir(dirpath);
	return;
}
