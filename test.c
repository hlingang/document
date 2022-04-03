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