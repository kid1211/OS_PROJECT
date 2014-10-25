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
#include <errno.h>

//pragram global variable declartion here
#define debug 1//0 = off, 1=on
#define MAX_LENGTH 1024//the max length of a line of command received
#define error 1



/*sidenote:
if you want that part for debug use the following (also set debug =1 )
#if debug
#endif
*/


//all the function should be declare here with the short explanation of what this function do and how(by calling what and what should be the argument)
void parse(char* input, char* commands, char* arguments);//parse the line of input, sepreate them into command catergory or argument and return both in pointers


//main function
int main(int argc, char *argv[]){

	//varriable declaration
	char* input = (char *) malloc(MAX_LENGTH);
	char* commands = (char *)malloc(MAX_LENGTH);
	char* arguments = (char *)malloc(MAX_LENGTH);
	
	if(argc>1){
		fprintf(stderr, "this program %S does not take argument\n",argv[0]);
		return(error);//return error/1 indicate taht this is a abnormal exit

	}
	while(1){//constantly running shell
		printf("$ ");//shell command line symbol
		if(!fgets(input, MAX_LENGTH, stdin)) break;//read the input untill it read a \n or the end of input
		parse(input, &commands, &arguments);
	}

//free up all the used space after before quit
	free(input); //freee the space before quit
	return 0;
}

//all functions bodies should be written here
void parse(char* input, char* commands, char* arguments)
{
	//parse the line of input, sepreate them into command catergory or argument and return both in pointers
	if(commands = strtok(input," \t\r\n")){//the first word that is in the input is the command		
#if debug
	fprintf(stdout,"debug:the command is %s\n",commands);
#endif
	}
	else return(error);

	return;
}





