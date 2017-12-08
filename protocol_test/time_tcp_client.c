/*************************************************************************
	> File Name: time_tcp_client.c
	> Author: 
	> Mail: 
	> Created Time: 2017年11月17日 18时09分47秒 PST
 ************************************************************************/

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<time.h>
#include<unistd.h>
#include<fcntl.h>

int main(int argc, char *argv[])
{
    if(argc < 3){
        printf("usage: %s ip port\n", argv[0]);
        exit(1);
    }
    /*步骤1： 创建socket套接字*/
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd < 0){
        perror("socket error");
        exit(1);
    }
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_port = htons(atoi(argv[2]));
    serveraddr.sin_family = AF_INET;
    //将ip地址转化成网络字节序后填入serveraddr中
    inet_pton(AF_INET, argv[1], &serveraddr.sin_addr.s_addr);
    /*
    *步骤2： 客户端调用connect函数连接服务器
    */
    if(connect(sock_fd, (struct sockaddr*)&serveraddr, sizeof(serveraddr))< 0){
        perror("connect error");
        exit(1);
    }
    /*
    *步骤三： 调用io函数(read, write)与服务器进行双向通信
    */
    char buff[512];
    size_t size;
    char *prompt = ">";
    while(1){
        memset(buff, 0, sizeof(buff));
        write(STDOUT_FILENO, prompt, 1);
        size = read(STDIN_FILENO, buff, sizeof(buff));
        if(size < 0) continue;
        buff[size -1] = '\0';
        if(write(sock_fd, buff, sizeof(buff)) < 0){
           perror("write error");
           continue;
        }else{
                if(read(sock_fd, buff, sizeof(buff)) < 0){
                    perror("read error");
                    continue;
                }else{
                printf("%s\n", buff);
            }
        }
    }
    /*
    *步骤4： 关闭socket
    */
    close(sock_fd);
    return 0;
}
