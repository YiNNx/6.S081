#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char* argv[])
{
  int pid;
  int p1[2];
  int p2[2];

  if (pipe(p1) < 0 || pipe(p2) < 0) {
    fprintf(STDERR, "pipe error");
    exit(1);
  }

  if ((pid = fork()) == 0) {
    char msg[20];

    read(p1[0], msg, 20);
    fprintf(STDOUT, "%d: received %s\n", getpid(), msg);
    close(p1[0]);

    write(p2[1], "pong", 20);
    close(p2[1]);
  }
  else if (pid > 0) {
    char msg[20];

    write(p1[1], "ping", 20);
    close(p1[1]);

    read(p2[0], msg, 20);
    fprintf(STDOUT, "%d: received %s\n", getpid(), msg);
    close(p2[0]);
  }
  else {
    fprintf(STDERR, "fork error");
    exit(1);
  }
  exit(0);
}
