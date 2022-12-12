#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <time.h>
#include "ipc.h"

int create_file(FILE* f1 , char* str){

    
    if(!str)
    {
        perror("cant creat 100mb\n");
        exit(1);
    }
    srand(time(NULL));
    for (size_t i = 0; i < 100000000; i++)
    {
        char rnd = '0'+(random()%2);
        str[i] =  rnd;
    }
    //printf("size of str = %ld\n",sizeof(str));
    //fprintf(f1,"%s",str);

}




int main() {
	char *str = malloc(100000000);
    FILE *f1;
    f1 = fopen("file1.txt","w+");
    create_file(f1, str);

    fclose(f1);
	free(str);
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