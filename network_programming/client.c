#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<string.h>
#include<unistd.h>

int main(void) {
    int sockfd;
    struct sockaddr_in addr;
    char _send[20];
    char _recv[20];
    
    if( ( sockfd = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 ) {
        perror( "socket" );
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons( 7038 ); //自分用のポート番号を入れる．
    addr.sin_addr.s_addr = inet_addr("40.74.86.61");

    printf("send:");
    scanf("%s", _send);
    connect( sockfd, (struct sockaddr *)&addr, sizeof(addr));
    send(sockfd, &_send, sizeof(_send), 0);
    int rsize;
    
    rsize = recv(sockfd, &_recv, sizeof(_recv), 0);
    if ( rsize == 0 ) {
    } 
    else if ( rsize == -1 ) {
        perror( "recv" );
    } 
    else {
        printf( "receive:%s\n", _recv);
        sleep( 1 );
    }
    return 0;
}
