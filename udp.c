#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/wait.h>

#define PORT 5566
#define DATA_SIZE 100000000

unsigned int checksum(FILE* fp){
    unsigned char checksum = 0;
    while (!feof(fp) && !ferror(fp)) {
    checksum ^= fgetc(fp);
    }
    fclose(fp);
    return checksum;
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
    FILE* f1 = fopen("file1.txt" , "r");
    unsigned int chcksum = checksum(f1);
    int pid1 = fork();
    if(pid1 ==0 ){
        sleep(1);
        int sock;
        int status;
        struct addrinfo sainfo, *psinfo;
        struct sockaddr_in6 sin6;
        int sin6len;
        char buffer[1];

        sin6len = sizeof(struct sockaddr_in6);

        sock = socket(PF_INET6, SOCK_DGRAM,0);

        memset(&sin6, 0, sizeof(struct sockaddr_in6));
        sin6.sin6_port = htons(0);
        sin6.sin6_family = AF_INET6;
        sin6.sin6_addr = in6addr_any;

        status = bind(sock, (struct sockaddr *)&sin6, sin6len);

        if(-1 == status)
            perror("bind"), exit(1);

        memset(&sainfo, 0, sizeof(struct addrinfo));
        memset(&sin6, 0, sin6len);

        sainfo.ai_flags = 0;
        sainfo.ai_family = PF_INET6;
        sainfo.ai_socktype = SOCK_DGRAM;
        sainfo.ai_protocol = IPPROTO_UDP;
        status = getaddrinfo("ip6-localhost", "5566", &sainfo, &psinfo);


            char c;
            FILE* fp = fopen("file1.txt", "r");
            print_time("UDP/IPv6 Socket");
            while((c = fgetc(fp)) ){
                if (sendto(sock, &c, 1, 0,
                    (struct sockaddr *)psinfo->ai_addr, sin6len) == -1){
                        perror("error send");
                    }
                    if(c == EOF){break;}
            }

            fclose(fp);
        // free memory
        freeaddrinfo(psinfo);
        psinfo = NULL;

        shutdown(sock, 2);
        close(sock);
        return 0;
    }
    else{
           int sock;
            int status;
            struct sockaddr_in6 sin6;
            int sin6len;
            char buffer[1];

            sock = socket(PF_INET6, SOCK_DGRAM,0);

            sin6len = sizeof(struct sockaddr_in6);

            memset(&sin6, 0, sin6len);

            /* just use the first address returned in the structure */

            sin6.sin6_port = htons(PORT);
            sin6.sin6_family = AF_INET6;
            sin6.sin6_addr = in6addr_any;

            status = bind(sock, (struct sockaddr *)&sin6, sin6len);
            if(-1 == status)
                perror("bind"), exit(1);

            status = getsockname(sock, (struct sockaddr *)&sin6, &sin6len);

            FILE * fp = 0;
            while (status = recvfrom(sock, buffer, 1, 0, 
                        (struct sockaddr *)&sin6, &sin6len)>0  )
                    {
                        if(fp == 0 ){ fp = fopen("file2.txt", "w");}
                        if( buffer[0] == EOF){break;}
                        fwrite(&buffer[0],1, sizeof(char), fp);
                    }


            
            fclose(fp);
            shutdown(sock, 2);
            close(sock);
            wait(NULL);
            FILE* f2 = fopen("file2.txt", "r");
            if( chcksum == checksum(f2)){
                print_time("UDP/IPv6 Socket end");
            }
            else{
                printf("UDP/IPv6 Socket end: -1 ");
            }

            return 0;
    }
}
