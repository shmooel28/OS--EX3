// Based on: http://beej.us/guide/bgipc/output/html/multipage/unixsock.html (code from "Beej's Guide to Network Programming" is public domain).
// Modifications by: Scott Kuhl
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCK_PATH "unix-stream-server.temp"

int main(void)
{
	// Create a socket - the only information needed is the type of
	// socket we want to create, not the address we want to connect
	// to.
	int s;
	if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	// Connect the socket to an address. The addresses for Unix domain
	// sockets are special filenames.
	//printf("Trying to connect...\n");
	
	struct sockaddr_un remote; // a struct representing a unix domain socket address
	remote.sun_family = AF_UNIX;
	strcpy(remote.sun_path, SOCK_PATH);
	int len = strlen(remote.sun_path) + sizeof(remote.sun_family);
	if (connect(s, (struct sockaddr *)&remote, len) == -1)
	{
		perror("connect");
		exit(EXIT_FAILURE);
	}

	//printf("Connected.\n");

	// Use send() and recv() to send and receive information on the
	// socket. Note that send(?, ?, ?, 0) is equivalent to write(?, ?,
	// ?). Similarly, recv(?, ?, ?, 0) is the same as read(?, ?, ?).
	//
	// Print a prompt, read a line of text that the user types in.
	FILE * fp = fopen("file1.txt", "r");
	char str[100];
	while(!feof(fp) && fgets(str, 100, fp))
	{	

		if (send(s, str, strlen(str), 0) == -1)
		{
			perror("send");
			exit(EXIT_FAILURE);
		}


	}

	close(s);
	fclose(fp);

	return EXIT_SUCCESS;
}