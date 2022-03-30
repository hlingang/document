# Unix 环境高级编程 #

## 第一章 ##

### 不带缓冲的IO接口 ###
1. open
2. close
3. lseek
5. read
6. write

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

### errno.h ###
1. errno 支持多线程
2. errno 在出错时被赋值，其值不会自动清除

### 进程处理函数 ###
1. fork 创建进程
2. exec 执行命令
3. wait/waitpid 等待子进程结束

### 进程间通过信号机制进行通信 ###
1. 当向某个进程发生信号时，我们必须是特定进程的所有者或者是超级用户
2. 不能忽略和捕获的信号(SIGSTOP, SIGKILL)

### 用户ID 和 组ID (/etc/passwd 口令文件)###
1. getuid() 获取用户ID
2. getgid() 获取组ID

### Linux 系统时间度量 ###
1. 日历时间 - 自世界协调时间以来经历的秒数
2. 进程时间 - （时钟时间, 用户CPU时间, 系统CPU时间)