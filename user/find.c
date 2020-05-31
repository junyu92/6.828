#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

int
str_match(char *pattern, char *name)
{
  int start;
  start = 0;

  if (strlen(pattern) != strlen(name)) {
    return 0;
  }

  while (pattern[start] != '\0' && name[start] != '\0') {
    if (pattern[start] != name[start]) {
      return 0;
    }
    start++;
  }
  return 1;
}

char buf[512];

void
find(char *path, char *pattern)
{
  int fd;
  struct stat st;
  struct dirent de;
  char *p;

  if ((fd = open(path, 0)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    printf ("find: %s is not a path\n", path);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
	continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("find: cannot stat %s\n", buf);
        continue;
      }
      if (st.type == T_FILE) {
	/* is file */
	if (str_match(pattern, de.name)) {
          printf("%s\n", buf);
	}
      } else {
	/* is directory, recursive */
        find (buf, pattern);
      }
    }
    break;
  }

  close(fd);

}

int
main(int argc, char *argv[])
{
  char *path;
  char *pattern;

  if(argc != 3){
    fprintf(2, "usage: find directory pattern \n");
    exit();
  }

  path = argv[1];
  pattern = argv[2];

  find(path, pattern);

  exit();
}

