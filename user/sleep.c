#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char* argv[])
{
  if (argc != 2) {
    write(2, "sleep: wrong operand\n", 22);
    exit(1);
  }
  int time = atoi(argv[1]);
  sleep(time);
  exit(0);
}
