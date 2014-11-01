/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------
Program/project name:

Aurthors: 
Xujie Zheng(xj721386) A00394753
Fengxiao Yuan(FengxiaYuan) A00394754
Jie Zhang(JieZhang0918) A00331569

Description: building a custom shell that include the following features
1.
2.
3.
4.
5.
6.
7.
8.
9.
-------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/wait.h>
/**********************************************pragram global variable declartion here*****************************************************************************/
#define debug 0//0 = off, 1=on
#define MAX_LENGTH 1024//the max length of a line of command received
#define error 1//errno
#define MAX_ARGS 20


/*
sidenote:
if you want that part for debug use the following (also set debug =1 )
#if debug
#endif
*/


/*******all the function should be declare here with the short explanation of what this function do and how(by calling what and what should be the argument)************************/
int parse(char* input, char** arguments);//parse the line of input, sepreate them into arguments array, the return number is how many argument there is, and arguments[0] is command
void check_cmd(char* commands,char* dir, int depth);//function that check the comand if it is in the directory

//main function
int main(int argc, char *argv[]){

	//varriable declaration
	char* input = (char *) malloc(MAX_LENGTH);
	char** arguments= (char **)malloc(MAX_ARGS*sizeof(char*));
   	int j;
       	for(j=0;j<MAX_ARGS;j++) arguments[j] = (char*)malloc(MAX_LENGTH+1);
	DIR* dirpath;
	int cargc;
	
	if(argc>1){
		fprintf(stderr, "this program %s does not take argument\n",argv[0]);
		exit(error);//return error/1 indicate taht this is a abnormal exit

	}

	//start of the main function
	while(1){//constantly running shell
		fprintf(stdout,"$ ");//shell command line symbol
		if(!fgets(input, MAX_LENGTH, stdin)) break;//if no argument is received the shell will quit
		//initialize the arguments[]
		cargc = parse(input, arguments);
		arguments[cargc+1] = NULL;//let the last argument be null
		
/*
#if debug
        int i;//counter for printing the output
        fprintf(stdout,"input is:\n");
        for(i=0;i<=cargc;i++) fprintf(stdout,"|%s|argument:%d\n",arguments[i],i);
#endif
*/
		int rc= fork();
		if(rc==0) execvp(*arguments,arguments);
		else if(rc>0){
		int wc = wait(NULL);

//		check_cmd(arguments[0],"/bin",0);//for tab feature
#if debug
	int i;//counter for printing the output
	fprintf(stdout,"input is:\n");
	for(i=0;i<=cargc;i++) fprintf(stdout,"%s\t",arguments[i]);
	fprintf(stdout,"\nargument is %d\n",cargc);
#endif
	}
}
//free up all the used space after before quit
	free(input); //freee the space before quit
	free(arguments);
	free(*arguments);
	return 0;

	
}
/***************************************all functions bodies should be written here***************************************************************************/
//parse the line of input, sepreate them into command catergory or argument and return both in pointers
int parse(char* input, char** arguments)
{
	int i=0;//counter for number of arguments
	arguments[i]=strtok(input," \t\r");
	
	
	while(arguments[i][strlen(arguments[i])-1] != '\n'){//since the last bit/interger of the string is \0 therefore the \n is the last bit -a bit
		
	
	
			//the first token  is in the input is the command and the rest are the argument
			i++;
			arguments[i] = strtok(NULL," \t\r") ;	
	
	}
	arguments[i][strlen(arguments[i])-1] = '\0';//remove the enter from the input and replce it with \0
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
#if debug
			fprintf(stdout,"%sis in folder %s\n",entry->d_name,dir);
#endif			

			}
		check_cmd(commands,entry->d_name,depth+4);//go inside the folder
		}
		else if(strcmp(entry->d_name,commands)==0){
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
