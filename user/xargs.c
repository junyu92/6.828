#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

char *child_argv[MAXARG];
char buf[512];

int
main(int argc, char *argv[])
{
  int i, f;

  if (argc < 2) {
    printf("help: xargs command\n");
    exit();
  }

  i = 0;
  while (i < argc - 1) {
    child_argv[i] = argv[i+1];
    i++;
  }

  while (1) {
    int pos = argc-1;
    gets(buf, 512);

    if (buf[0] == 0) break;

    int left = 0, right = 0;
    int len = strlen(buf);
    while (right <= len-1) {
      if (right == len-1 || buf[right] == ' ') {
	buf[right] = '\0';
        child_argv[pos] = &buf[left];
        left = right+1;
        right = left;
        pos++;
      } else {
        right++;
      }
    }

    while (pos < MAXARG) {
      child_argv[pos] = 0;
      pos++;
    }

    f = fork();
    if (f < 0) {
      printf ("fork failed\n");
      exit();
    }
    if (f == 0) {
      exec(argv[1], child_argv);
      exit();
    }
    wait();
  }
  exit();
}
