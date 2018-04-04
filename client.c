#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define aws_tcp_port 25356

int main(int argc ,char* argv[]){
    // first we need to check the input
    if(argc != 3){printf("please input a function and a number only!\n");exit(0);}
    if(strcmp(argv[1],"LOG") && strcmp(argv[1],"DIV")){
        printf("wrong function!!\n");
        exit(0);
    }
    double in = atof(argv[2]);
    if(in >= 1 || in <= -1){
        printf("please input a number < 1 and > -1!\n");
        exit(0);
    }
    // set up the socket
    int socket2 = socket(AF_INET,SOCK_STREAM,0);
    if(socket2 == -1){
        perror("failed to create TCP socket");
        exit(1);
    }
    // it is the address of the aws (TCP)
    struct sockaddr_in aws_addr;
    memset(&aws_addr,0,sizeof(aws_addr));
    aws_addr.sin_family = AF_INET;
    aws_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    aws_addr.sin_port = htons(aws_tcp_port);
    // then connect
    if (connect(socket2,(struct sockaddr*)&aws_addr,sizeof(aws_addr)) == -1){
        perror("failed to connect");
        exit(1);
    }
printf("The client is up and running.\n");
//    while(1){
        if(send(socket2,argv[1],10,0) == -1){
            perror("failed to send function");
            exit(1);
        }
        //printf("send %s\n",argv[1]);
        if(send(socket2,argv[2],10,0) == -1){
            perror("failed to send number");
            exit(1);
        }
        //printf("send %s\n",argv[2]);
printf("The client sent <%s> and %s to AWS\n",argv[2],argv[1]);

//    }
//we have sent the data, and we can receive the final result now
    double result[10];
    if(recv(socket2,result,10,0) == -1){
        perror("failed to receive the result");
        exit(1);
    }
printf("According to AWS, %s on <%s>: <%lf>\n",argv[1],argv[2],result[0]);
    close(socket2);
    return 0;
}

