#include <stdio.h>
#include <sys/time.h>
#include <sys/utsname.h>
#include <time.h>
#include <unistd.h>

int main() {
    struct tm *tmptr;
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