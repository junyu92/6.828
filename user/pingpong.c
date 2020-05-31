#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int fds[2];

int
main(int argc, char *argv[])
{
  int p;
  int pid;
  char buf[1];

  if(argc != 1){
    fprintf(2, "Usage: pingpong...\n");
    exit();
  }

  if(pipe(fds) != 0) {
    printf ("pipe() failed\n");
    exit();
  };

  p = fork();
  if (p < 0) {
    printf ("fork failed\n");
    exit();
  }

  if (p == 0) {
    while (read (fds[0], buf, sizeof(char) != sizeof(char))) {};
    pid = getpid();
    printf ("%d: received ping\n", pid);
    write (fds[1], buf, sizeof(char));
    exit();
  } 

  write (fds[1], buf, sizeof(char));
  while (read (fds[0], buf, sizeof(char) != sizeof(char))) {};
  pid = getpid();
  printf ("%d: received pong\n", pid);

  exit();
}
