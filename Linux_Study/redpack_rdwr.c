/*************************************************************************
	> File Name: redpack_rdwr.c
	> Author: 
	> Mail: 
	> Created Time: 2017年11月09日 21时25分17秒 PST
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
/*
*多线程抢红包，使用读写线程，条件变量
*/
typedef struct{
    int  pack_num;
    int  pack_money;

    pthread_mutex_t mutex;
    pthread_cond_t cond;
    //设置分离属性
    pthread_attr_t attr;
}Redpack;

void *th_func(void *arg){
    Redpack *temp = (Redpack *)arg;

    pthread_mutex_lock(&temp->mutex);
    pthread_cond_wait(&temp->cond, &temp->mutex);
    pthread_mutex_unlock(&temp->mutex);
    printf("0X%lx thread cread success\n", pthread_self());



    return (void *)0; 
}

int main(void)
{
    Redpack *red_pack;
    pthread_mutex_init(&red_pack->mutex, NULL);
    pthread_cond_init(&red_pack->cond, NULL);
    pthread_attr_init(&red_pack->attr);
    

    pthread_t th_tid;
    int number;
    int money;

    //create 20 thread;
    for(int i = 0; i < 20; ++i){
        pthread_create(&th_tid, &red_pack->attr, th_func, (void *)red_pack);
        printf("I am %d thread\n", i);
    }
    while(1)
    {
        printf("please input the pack_num:\n");
        if(fscanf(stdin, "%d", &number) == EOF)break;
        printf("please input the pack_money\n");
        if(fscanf(stdin, "%d", &money) == EOF)break;
    
    //lock 
        pthread_mutex_lock(&red_pack->mutex);
    
        red_pack->pack_num = number;
        red_pack->pack_money = money;

        pthread_cond_broadcast(&red_pack->cond);
        pthread_mutex_unlock(&red_pack->mutex);

    }
    
    pthread_mutex_destroy(&red_pack->mutex);
    pthread_cond_destroy(&red_pack->cond);
    pthread_attr_destroy(&red_pack->attr);
    exit(0);
}
