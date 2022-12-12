#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define NAME "socket"
int main()
{
    int sock, msgsock, rval;
    struct sockaddr_un server;
     char buf[1024];
     sock = socket(AF_UNIX, SOCK_STREAM, 0);
     if (sock < 0) {
         perror("opening stream socket");
         exit(1);
     }
     server.sun_family = AF_UNIX;
     strcpy(server.sun_path, NAME);
     if (bind(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un))) {
         perror("binding stream socket");
         exit(1);
     }
    FILE * fp = 0;
    listen(sock, 5);
    msgsock = accept(sock, 0, 0);
    if (msgsock == -1)
        perror("accept");
         
         
         else do {
              if(fp == 0){
                fp = fopen("file2.txt" , "w+");
              }
             bzero(buf, sizeof(buf));
             if ((rval = read(msgsock, buf, 1)) < 0)
                 perror("reading stream message");
             else if (rval == 0 || buf[0] == EOF){
                 break;
             }
             else
                fwrite(&buf[0],1, sizeof(char), fp);
         } while (rval > 0);


    close(msgsock);
    fclose(fp);
    close(sock);
    unlink(NAME);
    return 0
 }
