#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include<sys/wait.h>
#include "ipc.h"
#define DATA_SIZE 100000000

int create_file(FILE* f1 , char* str){

    
    if(!str)
    {
        perror("cant creat 100mb\n");
        exit(1);
    }
    srand(time(NULL));
    for (size_t i = 0; i < DATA_SIZE; i++)
    {
        char rnd = '0'+(random()%2);
        str[i] =  rnd;
    }
    //printf("size of str = %ld\n",sizeof(str));
    fprintf(f1,"%s",str);

}
int print_time(char* str){
    int hours, minutes, seconds, day, month, year;
    time_t now;


    struct tm *local = localtime(&now);
    hours = local->tm_hour;
    minutes = local->tm_min;
    seconds = local->tm_sec;

    printf("%s:" ,str);
    time(&now);
    printf(" %s\n", ctime(&now));
    return 1;
}
int main()
{
    FILE* f1 = fopen("file1.txt", "r");
    unsigned int chcksm = checksum(f1);

    int pid1 = fork();
    if(pid1 <0){return 1;}
    if(pid1 ==0){
        // client side
        sleep(1);
        int fd;
        struct sockaddr_un addr;
        int ret;
        char buff[8192] = {EOF};
        struct sockaddr_un from;
        int ok = 1;
        int len;

        if ((fd = socket(PF_UNIX, SOCK_DGRAM, 0)) < 0) {
            perror("socket");
            ok = 0;
        }

        if (ok) {
            memset(&addr, 0, sizeof(addr));
            addr.sun_family = AF_UNIX;
            strcpy(addr.sun_path, CLIENT_SOCK_FILE);
            unlink(CLIENT_SOCK_FILE);
            if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
                perror("bind");
                ok = 0;
            }
        }

        if (ok) {
            memset(&addr, 0, sizeof(addr));
            addr.sun_family = AF_UNIX;
            strcpy(addr.sun_path, SERVER_SOCK_FILE);
            if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
                perror("connect");
                ok = 0;
            }
        }

        FILE * fp = fopen("file1.txt", "r");
        if (ok) {
            char c ;
            print_time("UDS - Dgram socket start:");
            while((c = fgetc(fp))){
                send(fd, &c, 1, 0);
                if(c == EOF || c == '\0'){break;}
            }
            close(fp);
            //while (send(fd, &buff[i++], 1, 0) > 0 && buff[i-1]!='\0') {}
        }

        if (fd >= 0) {
            close(fd);
        }

        unlink (CLIENT_SOCK_FILE);
        return 0;

    }
    else{
        // server side

        int fd;
        struct sockaddr_un addr;
        int ret;
        char buff[8192] = {'\0'};
        struct sockaddr_un from;
        int ok = 1;
        int len;
        socklen_t fromlen = sizeof(from);

        if ((fd = socket(PF_UNIX, SOCK_DGRAM, 0)) < 0) {
            perror("socket");
            ok = 0;
        }

        if (ok) {
            memset(&addr, 0, sizeof(addr));
            addr.sun_family = AF_UNIX;
            strcpy(addr.sun_path, SERVER_SOCK_FILE);
            unlink(SERVER_SOCK_FILE);
            if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
                perror("bind");
                ok = 0;
            }
        }
        if(ok){
            FILE * fp = 0;
            while ((len = recvfrom(fd, buff, 1, 0, (struct sockaddr *)&from, &fromlen)) > 0  && buff[0] != EOF) {
                if(fp == 0){
                    fp = fopen("file2.txt" , "w+");
                }
                fwrite(buff, 1 , sizeof(char) , fp );
                //ret = sendto(fd, buff, strlen(buff)+1, 0, (struct sockaddr *)&from, fromlen);
            }

            print_time("USD End");
            close(fp);
        }


        if (fd >= 0) {
            close(fd);
        }

        wait(NULL);

        FILE * f2 = fopen("file2.txt", "r");
        if(chksum == checksum(f2)){
            print_time("UDS - Dgram socket end:")
        }
        else{
            printf("UDS - Dgram socket end: -1");
        }
        return 0;
    }
}