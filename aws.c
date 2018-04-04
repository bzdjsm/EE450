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

#define ServerA_port 21356
#define ServerB_port 22356
#define ServerC_port 23356
#define fclient_port 24356
#define aws_tcp_port 25356
int main(){
    // first create a socket to receive data from client
    int socket3 = socket(AF_INET,SOCK_STREAM,0);
    if(socket3 == -1){
        perror("failed to create TCP socket");
        exit(1);
    }
    struct sockaddr_in aws_addr;
    memset(&aws_addr,0,sizeof(aws_addr));
    aws_addr.sin_family = AF_INET;
    aws_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    aws_addr.sin_port = htons(aws_tcp_port);
    // bind the socket
    if(bind(socket3,(struct sockaddr*)&aws_addr,sizeof(aws_addr)) == -1){
        perror("failed to bind socket");
        exit(1);
    }
    // now listen
    if(listen(socket3,5) == -1){
        perror("having problems when listening");
        exit(1);
    }
    // create an address represents the client address
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    // create socket for sending the number, error occured if return -1
    int socket1 = socket(AF_INET,SOCK_DGRAM,0);
    if (socket1 == -1){
        perror("failed to create socket");
        exit(1);
    }
    struct sockaddr_in  fakeclient_addr;
    memset(&fakeclient_addr,0,sizeof(fakeclient_addr));
    fakeclient_addr.sin_family = AF_INET;
    fakeclient_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    fakeclient_addr.sin_port = htons(fclient_port);
    // bind the socket
    if(bind(socket1,(struct sockaddr*)&fakeclient_addr,sizeof(fakeclient_addr)) == -1){
        perror("failed to bind socket");
        exit(1);
    }

    // create serverA address
    struct sockaddr_in  serverA_addr;
    memset(&serverA_addr,0,sizeof(serverA_addr));
    serverA_addr.sin_family = AF_INET;
    serverA_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverA_addr.sin_port = htons(ServerA_port);
// create serverB address
    struct sockaddr_in  serverB_addr;
    memset(&serverB_addr,0,sizeof(serverB_addr));
    serverB_addr.sin_family = AF_INET;
    serverB_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverB_addr.sin_port = htons(ServerB_port);
// create serverC address
    struct sockaddr_in  serverC_addr;
    memset(&serverC_addr,0,sizeof(serverC_addr));
    serverC_addr.sin_family = AF_INET;
    serverC_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverC_addr.sin_port = htons(ServerC_port);

//    char input_number[10];
//    printf("input a float number \n");
//    scanf("%s",input_number);
// At first I only tried to create three servers and the UDP side of the aws(I called it fakecilent), the code above is used to input the number
printf("The AWS is up and running\n");
while(1){
    double result_A[10];
    double result_B[10];
    double result_C[10];

        int socket_accept = accept(socket3,(struct sockaddr*)&client_addr,&client_len);
        if(socket_accept == -1){
            perror("failed to accept");
            exit(1);
        }
        char function[10];
        char input_num[10];
        double input_number[10];
        if(recv(socket_accept,function,10,0) == -1){
            perror("failed to receive");
            exit(1);
        }
        if(recv(socket_accept,input_num,10,0) == -1){
            perror("failed to receive");
            exit(1);
        }
        //printf("receive %s\n",input_num);    
    // we have already gotten the data, and now we should send it to the 3 servers
    double a = atof(input_num);
    input_number[0] = a; // transfer char[] to double
printf("The AWS received input <%lf> and function = %s from the client using TCP over port 25356\n",a,function);
    //printf("receive %lf\n",a);
// send to A  
    int sentA = sendto(socket1,input_number,10,0,(struct sockaddr*)&serverA_addr,sizeof(serverA_addr));
    if (sentA == -1){
        perror("failed to send number");
        exit(1);
    }
printf("The AWS sent <%lf> to Backend-Server A\n",a);
// send to B  
    int i = 0;
//make a time out here in order to make the packets in order
    while(i < 10000000){i++;}
    int sentB = sendto(socket1,input_number,10,0,(struct sockaddr*)&serverB_addr,sizeof(serverB_addr));
    if (sentB == -1){
        perror("failed to send number");
        exit(1);
    }
printf("The AWS sent <%lf> to Backend-Server B\n",a);
// send to C  
    i = 0;
    while(i < 10000000){i++;}
    int sentC = sendto(socket1,input_number,10,0,(struct sockaddr*)&serverC_addr,sizeof(serverC_addr));
    if (sentC == -1){
        perror("failed to send number");
        exit(1);
    }
printf("The AWS sent <%lf> to Backend-Server C\n",a);

  //printf("send %s\n",input_number);
//the number is sent to the 3 servers, and now we should receive the answers
//receive from A
    socklen_t serverA_len = sizeof(serverA_addr);
    if(recvfrom(socket1,result_A,10,0,(struct sockaddr*)&serverA_addr,&serverA_len) ==-1){
        perror("failed to receive result");
        exit(1);
    }
printf("The AWS received <%lf> from Backend-Server <A> using UDP over port <24356>\n",result_A[0]);
//receive from B
    socklen_t serverB_len = sizeof(serverB_addr);
    if(recvfrom(socket1,result_B,10,0,(struct sockaddr*)&serverB_addr,&serverB_len) ==-1){
        perror("failed to receive result");
        exit(1);
    }
printf("The AWS received <%lf> from Backend-Server <B> using UDP over port <24356>\n",result_B[0]);
//receive from C
    socklen_t serverC_len = sizeof(serverC_addr);
    if(recvfrom(socket1,result_C,10,0,(struct sockaddr*)&serverC_addr,&serverC_len) ==-1){
        perror("failed to receive result");
        exit(1);
    }
printf("The AWS received <%lf> from Backend-Server <C> using UDP over port <24356>\n",result_C[0]);
    //printf("receive %lf from A\n",result_A[0]);
    //printf("receive %lf from B\n",result_B[0]);
    //printf("receive %lf from C\n",result_C[0]);
// send x^2 to A and B again
    int sentA2 = sendto(socket1,result_A,10,0,(struct sockaddr*)&serverA_addr,sizeof(serverA_addr));
    if (sentA2 == -1){
        perror("failed to send number");
        exit(1);
    }
printf("The AWS sent <%lf> to Backend-Server A\n",result_A[0]);
    i = 0;
    while(i < 10000000){i++;}
    int sentB2 = sendto(socket1,result_A,10,0,(struct sockaddr*)&serverB_addr,sizeof(serverB_addr));
    if (sentB2 == -1){
        perror("failed to send number");
        exit(1);
    }
printf("The AWS sent <%lf> to Backend-Server B\n",result_A[0]);
// receive from A and B again
    double result_A2[10];
    double result_B2[10];
    //socklen_t serverA_len = sizeof(serverA_addr);
    if(recvfrom(socket1,result_A2,10,0,(struct sockaddr*)&serverA_addr,&serverA_len) ==-1){
        perror("failed to receive result");
        exit(1);
    }
printf("The AWS received <%lf> from Backend-Server <A> using UDP over port <24356>\n",result_A2[0]);
    //socklen_t serverA_len = sizeof(serverA_addr);
    if(recvfrom(socket1,result_B2,10,0,(struct sockaddr*)&serverB_addr,&serverB_len) ==-1){
        perror("failed to receive result");
        exit(1);
    }
printf("The AWS received <%lf> from Backend-Server <B> using UDP over port <24356>\n",result_B2[0]);
  //}
    // now compute the final result
    double final_result[10];
    double A = result_A[0];
    double B = result_B[0];
    double C = result_C[0];
    double D = result_A2[0];
    double E = result_B2[0];
printf("Values of powers received by AWS:<%lf %lf %lf %lf %lf %lf>\n",a,A,B,D,C,E);
    if (! strcmp(function,"LOG")){
        final_result[0] = -a - A/2 - B/3 - D/4 - C/5 - E/6;
    }
    else{
        final_result[0] = 1+a+A+B+D+C+E;
    }
printf("AWS calculated %s on <%lf> : <%lf> \n",function,a,final_result[0]);
// send the final result to client
    if(send(socket_accept,final_result,10,0) == -1){
        perror("failed to send the final result");
        exit(1);
    }
printf("The AWS send <%lf> to client",final_result[0]);
    close(socket_accept);
}
    close(socket1);
    close(socket3);

    return 0;
}
