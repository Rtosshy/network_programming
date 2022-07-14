/* コネクション型 対話プログラム （サーバ） */
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
#define PORT_NO (u_short)7038
#define BUFMAX 100
#define Err(x) {fprintf( stderr,"server-"); perror(x); return(0);}
char shostname[60];
char buf[BUFMAX];

int main() {
    struct hostent *shost;
    struct sockaddr_in me;
    struct sockaddr_in cl;
    int s_waiting, s;
        union {
        unsigned int i;
        unsigned char c[4];
    }ipaddr;
    int len;


    /* 自分のホスト名を得る */
    if (gethostname(shostname,sizeof(shostname)) < 0)
    Err("gethostname");

    /* 自分のアドレスを me という変数につくり出す */
    shost = gethostbyname(shostname);
    if (shost == NULL) Err("gethostbyname");

    bzero((char *)&me,sizeof(me));
    me.sin_family = AF_INET;
    me.sin_port = htons(PORT_NO);
    me.sin_addr.s_addr = htonl(INADDR_ANY);
    bcopy(shost->h_addr,(char *)&me.sin_addr,shost->h_length);

    /*サーバのIPアドレスの表示 */
    ipaddr.i=me.sin_addr.s_addr;
    fprintf(stderr, "%d.%d.%d.%d\n",ipaddr.c[0],ipaddr.c[1],ipaddr.c[2],ipaddr.c[3]);

    /* ソケットを獲得し、自分のアドレスを結び付ける */
    if((s_waiting = socket(AF_INET,SOCK_STREAM,0)) < 0)
    Err("socket");

    if(bind(s_waiting,&me,sizeof(me)) == -1)
    Err("bind");

    fprintf(stderr,"Successfully bound.\n");
    

    /* 接続の確立 */
    if (listen(s_waiting, 1) < 0){
        perror("listen");
        exit(1);
    }

    len = sizeof(cl);
    s = accept(s_waiting,(struct sockaddr *)&cl,&len);


    /* 対話のループ */
    do {
        int n;
        write(1,"Send message.\n",14);
        n = read(0,buf,BUFMAX); // キーボード入力
        write(s,buf,n); // ソケット書き出し
        write(1,"Waiting for message.\n",22);
        n = read(s,buf,BUFMAX); //ソケット読み込み
        write(1,buf,n); // ディスプレイ表示
    }
    while(strncmp(buf,"quit",4) != 0);
    close(s_waiting);
    close(s);
}