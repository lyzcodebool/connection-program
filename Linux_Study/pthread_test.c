/*************************************************************************
	> File Name: pthread_test.c
	> Author: 
	> Mail: 
	> Created Time: 2017年11月08日 22时04分53秒 PST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<pthread.h>

/*
*线程的综合运用
*包括线程属性，互斥锁，读写锁，
*条件变量的使用

*案例:抢票系统
*/
typedef struct{
    //剩余的票的数量
    int ticket_num;
    //互斥锁
    pthread_mutex_t mutex;
    pthread_cond_t cond;

}Ticket;

void *th_func(void *arg){
//线程运行函数
    Ticket *th_temp = (Ticket *) arg;
    printf("I am the 0X%lx thread\n", pthread_self());
    printf("left %d tickets at the current\n", th_temp->ticket_num);

    pthread_mutex_init(&th_temp->mutex, NULL);
    pthread_cond_init(&th_temp->cond, NULL);
    //rand create one person get tickets number
    srand((unsigned) time(NULL));
    int rand_num = (int)rand()%100;
    //lock
    pthread_mutex_lock(&th_temp->mutex);

    th_temp->ticket_num -= rand_num;

    //unlock
    pthread_mutex_unlock(&th_temp->mutex);
    
    //destroy lock
    pthread_mutex_destroy(&th_temp->mutex);

    return (void *)0;
}

int main(void)
{
    Ticket temp;
    temp.ticket_num = 0;
    printf("please input the tickets num: \n");
    scanf("%d", &temp.ticket_num);
    /*if(temp.ticket_num){
        
    }*/
    pthread_t th_tid;
    //20 threads at same time
    for(int i = 0; i < 20; ++i){
        if(pthread_create(&th_tid, NULL, th_func, (void *)&temp) != 0){
            perror("pthread create error");
            exit(1);
        }else{
            printf("%d ----", i);
        }
    }

    pthread_join(th_tid, NULL);
    


    exit(0);
}
