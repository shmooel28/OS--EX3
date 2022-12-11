#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#include<sys/wait.h>

#define SIZE 1024
#define TICK(X) clock_t X = clock()
#define TOCK(X) printf("time %s: %g sec.\n", (#X), (double)(clock() - (X)) / CLOCKS_PER_SEC)


void send_file(FILE *fp, int sockfd)
{
    char data[SIZE] = {0};

    while(fgets(data, SIZE, fp)!=NULL)
    {
        if(send(sockfd, data, sizeof(data), 0)== -1)
        {
            perror("[-] Error in sendung data");
            exit(1);
        }
        bzero(data, SIZE);
    }
}


void write_file(int sockfd)
{
    int n; 
    FILE *fp;
    char *filename = "file2.txt";
    char buffer[SIZE];

    fp = fopen(filename, "w");
    if(fp==NULL)
    {
        perror("[-]Error in creating file.");
        exit(1);
    }
    while(1)
    {
        n = recv(sockfd, buffer, SIZE, 0);
        if(n<=0)
        {
            break;
            return;
        }
        fprintf(fp, "%s", buffer);
        bzero(buffer, SIZE);
    }
    return;
    
}

int create_file(FILE* f1 , char* str){

    
    if(!str)
    {
        perror("cant creat 100mb\n");
        exit(1);
    }
    srand(time(NULL));
    for (size_t i = 0; i < 100000000/sizeof(char); i++)
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
    char *str = malloc(100000000);
    FILE *f1;
    f1 = fopen("file1.txt","w+");
    create_file(f1, str);

    fclose(f1);


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
        print_time("TCP IPV4 socket start");
        send_file(fp,sockfd);
        //printf("[+] File data send successfully. \n");
        close(sockfd);
        //printf("[+]Disconnected from the server. \n");
        return 0;
    }
    else{

        clock_t start, end;
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
        //    printf("[+]Listening...\n");
        }
        else 
        {
            perror("[-]Error in Binding");
            exit(1);
        }
        addr_size = sizeof(new_addr);
        new_sock = accept(sockfd,(struct sockaddr*)&new_addr, &addr_size);

        write_file(new_sock);
        end = clock();
        //printf("TCP IPV4 socket: %ld", clock());
        //printf("[+]Data written in the text file ");
        print_time("TCP IPV4 socket end");
        wait(NULL);
        return 0;
        }
     

}