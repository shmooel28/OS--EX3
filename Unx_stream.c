#include <sys/types.h>
 #include <sys/socket.h>
 #include <sys/un.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <time.h>
 #define NAME "socket"
 #define DATA_SIZE 100000000

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



unsigned int checksum(FILE* fp){
    unsigned char checksum = 0;
    while (!feof(fp) && !ferror(fp)) {
    checksum ^= fgetc(fp);
    }
    fclose(fp);
    return checksum;
}
 int main()
 {
	FILE * f1 = fopen("file1.txt", "r");
	unsigned int chksum = checksum(f1);
	int pid1 = fork();

		if(pid1 == 0){
			sleep(1);
			int sock;
			struct sockaddr_un server;
			char buf[1024];
			sock = socket(AF_UNIX, SOCK_STREAM, 0);
			if (sock < 0) {
				perror("opening stream socket");
				exit(1);
			}
			server.sun_family = AF_UNIX;
			strcpy(server.sun_path, NAME);
			if (connect(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un)) < 0) {
				close(sock);
				perror("connecting stream socket");
				exit(1);
			}
			FILE* fp = fopen("file1.txt", "r");
			char c;
			print_time("UDS - Stream socket start:")
			while(c = fgetc(fp)){
				if (write(sock, &c, 1) < 0){
					perror("writing on stream socket");
					break;
				}
				if(c == EOF){
					break;
				}
			}
			fclose(fp);
			close(sock);
			return 0;
		}
		else{
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
				FILE * f2 = fopen("file2.txt", "r");
				if(chksum == checksum(f2)){
					print_time("UDS - Stream socket end:")
				}
				else{
					printf("UDS - Stream socket end: -1");
				}
				return 0;
		}
	}
