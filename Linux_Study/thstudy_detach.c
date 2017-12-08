/*************************************************************************
	> File Name: thstudy_detach.c
	> Author: 
	> Mail: 
	> Created Time: 2017年11月10日 21时51分16秒 PST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

void *th_fn(void *arg){
    int *temp = (int *)arg;
    for(int i = 0; i < 20; ++i){
        printf("0X%lx IM %d thread\n", pthread_self(), i);
    }
    return (void *)0;
}

int main(void)
{
    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    if(pthread_create(&tid, &attr, th_fn, (void *)0) < 0){
        perror("create thread error");
        exit(1);
    }
    pthread_attr_destroy(&attr);
    printf("0x%lx main thread end\n", pthread_self());
    sleep(1);
    exit(0);
}
