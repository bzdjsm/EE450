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
#define fclient_port 24356 // it is the port number of aws for the UDP, it seems like a "fakeclient" so I use this name
int main(){
    // create socket, error occured if return -1
    int socket1 = socket(AF_INET,SOCK_DGRAM,0);
    if (socket1 == -1){
        perror("failed to create socket");
        exit(1);
    }
    // create serverA address
    struct sockaddr_in  ans_addr;
    memset(&ans_addr,0,sizeof(ans_addr));
    ans_addr.sin_family = AF_INET;
    ans_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    ans_addr.sin_port = htons(ServerA_port);
    // now bind, error occured if return -1
    if(bind(socket1,(struct sockaddr*)&ans_addr,sizeof(ans_addr)) == -1){
        perror("failed to bind socket");
        exit(1);
    }
printf("The server A is up and running using UDP on port 21356 \n");
    // now we can transmit data

  // reference from www.jb51.net/net/article/53737.htm
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
printf("The server A received input <%lf> \n",a);
        double b = a*a;
printf("The server A calculated square: <%lf> \n",b);
    //    result[0] = receive_number[0] * receive_number[0];
    
    //printf("%lf",b);
    //char result[10];
    //memcpy(result,&b,sizeof(double));
    double result[10];
    result[0]=b;
    //printf("%s\n",result);
  
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
printf("The server A finished sending the output to AWS\n");
  //printf("send %lf to the aws\n",result[0]);
  //printf("%lf\n",result[0]);
  //break;
//    double receive_number2[10];
//    int received2 = recvfrom(socket1,receive_number2,10,0,(struct sockaddr*)&client_addr,&client_len);
//    if (received2 == -1){
//        perror("failed to receive data");
//        exit(1);
//    }
//    double result2[10];
//    result2[0] = receive_number2[0]*receive_number2[0];
//   if(sendto(socket1,result,10,0,(struct sockaddr*)&fakeclient_addr,sizeof(fakeclient_addr)) == -1){
//        perror("failed to send result");
//        exit(1);
//    }
// At first I thought I should make these to reveive from the aws again to calculate x^4, but finally I realized I did not need to do so.
  } 
    close(socket1);
    return 0;
}
