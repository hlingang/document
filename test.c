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