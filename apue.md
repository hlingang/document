# Unix 环境高级编程 #

## 第一章 Unix 基础知识 ##

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

### 用户ID 和 组ID (/etc/passwd 口令文件) ###
1. getuid() 获取用户ID
2. getgid() 获取组ID

### Linux 系统时间度量 ###
1. 日历时间 - 自世界协调时间以来经历的秒数
2. 进程时间 - （时钟时间, 用户CPU时间, 系统CPU时间)

## 第二章 Unix 标准及实现 ##
### Linux 系统限制 ###
1. 编译期 确定的限制
2. 运行时 确定的限制

        #include <unistd.h>
        #include <stdio.h>
        #include <limits.h>
        int main()
        {
            int int_max = INT_MAX; 
            long page_size = sysconf(_SC_PAGE_SIZE);
            long name_max = pathconf("./", _PC_NAME_MAX);
            long path_max = pathconf("./", _PC_PATH_MAX);
            printf("int max=%d\n", int_max);
            printf("page size=%ld\n", page_size);
            printf("name_max=%ld\n", name_max);
            printf("path_max=%ld\n", path_max);
        }

## 第三章 文件IO ##

### 文件描述符 ###
1. 文件描述符用来唯一标识已经打开的文件
2. 系统文件描述符(STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO)
3. 通过 *open* 和 *openat* 函数来打开文件获得文件描述符
4. *open(fileName, IO_PERMISSION|IO_MODE, FILE_PERMISSION)*
5. open 函数打开文件的时候默认不会截断文件内容
6.  *off_t = long int <fcntl.h>*
7. *lseek* 函数用于设置,获取,检测文件描述符的偏移量(出错返回-1)
8. *read* 函数从指定描述符读取指定字节数(出错返回-1)
8. *write* 函数往指定描述符写入指定字节数(出错返回-1)

### 文件表项的共享 ###
1. 不同进程间可以通过共享文件表项来实现文件共享
2. 文件表项包含1) 文件状态标志 2)文件当前偏移量 3)文件v(i)节点 
3. *dup, dup2* 函数用于文件描述符的复制

### 缓冲区数据的冲洗 ###
1. *fsync(fd)* 刷新特定的文件描述符缓冲区
2. *fsyncdata(fd)* 刷新特定的文件描述符缓冲区
3. *sync(void)*  刷新所有缓冲区

### 修改文件状态标志 ###
1. *fcntl(fd, cmd, arg)*
    1) 复制文件描述符
    2) 获取/设置文件状态标志
    3) 设置文件锁(F_SETLK/F_SETLKW)
    4) 测试是否可以加锁(F_GETLK)

            #include <unistd.h>
            #include <stdio.h>
            #include <limits.h>
            #include <fcntl.h>
            #include <string.h>
            #include <sys/stat.h>
            int main()
            {
                int fd = open("test.bin", O_RDWR|O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO);
                int fd1 = fcntl(fd, F_DUPFD, 0);
                int fd2 = fcntl(fd, F_DUPFD, 5);
                int mode = fcntl(fd, F_GETFL, 0);
                printf("fd=%d fd1=%d fd2=%d\n", fd, fd1, fd2);
                fcntl(fd, F_SETFL, mode&(~O_APPEND));

                struct flock flk;
                flk.l_type = F_WRLCK; // (F_WRLCK, F_RDLCK, F_UNLCK)
                flk.l_whence = SEEK_SET;
                flk.l_start = 0;
                flk.l_len = 0;
                flk.l_pid = getpid();
                int ret = fcntl(fd, F_SETLK, &flk);
                printf("set lock ret=%d\n", ret);
                // memset(&flk, 0x00, sizeof(flk));
                fcntl(fd, F_GETLK, &flk); /// 测试是否可以加锁
                printf("flk.type=%d flk.pid=%d\n", flk.l_type, (int)flk.l_pid);
                close(fd);
            }

## 第四章 文件系统 ##

### 文件（固有）详细信息 ###
1. stat
2. fstat
3. lstat
4. fstatat

#### 文件类型 ###
1. 文件类型信息存放于 stat 结构体 的st_mode 字段
    1) *S_ISREG(st_mode)* 普通文件
    2) *S_ISDIR(st_mode)* 目录
    3) *S_ISBLK(st_mode)* 块特殊文件
    4) *S_ISCHR(st_mode)* 字符特殊文件
    5) *S_ISLNK(st_mode)* 符号链接
    6) *S_ISFIFO(st_mode)* 管道
    7) *S_ISSOCK(st_mode)* socekt 

### 文件访问权限 ### 
1. 文件访问权限信息存放于 stat 结构体 的st_mode 字段
    1) st_mode&S_IRUSR
    2) st_mode&S_IWUSR
    3) st_mode&S_IXUSR
    4) st_mode&S_IRGRP
    5) st_mode&S_IWGRP
    6) st_mode&S_IXGRP
    7) st_mode&S_IROTH
    8) st_mode&S_IWOTH
    9) st_mode&S_IXOTH
2. 文件访问权限的测试
    1) *access(file, mode)*
    2) *faccessat()*
3. 新创建文件的访问权限
    1) *umask* 新创建文件的访问权限会根据文件创建屏蔽字进行屏蔽
4. 修改文件的访问权限
    1) *chmod(file, mode)*
    2) *fchmod(fd, mode)*
    3) *fchmodat(fd, file, mode)*
5. 文件所有者的更改
    1) *chown(file, uid, gid)*
    2) *fchown(fd, uid, gid)*
    3) *lchown(file, uid, gid)*
    4) *fchown(fd, file, uid, gid, flag)*
6. 文件截断
    1) *truncate(file, length)*
    2) *ftruncate(fd, length)*
7. 文件链接(硬链接)
    1) *link(file, nefile)*
    2) *linkat(fd, file, newfd, newfile, flag)*
    1) *unlink(file)*
    2) *unlinkat(fd, file, flag)*
8. 文件重命名
    1) *rename(file, newfile)*
    2) *renameat(fd, file, newfd, newfile)*
9. 软连接和硬链接的区别
    1) 软连接是两个独立的文件(各自独立的节点), 其中一个文件保存另外一个文件的文件名
    2) 硬链接是多个文件指向同一个i节点(一个文件具有多个别名)
    3) **硬链接只能存在同一个文件系统**
    4) **只有超级用户可以创建指向目录的硬链接**
10. 符号链接的创建和读取
    1) *symlink(file, linkfile)*
    2) *symlinkat(file, fd, linkfile)*
    3) *readsymlink(file, buf, size)*
    4) *readsymlinkat(fd, file, buf, size)*
11. 文件i节点包含的信息
    1) 文件类型
    2) 文件访问权限
    3) 指向文件实际存储位置的指针
12. **目录项保存【文件名】和【i节点编号】用于文件索引**

13. 文件访问时间和修改时间的更改
    1) *futimens(fd, timespes[2])*
    2) *utimens(fd, file, timespecs[2], flag)*

### 目录操作 ###
1. 创建、删除目录
    1) *mkdir(file, mode)*
    2) *mkdirat(fd, file, mode)*
    3) *rmdir(file)*
2. 读取目录
    1) *DIR opendir(dirname)* 
    2) *dirent readdir(DIR)*
    3) *rewinddir(DIR)*
    4) *telldir(DIR)*
    5) *seekdir(DIR, offset)*

            #include <unistd.h>
            #include <stdio.h>
            #include <dirent.h>
            int main()
            {
                DIR* dp = opendir("./"); // 获取目录流对象
                struct dirent* dirp = NULL;
                while((dirp = readdir(dp))!=NULL) // 获取目录项包含的文件
                {
                    printf("dname=%s\n", dirp->d_name);    
                }
            }
3. 切换目录
    1) *chdir(file)* 只影响当前进程
    2) *fchadir(fd)* 只影响当前进程
    3) *chadirat(fd, file)* 只影响当前进程
    4) *getcwd(buf, size)* 

### 设备号 ###
1. 设备号包含主设备号(**驱动程序**)和次设备号(**特定子设备**)
2. 块特殊设备和字符特殊设备包含两个设备号, 文件系统设备号(st_dev)和实际设备号(st_rdev)

        #include <unistd.h>
        #include <stdio.h>
        #include <dirent.h>
        #include <sys/stat.h>
        #include <sys/sysmacros.h>
        int main()
        {
            DIR* dp = opendir("./"); // 获取目录流对象
            struct dirent* dirp = NULL;
            struct stat pstat;
            while((dirp = readdir(dp))!=NULL) // 获取目录项包含的文件
            {
                stat(dirp->d_name, &pstat);  
                if(S_ISCHR(pstat.st_mode)||S_ISBLK(pstat.st_mode))
                {
                    printf("%10s dev(%d/%d), rdev(%d/%d)\n", dirp->d_name, major(pstat.st_dev), minor(pstat.st_dev), major(pstat.st_rdev), minor(pstat.st_rdev));    
                }
                else
                {
                    printf("%10s dev(%d/%d)\n", dirp->d_name, major(pstat.st_dev), minor(pstat.st_dev));    
                }
            }
        }






