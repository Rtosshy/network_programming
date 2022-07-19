#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<string.h>
#include<unistd.h>

/* 下のポート番号の ** には自分の出席番号を入れる */
#define BUFMAX 100
#define Err(x) {fprintf( stderr,"server-"); perror(x); return(0);}
char shostname[60];
char buf[BUFMAX];

int main(int argc, char *argv[]) {
    struct hostent *shost;
    struct sockaddr_in me;
    int s_waiting, s_c;
    union {
        unsigned int i;
        unsigned char c[4];
    }ipaddr;
    int len;

    int PORT_NO = atoi(argv[2]);

    shost = gethostbyname(argv[1]);

    bzero((char *)&me,sizeof(me));
    me.sin_family = AF_INET;
    me.sin_port = htons(PORT_NO);
    me.sin_addr.s_addr = htonl(INADDR_ANY);
    bcopy(shost->h_addr,(char *)&me.sin_addr,shost->h_length);


    /* ソケットを獲得し、自分のアドレスを結び付ける */
    if((s_waiting = socket(AF_INET,SOCK_STREAM,0)) < 0)
    Err("socket");


    /* 接続の確立 */
    if ((s_c = connect(s_waiting, (struct sockaddr *)&me, sizeof(me))) < 0) {
        perror("connect");
        exit(1);
    }
    printf("s_w:%d s_c:%d\n",s_waiting, s_c);
    /* 対話のループ */
    do {
        int n;
        write(1,"Waiting for message.\n",22);
        n = read(s_waiting,buf,BUFMAX); //ソケット読み込み
        write(1,buf,n); // ディスプレイ表示
        write(1,"Send message.\n",14);
        n = read(0,buf,BUFMAX); // キーボード入力
        write(s_waiting,buf,n); // ソケット書き出し
    }
    while(strncmp(buf,"quit",4) != 0);
    close(s_waiting);
    return 0;
}