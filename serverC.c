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

#define ServerC_port 23356
#define fclient_port 24356
int main(){
    // create socket, error occured if return -1
    int socket1 = socket(AF_INET,SOCK_DGRAM,0);
    if (socket1 == -1){
        perror("failed to create socket");
        exit(1);
    }
    // create serverA address
    struct sockaddr_in  serverC_addr;
    memset(&serverC_addr,0,sizeof(serverC_addr));
    serverC_addr.sin_family = AF_INET;
    serverC_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverC_addr.sin_port = htons(ServerC_port);
    // now bind, error occured if return -1
    if(bind(socket1,(struct sockaddr*)&serverC_addr,sizeof(serverC_addr)) == -1){
        perror("failed to bind socket");
        exit(1);
    }
    // now we can transmit data
printf("The server C is up and running using UDP on port 23356 \n");
  while(1){
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        double receive_number[10];
        int received = recvfrom(socket1,receive_number,10,0,(struct sockaddr*)&client_addr,&client_len);
        if (received == -1){
            perror("failed to receive data");
            exit(1);
        }
        double a = receive_number[0];
printf("The server C received input <%lf> \n",a);
        double b = a*a*a*a*a;
printf("The server C calculated 5th power: <%lf> \n",b);
        double result[1];
        result[0]=b;
// we can send the result now
    struct sockaddr_in fakeclient_addr;
    memset(&fakeclient_addr,0,sizeof(fakeclient_addr));
    fakeclient_addr.sin_family = AF_INET;
    fakeclient_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    fakeclient_addr.sin_port = htons(fclient_port);
    if(sendto(socket1,result,10,0,(struct sockaddr*)&fakeclient_addr,sizeof(fakeclient_addr)) == -1){
        perror("failed to send result");
        exit(1);
    }
printf("The server C finished sending the output to AWS\n");
   // printf("send %lf to the aws\n",result[0]);
//  break;
  } 
    close(socket1);
    return 0;
}

