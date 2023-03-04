#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int
main(int argc, char* argv[])
{
  char* argvall[MAXARG];
  for (int i = 0;i < argc - 1;i++) {
    argvall[i] = (char*)malloc(sizeof argv[i + 1]);
    memmove(argvall[i], argv[i + 1], sizeof argv[i + 1]);
  }

  char buf[512];
  int index = 0;
  while (read(0, &buf[index++], sizeof(char)) != 0);
  buf[index] = 0;
  int start = 0;
  int argindex = argc - 1;
  for (int i = 0;buf[i] != 0;i++) {
    if (buf[i] == ' ' || buf[i] == '\n') {
      argvall[argindex] = (char*)malloc((i - start) * sizeof(char));
      memmove(argvall[argindex], &buf[start], (i - start) * sizeof(char));
      argvall[argindex++][i - start] = 0;
      while (buf[i] == ' ' || buf[i] == '\n') i++;
      start = i;
    }
  }

  if (fork() == 0) {
    exec(argvall[0], argvall);
  }
  wait(0);
  exit(0);
}
