#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char* argv[])
{
  int p[2];
  pipe(p);
  char nums[34];
  for (int i = 0;i <= 33;i++) {
    nums[i] = i + 2;
  }
  write(p[1], nums, 34);
  close(p[1]);

  while (1) {
    char div;
    if (read(p[0], &div, 1) == 0) {
      close(p[0]);
      break;
    }
    printf("prime %d\n", div);

    char n;
    char res[34];
    int i = 0;
    while (read(p[0], &n, 1) != 0) {
      if (n % div != 0) res[i++] = n;
    }
    close(p[0]);

    pipe(p);
    write(p[1], res, i);
    close(p[1]);
  }

  exit(0);
}
