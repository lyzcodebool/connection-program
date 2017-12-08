/*************************************************************************
	> File Name: time_tcp_server.c
	> Author: 
	> Mail: 
	> Created Time: 2017年11月15日 18时38分32秒 PST
 ************************************************************************/

#include<stdio.h>
#include<sys/socket.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<netdb.h>
#include<memory.h>
#include<signal.h>
#include<time.h>
#include<syslog.h>


int sock_fd;

/*void sig_handler(int signo){
    if(signo == SIGINT){
        printf("server close!\n");
        close(sock_fd);
        exit(1);
    }
}*/

//输出连接上来的客户端信息
void out_addr(struct sockaddr_in* clientaddr){
    //将端口从网络字节序变为主机字节序
    int port = ntohs(clientaddr->sin_port);
    char ip[16];
    memset(ip, 0, sizeof(ip));
    
    //将ip地址从网络地址转换成点分十进制
    inet_ntop(AF_INET, &clientaddr->sin_addr.s_addr, ip, sizeof(ip));

//    printf("client: %s(#%d) connected!\n", ip, port);
    syslog(LOG_DEBUG, "%s(#%d)connected!", ip, port);
    return (void)0;
}

void do_server_task(int fd){
    //获取系统时间
    long t = time(0);
    char *s = ctime(&t);
    size_t size = strlen(s)*sizeof(char);
    //将服务器获得的系统时间发送给客户端
    if(write(fd, s, size) != size){
        //perror("write error");
        syslog(LOG_DEBUG, "error: write error!");
        exit(1);
    }
    return (void)0;
}


int main(int argc, char *argv[])
{
    if(argc < 2){
        printf("usage: %s #port\n", argv[0]);
        exit(1);
    }
    
    /*
    *加入守护进程，让服务器在后台执行，消息记录存入系统日志文件中(/var/log/syslog)
    */

    /*步骤一：创建屏蔽字为0*/
    umask(0);
    /*步骤二：创建子进程，关闭父进程*/
    pid_t pid;
    pid = fork();
    if(pid > 0){exit(1);}
    /*步骤三：调用setsid函数创建一个新的会话*/
    setsid();
    /*步骤四：将当前目录改为根目录*/
    chdir("/");
    /*步骤五：关闭不必要的文件描述符*/
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    /*步骤六：将输出到中断的内容重写进系统日志文件中，调用syslog函数*/

    /*if(signal(SIGINT,sig_handler) == SIG_ERR){
        perror("signal sigint error");
        exit(1);
    }*/
    openlog(argv[1], LOG_PID, LOG_SYSLOG);
    /*
    *创建socket套接字
    *socket 创建在内核中， 是一个结构体。
    *AF_INET： IPV4
    *SOCK_STREAM： tcp协议
    * */
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd < 0){
        //perror("socket error");
        syslog(LOG_DEBUG, "error: %s", strerror(sock_fd));
        exit(1);
    }

    /*
    *调用bind函数将socket和地（ip、port）址进行绑定
    */

    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    //往地址中填入ip/port/internet的地址族类型
    serveraddr.sin_family = AF_INET;//IPV4
    serveraddr.sin_port = htons(atoi(argv[1]));//port
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    if(bind(sock_fd, (struct sockaddr*)&serveraddr, sizeof(serveraddr))< 0){
        //perror("bind error");
        syslog(LOG_DEBUG, "error: %s", strerror(sock_fd));
        exit(1);
    }

    /*
    *步骤三： 调用listen函数启动监听(指定port监听)
    *作用： 通知系统接受请求连接的客户端
    *(将接收到的连接请求放置到连接队列中)
    *第二个参数是队列长度
    */
    
    if(listen(sock_fd, 10)< 0){
        //perror("listen error");
        syslog(LOG_DEBUG, "error: listen error!");
        exit(1);
    }

    /*
    *步骤4： 调用accept函数从队列中获得一个客户端的连接请求 
    *       并返回一个新的socket描述符
    *注意：若没有客户端连接，调用次函数后会阻塞。
    *      直到获得一个客户端的连接
    */
    struct sockaddr_in clientaddr;
    int  clientaddr_len = sizeof(clientaddr);
    while(1){     
        int fd = accept(sock_fd, (struct sockaddr *)&clientaddr, &clientaddr_len);
        if(fd < 0){
            //perror("accept error");
            syslog(LOG_DEBUG, "accept error ");
            continue;
        }
        /*
        *步骤5: 调用io函数(read/write)和连接的客户端进行双向通信
        */

        out_addr(&clientaddr);
        do_server_task(fd);

        /*
        *步骤6： 关闭套接字
        */
        close(fd);
        
    }

    return 0;
}
