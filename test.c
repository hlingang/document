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