/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------
Program/project name:

Aurthors: 
Xujie Zheng(xj721386) A00394753
Fengxiao Yuan(FengxiaYuan) A00394754
Jie Zhang(JieZhang0918) A00331569

Description: building a custom shell that include the following features
1.check if the command is found
2.listing the command that has been input command cmdhistory (this will print the cmd entered even it is an unsuccessful input, give a chance for the user for checking)
3.use "usecmdhistory" and 0~20 can access use the cmd that is type directly
4.ctrl+c or ctrl+z to end(quit) the shell
5.after modify the config.h file, another compilation is require also another ./a.out run
6.
7.
8.
9.
10.
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
int parse(char* input, char** arguments);//parse the line of input, seperate them into arguments array, the return number is how many argument there is, and arguments[0] is command
//follow two function can be combined together
char* check_cmd(char* commands,char* dir, int depth, int further);//function that check the command if it is in the directory




//main function
int main(int argc, char *argv[]){
	char** arg_his= (char **) malloc(MAX_LOG*sizeof(char*));//store input from user
	int j;
	//int mypipe[2];
	//char *command_arrayA[10];
    	//char *command_arrayB[10];
	//int pipe_flag=1;
	//char *line[2];
	for(j=0;j<MAX_LOG;j++) arg_his[j] = (char*) malloc(MAX_LENGTH+1);//the +1 is for null
	lognumber=0;//initialize the global variable for the arguments history

	//return value from check_cmd
	char* tab_log = (char*) malloc(MAX_LENGTH+1);//the +1 is for null and it can only show 20 similiar tab result

	if(argc>1){
		fprintf(stderr, "this program %s does not take argument\n",argv[0]);
		exit(error);//return error/1 indicate taht this is a abnormal exit

	}

	//start of the main function
	while(1){//constantly running shell
		printf("NBshell ");
		//initialize the global variable
		
			
		//varriable declaration
		char* input = (char *) malloc(MAX_LENGTH);
		char** arguments= (char **) malloc(MAX_ARGS*sizeof(char*));
       		for(j=0;j<MAX_ARGS;j++) arguments[j] = (char*) malloc(MAX_LENGTH+1);//the +1 is for null
		
		int cargc;
		fprintf(stdout,"$ ");//shell command line symbol

		//cannot handle the tab or up arrow input in concurren. Maybe create a new process to monitor the input from user might work, but it will take a lot of work
		if(!fgets(input, MAX_LENGTH, stdin)) break;//if the argument is sending longer than MAX_LENGTH the program will quit


		strcpy(arg_his[lognumber],input);
		lognumber++;
		
	

		
		//parse the input
		cargc = parse(input, arguments);
		arguments[cargc+1] = NULL;//let the last argument be null


	
		if(arguments[cargc][strlen(arguments[cargc])-1] == '\t'){//need to auto finish
			//get current directory and store it so that later on it can be retreat
			char* cwd = getcwd(NULL,0);
			if(cwd==NULL) fprintf(stderr,"getcwd() error");
#if debug
			else fprintf(stdout,"current directory is %s\n",cwd);
#endif

			//printf("getcha->%c<-\n\n",arguments[cargc][strlen(arguments[cargc])-1]);
			//initialize an array for store the value from check_cmd for tab feature

			tab_log = NULL;
			//further is equal to 0
			tab_log = check_cmd(arguments[cargc],cwd, 0,0);
			
			if(tab_log != NULL){
				//this will make the program run 2 times for reasons that is unkonwn
				
				/*fprintf(stdout,"do you mean %s? y/n\n",tab_log);
				if(getchar() == 'y') */
					arguments[cargc] = tab_log;

			}
			else fprintf(stderr,"more than one file is matched or no such file in this folder\n");
			tab_log = NULL;	
			chdir(cwd);//go back to current cwd
			free(cwd);
		}	
			
			
		//special case command
		if(strcmp(arguments[0],"cmdhistory")==0){//print the cmd history
			for(j = 0; j<lognumber;j++) fprintf(stdout,"%d %s",j,arg_his[j]);	
			tab_log = "cmdhistory";
		}
		if(strcmp(arguments[0],"usecmdhistory")==0){//use one of the command from history
			//parse the input of the desired input from the history
			int targetlog = atoi(arguments[1]);
			cargc = parse(arg_his[targetlog], arguments);//change the second argument to interger so that the arg_his can be access
			arguments[cargc+1] = NULL;//let the last argument be null
			tab_log = "usecmdhistory";		
		}
		if(strcmp(arguments[0],"cd")==0){
			chdir(arguments[1]);			
			tab_log = "cd";
		}

		
/*************************going to create a new process to run***********************************************************************************************************************/		
		int rc= fork();
		if(rc<0) fprintf(stderr, "unable to create a new process,worst shell ever\n");
		else if(rc==0) {//excute the valid command and also check if the command is valid and tab feature
			//get current directory and store it so that later on it can be retreat
			char* cwd = getcwd(NULL,0);
			if(cwd==NULL) fprintf(stderr,"getcwd() error");
#if debug
			else fprintf(stdout,"current directory is %s\n",cwd);
#endif
			//tab   
			tab_log= NULL;
			tab_log = check_cmd(arguments[0],"/bin",0,1);
			if(tab_log==NULL) {
				tab_log=check_cmd(arguments[0],"/usr/bin",0,1);//for tab feature
				if(tab_log ==NULL) fprintf(stderr,"command is not found\n");//if no command is found then tab_log return null
			}
			chdir(cwd);
			free(cwd);
#if debug
			//display all the arguments that is parsed to excute
			int b;
			for(b =0 ; b<sizeof(arguments);b++) fprintf(stdout,"|%d %s|\n",b,arguments[b]);
#endif	
			
			execvp(*arguments,arguments);
	
		}
		else if(rc>0){
			//int wc = wait(NULL);//return what child process it has been waited
			wait(NULL);
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
	int i=0;//counter for number of arguments9
	char *p;
	arguments[i]=strtok(input," ");//no \n for the strtok so that the \n will not be removed
	while(arguments[i][strlen(arguments[i])-1] != '\n'){//since the last bit/interge of the string is \0 therefore the \n is the last bit -a bit
			//the first token  is in the input is the command and the rest are the argument
			p = arguments[i];
			if (strcmp(p, "|") == 0){
				arguments[i] = NULL;
			}
			i++;
			arguments[i] = strtok(NULL," ");

	}
	arguments[i][strlen(arguments[i])-1] = '\0';//remove the enter from the input and replace it with \0
	return i;
}

//function that check the comand if it is in the directory
char* check_cmd(char* commands,char* dir, int depth, int further)
{		
	//return a string for tab feature and return one string if it is normal usage
	char* tab_log = (char*) malloc(MAX_LENGTH+1);//the +1 is for null and it can only show 20 similiar tab result
	// part of the code is come from http://blog.csdn.net/zhuyi2654715/article/details/7605051
	DIR *dirpath;

	int dirtybit=0;//indicate if the tab match has been more than one
	struct dirent *entry;
	struct stat statbuf;//stat buffer
	if((dirpath = opendir(dir))== NULL){
		fprintf(stderr, "the directory %s is not able to be open\n",dir);
		return 0;
	}

	chdir(dir);//change working directory

	while((entry = readdir(dirpath)) != NULL){//not the end of the directory
		lstat(entry->d_name, &statbuf);
		if (S_ISDIR(statbuf.st_mode)) {//check if it is still in the directory
			if(further==1){
				if (strcmp(entry->d_name, ".")==0||strcmp(entry->d_name, "..")==0) continue; 
				else if(strcmp(entry->d_name,commands)==1){//strcmp return 0 means it is a match
					check_cmd(commands,entry->d_name,depth+4,1);//go inside the folder
				}
				else if(strcmp(entry->d_name,commands)==0){
					tab_log = entry->d_name;
#if debug
					 fprintf(stdout,"%s is in foloder %s\n",entry->d_name,dir);
#endif

				}
			
			}
			else if(further == 0){//tab feature, it will compare the cmd with all the file in the working directory
				char* temp = (char*) malloc( strlen(commands)+1);
				int strcpycounter;
				if(strlen(commands) <= strlen(entry->d_name)){
					for(strcpycounter=0;strcpycounter<(strlen(commands)-1);strcpycounter++) temp[strcpycounter] = entry->d_name[strcpycounter];
					temp[strcpycounter] = '\t';
					if(strcmp(temp,commands)==0) {
							tab_log = entry->d_name;
							dirtybit++;
							//free up memory before quit
							free(temp);
					}


				}
				
			}
		}//s_ISDIR
	}//WHILE
	//change the working directory back to home and close it before return
	chdir("..");
	closedir(dirpath);
	if(further==0 && dirtybit!= 1) tab_log=NULL; //dirtybit will only be checked when it is a tab feature implementation
	
	return tab_log;//return nothing
	
}






