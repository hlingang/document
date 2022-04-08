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