#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include<sys/wait.h>
#include <string.h>
#include <sys/mman.h>
 #include <fcntl.h>


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



int main(int argc, char **argv){

    int sfd, dfd;
    char *src, *dest;
    size_t filesize;

    print_time("MMAP start time");
    /* SOURCE */
    sfd = open("file1.txt", O_RDONLY);
    filesize = lseek(sfd, 0, SEEK_END);

    src = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, sfd, 0);

    /* DESTINATION */
    dfd = open("file2.txt", O_RDWR | O_CREAT, 0666);

    ftruncate(dfd, filesize);

    dest = mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, dfd, 0);

    /* COPY */

    memcpy(dest, src, filesize);

    munmap(src, filesize);
    munmap(dest, filesize);

    close(sfd);
    close(dfd);
    FILE* f1 = fopen("file1.txt", "r");
    FILE* f2 =fopen("file2.txt", "r");
    if( (checksum(f1)) == (checksum(f2))){
        print_time("MMAP end time");
    }
    else{
        printf("MMAP end time: -1");
    }

    return 0;
}