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

/**********************************************pragram global variable declartion here*****************************************************************************/
#define debug 1//0 = off, 1=on
#define MAX_LENGTH 1024//the max length of a line of command received
#define error 1



/*
sidenote:
if you want that part for debug use the following (also set debug =1 )
#if debug
#endif
*/


/*******all the function should be declare here with the short explanation of what this function do and how(by calling what and what should be the argument)***********/
int parse(char* input, char* commands, char* arguments);//parse the line of input, sepreate them into command catergory or argument and return both in pointers
int check_cmd(char* commands,char* dir, int depth);//function that check the comand if it is in the directory

//main function
int main(int argc, char *argv[]){

	//varriable declaration
	int err;
	char* input = (char *) malloc(MAX_LENGTH);
	char* commands = (char *)malloc(MAX_LENGTH);
	char* arguments = (char *)malloc(MAX_LENGTH);
	
	if(argc>1){
		fprintf(stderr, "this program %s does not take argument\n",argv[0]);
		return error;//return error/1 indicate taht this is a abnormal exit

	}
	while(1){//constantly running shell
		printf("$ ");//shell command line symbol
		if(!fgets(input, MAX_LENGTH, stdin)) break;//read the input untill it read a \n or the end of input
		if((err = parse(input, commands, arguments))==0) fprintf(stderr,"unable to use parse function");
	}

//free up all the used space after before quit
	free(input); //freee the space before quit
	return 0;
}

/***************************************all functions bodies should be written here***************************************************************************/
//parse the line of input, sepreate them into command catergory or argument and return both in pointers
int parse(char* input, char* commands, char* arguments)
{

	while(1){
		if((commands = strtok(input," \t\r\n")) != NULL){//the first word that is in the input is the command		

#if debug
			fprintf(stdout,"debug:the command is %s\n",commands);
#endif

		//check if the command is viable
		if(check_cmd(commands,"/bin", 0)) fprintf(stdout,"the command %s is in bin\n",commands);
		else if (check_cmd(commands,"/usr/bin",0)) fprintf(stdout, "the command %s is in the user bin",commands);
		else{
			fprintf(stderr,"command %s not found\n",commands);
			return error;
			}
		}
	}
	return 0;
}

//function that check the comand if it is in the directory
int check_cmd(char* commands,char* dir, int depth)
{
	// part of the code is come from http://blog.csdn.net/zhuyi2654715/article/details/7605051
	DIR *dirpath;
	struct dirent *entry;
	struct stat statbuf;//stat buffer

	if((dirpath = opendir(dir))== NULL){
		fprintf(stderr, "the directory %s is not able to be open\n",dir);
		return error;
	}

	chdir(dir);//change working directory

	while((entry = readdir(dirpath)) != NULL){//not the end of the directory
		lstat(entry->d_name, &statbuf);
		if (S_ISDIR(statbuf.st_mode)) {//check if it is still in the directory
			if (strcmp(entry->d_name, ".")==0||strcmp(entry->d_name, "..")==0) continue; 
			else if(strcmp(entry->d_name,commands)==1){//strcmp return 0 means it is a match
#if debug
			fprintf(stdout,"%s\n",entry->d_name);
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
	return 0;
}
