#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char* i;
  int t;

  if(argc < 2){
    fprintf(2, "Usage: sleep seconds...\n");
    exit();
  }

  i = argv[1];
  t = atoi(i);
  sleep(t);

  exit();
}
