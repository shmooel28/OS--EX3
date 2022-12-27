#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <sys/wait.h>

#define SOCK_PATH_SERVER "unix-dgram-server.temp"
#define SOCK_PATH_CLIENT "unix-dgram-client.temp"

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

    int pid1 = fork();
    if(pid1 <0){return 1;}
    if(pid1 ==0){
	sleep(2);
		struct sockaddr_un remote;

	// Create a socket - the only information is the type of socket we
	// want to create, not the address we want to connect to.
	int s;
	if ((s = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

#if 0
	// If we also wanted to receive datagrams, we would need to call
	// bind().
	struct sockaddr_un local;
	local.sun_family = AF_UNIX;
	strcpy(local.sun_path, SOCK_PATH_CLIENT);
	unlink(local.sun_path);

	int len = strlen(local.sun_path) + sizeof(local.sun_family);
	if (bind(s, (struct sockaddr *)&local, len) == -1)
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}
#endif

	// What address are we sending the packets to?
	remote.sun_family = AF_UNIX;
	strcpy(remote.sun_path, SOCK_PATH_SERVER);


	// Use send() and recv() to send and receive information on the
	// socket. Note that send(?, ?, ?, 0) is equivalent to write(?, ?,
	// ?). Similarly, recv(?, ?, ?, 0) is the same as read(?, ?, ?).
	//
	// Print a prompt, read a line of text that the user types in.
	char str[100];
	FILE* fp = fopen("file1.txt", "r");
	while(fgets(str, 100, fp))
	{
		if (sendto(s, str, strlen(str), 0,
				   (struct sockaddr*) (&remote),
				   (socklen_t) sizeof(struct sockaddr_un)) == -1)
		{
			perror("sendto");
			exit(EXIT_FAILURE);
		}
		//printf("%s", str);
		//printf("sent %zu bytes\n", strlen(str));

	}

	close(s);

	return EXIT_SUCCESS;

	}
	
	
	else{
	
		struct sockaddr_un remote;

	/*
	  A call to socket() with the proper arguments creates the Unix
	  socket.
	  The second argument, SOCK_DGRAM, tells socket() a datagram
	  socket. The number of read() or recv() must match the number of
	  send() or write(). If you read with a size smaller than the size
	  of the packet, you won't receive the entire message.
	  One more note: all these calls return -1 on error and set the
	  global variable errno to reflect whatever went wrong. Be sure to
	  do your error checking.
	*/
	int s = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (s == -1)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}

	/*
	  You got a socket descriptor from the call to socket(), now you
	  want to bind that to an address in the Unix domain. (That
	  address, is a special file on disk.)
	  This associates the socket descriptor "s" with the Unix socket
	  address "/home/beej/mysocket". Notice that we called unlink()
	  before bind() to remove the socket if it already exists. You
	  will get an EINVAL error if the file is already there.
	*/
	struct sockaddr_un local;
	local.sun_family = AF_UNIX;
	strcpy(local.sun_path, SOCK_PATH_SERVER);
	unlink(local.sun_path);
	int len = strlen(local.sun_path) + sizeof(local.sun_family);
	if (bind(s, (struct sockaddr *)&local, len) == -1)
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}

#if 0
	// If we wanted to send packets back to the client, we would need
	// this code.
	
	// What address are we sending the packets to?
	remote.sun_family = AF_UNIX;
	strcpy(remote.sun_path, SOCK_PATH_CLIENT);
#endif

	// Don't have to call listen() for a datagram connection.
	print_time("UDS - Dgram socket start");
	int done = 0;
	FILE * fp = fopen("file2.txt", "w");
		// don't have to accept() or wait for a connection, just wait
		// for datagrams...
		
		//printf("Waiting for a datagram...\n");


		do
		{
			char str[101];
			socklen_t remoteLen = (socklen_t) sizeof(struct sockaddr_un);
			int n = recvfrom(s, str, 100, 0, (struct sockaddr*)&remote, &remoteLen);
			if( n < 0)
			{
				perror("recvfrom");
				done = 1;
			}
			else if(n == 0)
				done = 1;
			else
			{
				// recvfrom() reads bytes, and so it has no reason to
				// put a null terminator at an end. So, we need to add
				// a null terminator.
				str[n] = '\0';
				// NOTE: The newline gets sent over to us from the client.
				fwrite(str, strlen(str), sizeof(char), fp);
				//printf("%s", str);
				//printf("\nLength of string was: %zu\n", strlen(str));
			}
			if(n < 99){
				break;
			}
			
		} while (!done);

		close(s);

	fclose(fp);

	wait(NULL);
		FILE * f1 = fopen("file1.txt", "r");
		FILE * f2 = fopen("file2.txt", "r");
		if(checksum(f2) == checksum(f1)){
			print_time("UDS - Dgram socket end");
		}
		else{
			printf("UDS - Dgram socket end : -1");
		}

	return 0;
	
	}
 
}
