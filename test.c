#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>


int main()
{
    int fd = open("test.bin", O_CREAT|O_RDWR);
    fchmod(fd, S_IRWXU|S_IRWXG|S_IRWXO);
    int seek = lseek(fd, 0, SEEK_CUR);
    printf("init seek = %d\n", seek);
    char buf[128] = {0};
    memset(buf, 0x01, sizeof(buf));
    int nwrite = write(fd, buf, sizeof(buf));
    seek = lseek(fd, 0, SEEK_CUR);
    printf("nwrite=%d seek = %d\n", nwrite, seek);

    lseek(fd, 0, SEEK_SET);
    int nread = read(fd, buf, sizeof(buf));
    printf("nread = %d\n", nread);
    nread = read(fd, buf, sizeof(buf));
    printf("nread = %d\n", nread);
    close(fd);
}