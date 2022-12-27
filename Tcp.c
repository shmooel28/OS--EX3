#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/wait.h>

#define SIZE 1024

void send_file(FILE *fp, int sockfd)
{
    char data[SIZE] = {0};

    while(fgets(data, SIZE, fp)!=NULL)
    {
        if(send(sockfd, data, strlen(data), 0)== -1)
        {
            perror("[-] Error in sendung data");
            exit(1);
        }
        bzero(data, SIZE);
    }
}

unsigned int checksum(FILE* fp){
    unsigned char checksum = 0;
    while (!feof(fp) && !ferror(fp)) {
    checksum ^= fgetc(fp);
    }
    fclose(fp);
    return checksum;
}




void write_file(int sockfd)
{
    int n; 
    FILE *fp;
    char *filename = "file2.txt";
    char buffer[SIZE+1];

    fp = fopen(filename, "w");
    if(fp==NULL)
    {
        perror("[-]Error in creating file.");
        exit(1);
    }
    while(!feof(fp))
    {
        n = recv(sockfd, buffer, SIZE+1, 0);
        if(n<=0)
        {
            break;
            return;
        }
		buffer[n] = '\0';
		fwrite(buffer, 1, n, fp);
        //fprintf(fp, "%s", buffer);
        bzero(buffer, SIZE);
    }
    fclose(fp);
    return;
    
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
    int pid1 = fork();
    if(pid1 <0){return 1;}
    if(pid1 ==0){

    char *ip = "127.0.0.1";
    int port = 8080;
    int e;

    int sockfd;
    struct sockaddr_in server_addr;
    FILE *fp;
    char *filename = "file1.txt";
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd<0)
    {
        perror("[-]Error in socket");
        exit(1);
    }
     //printf("[+]Server socket created. \n");

     server_addr.sin_family = AF_INET;
     server_addr.sin_port = port;
     server_addr.sin_addr.s_addr = inet_addr(ip);

     e = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
     if(e == -1)
     {
         perror("[-]Error in Connecting");
         exit(1);
     }
     //printf("[+]Connected to server.\n");
     fp = fopen(filename, "r");
     if(fp == NULL)
     {
         perror("[-]Error in reading file.");
         exit(1);
     }
     print_time("TCP/IPv4 Socke start");
     send_file(fp,sockfd);
     //printf("[+] File data send successfully. \n");
     close(sockfd);
     //printf("[+]Disconnected from the server. \n");
     fclose(fp);
     return 0;
    }
    else{
    char *ip = "127.0.0.1";
    int port = 8080;
    int e;

    int sockfd, new_sock;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    char buffer[SIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd<0)
    {
        perror("[-]Error in socket");
        exit(1);
    }
     //printf("[+]Server socket created. \n");

     server_addr.sin_family = AF_INET;
     server_addr.sin_port = port;
     server_addr.sin_addr.s_addr = inet_addr(ip);

     e = bind(sockfd,(struct sockaddr*)&server_addr, sizeof(server_addr));
     if(e<0)
     {
         perror("[-]Error in Binding");
         exit(1);
     }
     //printf("[+]Binding Successfull.\n");

     e = listen(sockfd, 10);
     if(e==0)
     {
         //printf("[+]Listening...\n");
     }
     else 
     {
         perror("[-]Error in Binding");
         exit(1);
     }
     addr_size = sizeof(new_addr);
     new_sock = accept(sockfd,(struct sockaddr*)&new_addr, &addr_size);

     write_file(new_sock);
     //printf("[+]Data written in the text file ");

     wait(NULL);

     FILE * f1 = fopen("file1.txt", "r");
     FILE * f2 = fopen("file2.txt", "r");
     if(checksum(f1) == checksum(f2)){
         print_time("TCP/IPv4 Socke end");
     }
     else{
         printf("TCP/IPv4 Socke end: -1");
     }
     return 0;
    }
     

}