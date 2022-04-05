#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main() {
  char dtemp[] = "/tmp/dirXXXXXX";
  char stemp[] = "/tmp/fileXXXXXX";
  char *ptr = mkdtemp(dtemp);
  if (ptr == NULL) {
    printf("make temp dir fail\n");
  } else {
    printf("make dir %s\n", ptr);
  }
  int fd = mkstemp(stemp);
  if (fd < 0) {
    printf("make temp file fail\n");
  } else {
    printf("make temp file %s(fd=%d)\n", stemp, fd);
  }
  close(fd);
}