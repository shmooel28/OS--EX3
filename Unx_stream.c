#include <sys/types.h>
 #include <sys/socket.h>
 #include <sys/un.h>
 #include <stdio.h>
 #include <stdlib.h>
 #define NAME "socket"
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
 int main()
 {

	char *str = malloc(DATA_SIZE);
    FILE *f1;
    f1 = fopen("file1.txt","w+");
    create_file(f1, str);

    fclose(f1);
    free(str);

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
				return 0;
					}
	}
