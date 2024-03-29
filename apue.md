# Unix 环境高级编程 

## 第一章 Unix 基础知识 

### 不带缓冲的IO接口 
1. open
2. close
3. lseek
5. read
6. write
```
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
```

### errno.h 
1. errno 支持多线程
2. errno 在出错时被赋值，其值不会自动清除

### 进程处理函数 
1. fork 创建进程
2. exec 执行命令
3. wait/waitpid 等待子进程结束

### 进程间通过信号机制进行通信 
1. 当向某个进程发生信号时，我们必须是特定进程的所有者或者是超级用户
2. 不能忽略和捕获的信号(SIGSTOP, SIGKILL)

### 用户ID 和 组ID (/etc/passwd 口令文件) 
1. getuid() 获取用户ID
2. getgid() 获取组ID

### Linux 系统时间度量 
1. 日历时间 - 自世界协调时间以来经历的秒数
2. 进程时间 - （时钟时间, 用户CPU时间, 系统CPU时间)

## 第二章 Unix 标准及实现 
### Linux 系统限制 
1. 编译期 确定的限制
2. 运行时 确定的限制
```
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
```

## 第三章 文件IO 

### 文件描述符 
1. 文件描述符用来唯一标识已经打开的文件
2. 系统文件描述符(STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO)
3. 通过 *open* 和 *openat* 函数来打开文件获得文件描述符
4. *open(fileName, IO_PERMISSION|IO_MODE, FILE_PERMISSION)*
5. open 函数打开文件的时候默认不会截断文件内容
6.  *off_t = long int <fcntl.h>*
7. *lseek* 函数用于设置,获取,检测文件描述符的偏移量(出错返回-1)
8. *read* 函数从指定描述符读取指定字节数(出错返回-1)
8. *write* 函数往指定描述符写入指定字节数(出错返回-1)

### 文件表项的共享 
1. 不同进程间可以通过共享文件表项来实现文件共享
2. 文件表项包含1) 文件状态标志 2)文件当前偏移量 3)文件v(i)节点 
3. *dup, dup2* 函数用于文件描述符的复制

### 缓冲区数据的冲洗 
1. *fsync(fd)* 刷新特定的文件描述符缓冲区 **刷新内核缓冲区(从内核缓冲区写入物理媒介)**
2. *fsyncdata(fd)* 刷新特定的文件描述符缓冲区  **刷新内核缓冲区(从内核缓冲区写入物理媒介)**
3. *sync(void)*  刷新所有缓冲区  **刷新内核缓冲区(从内核缓冲区写入物理媒介)**

### 修改文件状态标志 
1. *fcntl(fd, cmd, arg)*
    1) 复制文件描述符
    2) 获取/设置文件状态标志
    3) 设置文件锁(F_SETLK/F_SETLKW)
    4) 测试是否可以加锁(F_GETLK)
```
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
```

## 第四章 文件系统 

### 文件（固有）详细信息 
1. stat
2. fstat
3. lstat
4. fstatat

#### 文件类型 
1. 文件类型信息存放于 stat 结构体 的st_mode 字段
    1) *S_ISREG(st_mode)* 普通文件
    2) *S_ISDIR(st_mode)* 目录
    3) *S_ISBLK(st_mode)* 块特殊文件
    4) *S_ISCHR(st_mode)* 字符特殊文件
    5) *S_ISLNK(st_mode)* 符号链接
    6) *S_ISFIFO(st_mode)* 管道
    7) *S_ISSOCK(st_mode)* socekt 

### 文件访问权限  
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

### 目录操作 
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
    ```
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
    ```
3. 切换目录
    1) *chdir(file)* 只影响当前进程
    2) *fchadir(fd)* 只影响当前进程
    3) *chadirat(fd, file)* 只影响当前进程
    4) *getcwd(buf, size)* 

### 设备号 
1. 设备号包含主设备号(**驱动程序**)和次设备号(**特定子设备**)
2. 块特殊设备和字符特殊设备包含两个设备号, 文件系统设备号(st_dev)和实际设备号(st_rdev)
```
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
```

### 粘着位 
粘着位可以使程序运行结束后正文段仍然保留在交换区(swap), 后续运行时无需重复加载

## 第五章 标准 IO 库 

### 文件流对象（FILE 对象) 
1. 一个FILE 对象至少包含: 指向特定文件的描述符, 缓冲区地址
2. 文件流的定向
    #### *fwide(FILE, mode)* 
    1) mode < 0: 设置位字节定向
    2) mode > 0: 设置位宽定向
    3) mode = 0: 不设置
3. 标准输入输出流对象(stdio.h)
    1) stdin
    2) stdout
    3) stderr
### 缓冲 
1. 三种缓冲类型
    1) 全缓冲 _IOFBF
    2) 行缓冲 _IOLBF
    3) 无缓冲 _IONBF
2. 改变缓冲类型
    1) *setbuf(FILE, buf)* buffer 长度为预定义的 **BUFSIZ**
    2) *setvbuf(FILE, buf, mode, size)*
3. 冲洗缓冲区
    1) *fflush(FILE)* **若FILE指向NULL, 则刷新所有输出流 刷新用户缓冲区(从用户缓冲区写入内核缓冲区)**
4. 打开流对象
    1) *FILE fopen(file, mode)*
    2) *FILE freopen(file, mode, oldfile)*
    3) *FILE fdopen(fd, mode)*
    4) *int fclose(FILE \*fp)*  **关闭流**
    ```
    mode 参数详解
    1. r/rb: O_IRD
    2. w/wb: O_IWR
    3. r+/rb+: O_RDWR
    4. w+/wb+: O_RDWR|O_TRUNC|O_CREAT
    5. a+/ab+: O_RDWR|O_APPEND|O_CREAT
    6. **在fd上打开的时候无法截断文件**
    ```

### 流对象的读写 
1. 单字节 IO 读写
    1) *getc(FILE fp)*  **宏定义**
    2) *fgetc(FILE fp)*  **函数**
    3) *getchar(void)*
    4) *put(c, FILE fp)*  **宏定义**
    5) *fput(c, FILE fp)*  **函数**
    6) *putchar(c)*
    7) *unget(c)*
2. 文件尾端的判断
    1) *ferror(FILE fp)*  **判断是否出错**
    2) *feof(FILE fp)*  **判断是否到达文件尾端**

3. 行 IO 读写
    1) *fgets(buf, size, FILE fp)* **buf必须以null结尾**
    2) *fputs(buf, FILE fp)* **buf必须以null结尾**
    3) *gets(buf)*  **不推荐使用**
    4) *puts(buf)*  **不推荐使用**

4. 二进制 IO 
    1) *fwrite(buf, objsize, objcount, FILE fp)* **返回objcount**
    2) fread(buf, objsize, objcount, FILE fp)* **返回objcount**

### 流的定位 
1. *long ftell(FILE fp)*  **返回fp当前偏移量**
2. *int fseek(FILE fp, long offset, int whence)*
3. *frewind(fp)*  **重置fp**

### 格式化 IO 
1. *sprintf(buf, format, ##__VA_ARGS__)*
1. *snprintf(buf, size, format, ##__VA_ARGS__)*
3. *printf(format, ##__VA_ARGS__)*

### 创建临时文件 
1. *char \*tmpnam(char \*ptr)*
2. *FILE \*tmpfile(void)*
3. *char\* mkdtemp(char \*template)*  **按pattern创建临时目录(推荐使用)**
4. *int mkstemp(char \*template)*  **按pattern创建临时文件(推荐使用)**
```
#include <pwd.h>
#include <stdio.h>
#include <unistd.h>
int main() {
    schar dir[] = "/dev/shm/dirXXXXXX";
    char file[] = "/dev/shm/fileXXXXXX";
    char *ptr = mkdtemp(dir);
    int fd = mkstemp(file); //创建文件并打开
    printf("%s:\n", dir);
    printf("%s:\n", file);
    printf("fd:%d\n", fd);
    close(fd);
}
```

## 第六章 系统数据文件和信息 

### 获取用户详细信息 <passwd.h> 
1. struct passwd\* *getpwuid(uid)*  **通过uid 获取对应的口令文件详细信息**
2. struct passwd\* *getpwnam(char \*name)*  **通过user name获取对应的口令文件详细信息**
```
#include <pwd.h>
#include <stdio.h>
#include <unistd.h>
int main() {
struct passwd *pw;
pw = getpwuid(0);
if (pw == NULL) {
    printf("get pw by uid fail\n");
    return -1;
} else {
    printf("pw-name:%s(%d:%d)\n", pw->pw_name, pw->pw_uid, pw->pw_gid);
}
pw = getpwnam("huanglingang");
if (pw == NULL) {
    printf("get pw by name fail\n");
    return -1;
} else {
    printf("pw-name:%s(%d:%d)\n", pw->pw_name, pw->pw_uid, pw->pw_gid);
}
}
```

### 获取操作系统详细信息 <sys/utsname.h> 
1. *int uname(struct utsname\* name)*

### 时间和日期的获取 
1. *time_t time(time_t \*time_ptr)*  **日历时间<time.h>**
2. *clock_gettime(CLOCK_ID, timespec \*tsp)* **日历时间<sys/time.h>**
3. *gettimeofday(struct timeval \*tp, NULL)*  **不推荐使用**

### 日历时间和分解时间的转换 
1. *struct tm\* localtime(time_t \*calptr)*
2. *struct tm\* gmtime(time_t \*calptr)*
3. *time_t mktime(struct tm\* tmptr)*

### 时间和日期的格式化 
1. *strftime(buf, size, format, struct tm\* tmptr)*
```
#include <stdio.h>
#include <sys/time.h>
#include <sys/utsname.h>
#include <time.h>
#include <unistd.h>
int main() {
    struct tm* tmptr;
    time_t caltime;
    struct timeval caltimeval;
    struct timespec caltimespec;
    caltime = time(NULL);
    time(&caltime);
    gettimeofday(&caltimeval, NULL);
    clock_gettime(CLOCK_REALTIME, &caltimespec);
    printf("caltime01=%ld\n", caltime);
    tmptr = localtime(&caltime);
    char buf[128] = {0};
    strftime(buf, sizeof(buf), "%Y:%m:%d:%H:%M:%S(%A %B)", tmptr);
    caltime = mktime(tmptr);
    printf("date:%s\n", buf);
    printf("caltime02=%ld\n", caltime);
}
```

## 第七章 进程环境 

### 进程退出前的清理工作 
1. *int atexit(void (\*func)(void))*;
    1) 注册进程退出前的清理函数
    2) 调用顺序与注册顺序相反
    3) 同一函数可以登记多次(也会调用多次)
### 全局环境变量 
1. *extern char\* environ*
2.  环境变量以key=value 的字符串形式存在
3. 环境变量的操作
    1) *char\* getenv(char \*)*
    2) *int putenv(char \*)*
    ```
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    int main() {
        char *ptr = getenv("PATH");
        printf("PATH:%s\n", ptr);
        putenv("TESTPATH=./");
        ptr = getenv("TESTPATH");
        printf("TESTPATH:%s\n", ptr);
    }
    ```

### 程序存储空间模型 
1. 正文段(机器指令, 表现为共享, 只读) **低地址**
2. 初始化数据段(函数外显式初始化全局变量)
2. 未初始化数据段(未显式初始化全局变量, 内核负责初始化)
3. 堆空间
4. 栈空间  **高地址**

### 动态空间分配 
1. *void\* malloc(size)*
2. *void\* calloc(nobj, objsize)*
3. *void\* realloc(void\* ptr, size)*
4. void free(void\* ptr)*

### 进程资源限制的调整 <sys/resource.h> 
1. *int getrlimit(RESOURCE, struct rlimit\* rptr)*
2. *int setrlimit(RESOURCE, struct rlimit\* rptr)*
```
struct rlimit
{
    rlim_t rlim_cur; //soft limit(rlim_t=lu)
    rlim_t rlim_max; //hard limit(rlim_t=lu)
}
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>
int main() {
    struct rlimit rlimits;
    getrlimit(RLIMIT_CPU, &rlimits);
    printf("soft limit:%lu\n", rlimits.rlim_cur);
    printf("hard limit:%lu\n", rlimits.rlim_max);
}
```

## 第八章 进程控制 

### 进程标识 
1. 交换进程 ID=0, 属于内核系统进程
2. init 进程 ID=1, 属于普通用户进程, 负责启动系统服务
3. 获取进程标识
    1) *pid_t getpid()*
    2) *pid_t getppid()*
    3) *uid_t getuid()*
    4) *uid_t getpuid()*
    5) *gid_t getgid()*

### 创建进程 
1. *pid_t fork()*  **创建子进程, 返回子进程ID

### 父进程和子进程间的文件共享 
1. 父进程和子进程共享相同的文件表项
2. 父进程和子进程共享相同的文件偏移量
3. 父进程的重定向会同时反映到子进程

### 进程的退出 
1. exit 退出进程时返回进程的退出状态码, 同时刷新所有缓冲流

### 等待进程结束<sys/wait> 
1. *pid_t wait(int \*statptr)* **阻塞方式等待任意进程结束**
2. *pid_t waitpid(pid, int \*statptr, int option)* **阻塞或非阻塞方式(option)等待特定进程结束**

### 执行程序 
1. *execl* **列表方式传参**
    1) *execl(path, arg0, arg1,..., (void\*)0)*
    2) *execlp(file, arg0, arg1,..., (void\*)0)*
    3) *execle(path, arg0, arg1,..., (void\*)0, char\* env[])*
2. *execv* **数组方式传参**
    1) *execv(path, char\* argv[])*
    2) *execvp(file, char\* argv[])*
    3) *execve(path, char\* argv[], char\* env[])*

### 更改用户id和组id 
1. *int setuid(uid_t uid)*
2. *int setgid(gid_t gid)*

### system 函数 
1. *int system(char\* cmd)* **先调用fork创建子进程, 然后执行程序**

### 获取登录用户名 
1. *char \*getlogin(void)* **返回当前登录用户名**

### 进程调度 
1. *int nice(int incr)*  **incr为nice值得增量, 返回新的nice值**

### 进程时间 <sys/times.h> 
1. *clock_t times(tms\* tmsptr)*
```
struct tms
{
    clock_t tms_utime; 用户CPU时间
    clock_t tms_stime; 系统CPU时间
    clock_t tms_cutime; 已结束子进程用户CPU时间
    clock_t tms_cstime; 已结束子进程系统CPU时间
}
```

## 第九章 进程关系 

### 获取进程组ID 
1. *pid_t getpgrp(void)*
2. *pid_t getgpid(pid_t pid)*  **pid=0时， 返回调用进程的进程组ID**

### 孤儿进程 
1. 父进程已经终止的子进程, 会被init进程收养, 成为孤儿进程

## 第十章 信号 

### Unix 信号的概念 
1. 信号属于软中断, 用于进程间的通信

### 信号处理的方式 
1. 忽略信号(SIGKILL, SIGSTOP除外)
2. 捕获信号(用户定义信号处理行为, SIGKILL,SIGSTOP除外)
3. 默认处理(终止进程)

### 信号注册函数 
1. *void (\*signal(SIGNO, void (\*func)(int)))(int)*  **返回handler 函数指针**

### 信号捕获的限制 
1. 信号捕获函数仅仅在当前进程的当前程序里面有效
2. fork 创建的子进程会继承父进程的信号处理方式

### 信号的阻塞 
1. 当信号发生时, 并不忽略此信号, 但是等到系统准备好了再来处理此信号
2. 信号屏蔽字决定进程需要阻塞的所有信号

### 信号发送 
1. *kill(pid, signo)* **向特定进程发送信号**
    1) pid > 0, 发送给指定的pid进程
    1) pid = 0, **发送当前进程组的所有进程**
    2) pid = -1, **发送给所有进程**
    3) pid < -1, 发送给进程组绝对值为pid的所有进程
2. *raise(signo)*  **向当前进程发送信号**
    1) raise(signo) = kill(getpid(), signo)

### 闹钟和休眠 
1. *unsigned int alarm(secons)*
    1) 该函数返回 0 或者之前的闹钟余留的时间秒数
2. *int pause(void)* **返回-1**
    1) 进程陷入休眠, 并阻塞, 直到进程收到任意信号后并处理完成后返回(-1, errno=EINTR)

### 信号集 和 信号屏蔽 
1. *sigemptyset(sigset_t\* sigset)* **清空信号集**
2. *sigfillset(sigset_t\* sigset)* **默认初始化信号集**
3. *sigaddset(sigset_t\* sigset, int signo)*
4. *sigdelset(sigset_t\* sigset, int signo)*
5. *int sigprocmask(int how, sigset_t\* set, sigset_t\* oldset)*
    1) how=SIG_BLOCK 阻塞信号
    2) how=SIG_UNBLOCK  解除阻塞信号
    3) how=SIG_SETMASK 重新设置信号屏蔽字

### 获取屏蔽信号集 
1. *int sigpending(sigset_t\* set)*

### 阻塞等待特定信号发生 
1. *int sigsuspend(sigset_t set)* **捕获到特定信号后返回-1**
    1) 设置新的信号屏蔽字并且等待特定信号, 返回前设置原来的信号屏蔽字

### abort 函数 
1. *void abort(void)* **向调用进程发送SIGABORT 信号
2. 等效于 *raise(SIGABORT)*

### sleep 函数 
1. *unsigned int sleep(unsigned int seconds)* **返回0或者未休眠完的时间秒数
2. *unsigned int usleep(unsigned int us)* **返回0或者未休眠完的时间微秒数
3. *int nanosleep(timespec \*ptr, timespec\* temptr)* **返回0或者-1, temptr 指向未休眠的时间数
4. *int clock_nanosleep(clock_id, flag, timespec \*ptr, timespec\* temptr)

### 信号的打印 
1. *void psignal(signo, msg)* **类似于 perror
2. *void psiginfo(const siginfo_t \*info, const char \*msg)*
3. char \*strsignal(signo)*

## 第十一章 线程 

### 线程的概念 
1. 线程是微进程
2. 同一进程的所有线程共享相同的进程空间地址
3. 多线程的特征: **按事件类别进行异步处理** 
4. 即使在单处理器上也能发挥优势

### 线程id操作 
1. 线程id比较: *pthread_equal(pthread_t t1, pthread_t t2)*
2. 获取当前线程id: *pthread_self()*

### 创建线程 
1. pthread_create(\*thread_id, \*attr, (void\*)callback, void \*args)*

### 线程的终止及退出状态的访问 
1. *pthread_exit(void \*status_ptr)*
2. *pthread_join(tid, \*status_ptr)*

### 请求取消线程 
1. *pthread_cancel(tid)* **请求取消同一进程的某个线程(不等待线程取消)**

### 线程退出时的清理 
1. *pthread_cleanup_push(void (func\*)(void \*args), void\* args)* **注册**
2. *void pthread_cleanup_pop(int execute)* **清理注册**
```
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void clean() { printf("pthread clean\n"); }
void *func(void *args) {
    pthread_cleanup_push(clean, NULL);
    printf("pthread start\n");
    sleep(5);

    printf("pthread exit\n");
    pthread_exit(NULL);     // 退出线程
    pthread_cleanup_pop(0); //清理注册(一定要在线程退出后)
}
int main() {
    pthread_t tid;
    pthread_create(&tid, NULL, func, NULL);
    pthread_join(tid, NULL);
}
```

### 互斥量(同步锁mutex) 
1. 避免死锁的条件 **所有线程遵循尽量遵循相同资源(互斥量)访问顺序**
2. *pthread_mutex_init(\*mutex, attr)*
3. *pthread_mutex_destory(\*mutex)*

### 互斥量加锁 
4. *pthread_mutex_lock(\*mutex)*
5. *pthread_mutex_trylock(\*lock)* **非阻塞加锁**
5. *pthread_mutex_timedlock(\*lock, \*timespec)* **timeout时间前(绝对时间)阻塞加锁**
6. *pthread_mutex_unlock(\*lock)*

### 读写锁 
1. 读写锁允许多种加锁状态, 提供更多同步状态控制(读锁/写锁/无锁)，**读锁和写锁互斥**
2. *pthread_rwlock_init(\*rwlock, pthread_rwlockattr_t\* attr)*
3. *pthread_rwlock_rdlock(\*rwlock)*
4. *pthread_rwlock_wrlock(\*rwlock)*
5. *pthread_rwlock_timedrdlock(\*rwlock, \*timespec)*
6. *pthread_rwlock_timedwrlock(\*rwlock, \*timespec)*
7. *pthread_rwlock_unlock(\*rwlock)*
```
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    pthread_rwlock_t rwlock;
    pthread_rwlock_init(&rwlock, NULL);
    pthread_rwlock_rdlock(&rwlock);
    sleep(1);

    int ret = pthread_rwlock_rdlock(&rwlock); // 不阻塞
    // pthread_rwlock_wrlock(&rwlock); // 阻塞
    printf("ret=%d\n", ret);
}
```

### 条件变量 
1. 条件变量必须和互斥量一起使用, 实现无竞争的同步
2. *pthread_cond_init(\*pthread_cond_t, \*pthread_condattr_t)*
3. *pthread_cond_wait(\*pthread_cond_t, \*mutex)*
4. *pthread_cond_wait(\*pthread_cond_t, \*mutex)*
5. *pthread_cond_timedwait(\*pthread_cond_t, \*mutex, \*timespec)* 
**timeout 时间为绝对时间**
6. *pthread_cond_signal(\*pthread_cond_t)* **通知其中一个线程**
7. *pthread_cond_broadcast(\*pthread_cond_t)* **通知所有线程**
8. *pthread_cond_destroy(\*pthread_cond_t)*

### 自旋锁 
1. 自旋锁在等待的时候不会休眠, 而是一直处于忙等状态(自旋状态)

### 屏障 
1. 用于等待多个其他线程完成某一个动作(由线程计数参数指定)
2. *pthread_barrier_init(\*pthread_barrier_t, pthread_barrierattr_t \*attr, unsigned int count)* **等待的线程数**
3. *pthread_barrier_wait(pthread_barrier_t \*barrier)*
4. *pthread_barrier_destroy(pthread_barrier_t \*barrier)*


## 第十二章 线程属性 

### 线程属性 
1. *pthread_attr_init(pthread_attr_t \* attr)*
2. *pthread_attr_getdetachstate(pthread_attr_t \* attr, int \*detachstate)*
3. *pthread_attr_setdetachstate(pthread_attr_t\* attr, int \*detachstate)*
4. *pthread_attr_destroy()*

### 同步属性 

#### 互斥量属性  
1. *pthread_mutexattr_init(pthread_mutexattr_t\*)*

2. *pthread_mutexattr_gettype(pthread_mutexattr_t\*, int \*type)* **类型属性**
3. *pthread_mutexattr_settype(pthread_mutexattr_t\*, int type)* **类型属性**

4. *pthread_mutexattr_getpshared(pthread_mutexattr_t\*, int \*pshared)* **进程共享属性**
5. *pthread_mutexattr_getpshared(pthread_mutexattr_t\*, int pshared)* **进程共享属性**

6. *pthread_mutexattr_getrobust(pthread_mutexattr_t\*, int \*robust)* **健壮性属性**
7. *pthread_mutexattr_setrobust(pthread_mutexattr_t\*, int robust)* **健壮性属性**

8. *pthread_mutexattr_destroy()*

#### 读写锁属性 
1. 读写锁属性支持唯一的属性 -> 进程共享属性

#### 条件变量属性 
1. 进程共享属性
2. 时钟属性

#### 屏障属性 
1. 屏障属性支持唯一的属性 -> 进程共享属性

### 线程信号屏蔽 
1. *pthread_sigmask(how, sigset, oldset)* 
    1) **SIG_BLOCK** 添加屏蔽信号
    2) **SIG_SETMASK** 重新设置屏蔽信号
    3) **SIG_UNBLOCK** 删除屏蔽信号
2. *sigwait(sigset, int\* sigcount)*

### 向特定线程发送信号 
1. *pthread_kill(tid, signo)*


## 第十二章 守护进程 

### 系统守护进程 
1. 守护进程没有控制终端, 时钟在后台运行, 直到系统关闭
2. 常见的系统守护进程
    1) 缓存刷新
    2) 系统log记录
    3) 系统定时任务
3. 特殊的守护进程
    1) 0 进程: 内核进程
    2）1 进程(init)进程, 用户进程, 负责启动其他服务

### 文件锁<stdio.h> 
1. *int flockfile(FILE\* fd)*
2. *int ftrylockfile(FILE\* fd)*
3. *int funlockfile(FILE\* fd)*



