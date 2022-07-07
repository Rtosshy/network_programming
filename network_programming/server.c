/* コネクション型 対話プログラム （サーバ） */
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
    int client_sockfd;
    struct sockaddr_in addr;

    socklen_t len = sizeof( struct sockaddr_in );
    struct sockaddr_in from_addr;

    char buf[1024];
    //ここからはサーバはずっと待ち受け状態

    while(1){
        // 受信バッファ初期化
        memset( buf, 0, sizeof( buf ) );

        // ソケット生成
        if( ( sockfd = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 ) {
            perror( "socket" );
        }

        // 待ち受け用IP・ポート番号設定
        addr.sin_family = AF_INET;
        addr.sin_port = htons( 7038 ); //自分用のポート番号を入れる．
        addr.sin_addr.s_addr = INADDR_ANY;

        // バインドする
        if( bind( sockfd, (struct sockaddr *)&addr, sizeof( addr ) ) < 0 ) {
            perror( "bind" );
        }

        // 受信待ち
        if( listen( sockfd, SOMAXCONN ) < 0 ) {
            perror( "listen" );
        }

        // クライアントからのコネクト要求待ち
        if( ( client_sockfd = accept( sockfd, (struct sockaddr *)&from_addr, &len ) ) < 0 ) {
            perror( "accept" );
        }

        // 受信
        int rsize;
        while( 1 ) {
            rsize = recv( client_sockfd, buf, sizeof( buf ), 0 );

            if ( rsize == 0 ) {
                break;
            } 
            else if ( rsize == -1 ) {
                perror( "recv" );
            } 
            else {
                printf( "receive:%s\n", buf );
                sleep( 1 );

                // クライアントからの応答があれば，ソケット宛にそのデータを返す．
                printf( "send:%s\n", buf );
                write( client_sockfd, buf, rsize );
            }
        }

        // ソケットクローズ
        close( client_sockfd );
        close( sockfd );
    }
    return 0;
}