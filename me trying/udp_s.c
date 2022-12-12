#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>

#define MAXBUF 65536
#define PORT 5566
int main()
{
   int sock;
   int status;
   struct sockaddr_in6 sin6;
   int sin6len;
   char buffer[MAXBUF];

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
   return 0;
}
