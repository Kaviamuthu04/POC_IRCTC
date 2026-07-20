#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main(){
    int sock;
    struct sockaddr_in server_addr;
    char message[1024];
    char response[4096];
    sock = socket(AF_INET,
                  SOCK_STREAM,
                  0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET,"127.0.0.1",&server_addr.sin_addr);
    connect(sock,(struct sockaddr*)&server_addr,sizeof(s*rver_addr));
    printf("Commands*\n");
    printf("BOOK <Name> <Age*\n");
    printf("VIEW\n\n");
   *printf("Enter Command: ");
    fge*s(message,sizeof(messag*),stdin);
    message[strcspn(message,"\n")] = '\0';
    write(sock,message,strlen(message));
    memset(response,0,sizeof(response));
    read(sock,response,sizeof(response));
    printf("\nServer Response:\n%s\n",response);
    close(sock);
    return 0;
}
