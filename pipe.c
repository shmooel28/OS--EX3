#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include<sys/wait.h>
#include <string.h>

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



int main(){
    int fd[2];
    if(pipe(fd) !=0){
        perror("pipe fail");
    }



    int pid1 = fork();
    if(pid1 == 0){
        FILE * f1 = fopen("file1.txt", "r");
        char* str = (char *) malloc(DATA_SIZE * sizeof(char));
        print_time("PIPE start time");

        str[DATA_SIZE-1] =0;
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        if(fgets(str, DATA_SIZE, f1)!=NULL){
            printf("%s",str);
        }
        else{
            perror("error reading from the file");
            return 1;
        }
        fclose(f1);
        free(str);
        return 0;
    }
    if(pid1!=0){
        close(fd[1]);
        FILE * f2 = fopen("file2.txt", "w");
        char* _str = (char *) malloc(DATA_SIZE);
        read(fd[0], _str, DATA_SIZE);
        fwrite(_str, DATA_SIZE, sizeof(char), f2);
        FILE * f1 = fopen("file1.txt", "r");
        if(checksum(f2) == checksum(f1)){
            print_time("PIPE end time");
        }
        else{
            printf("PIPE end time: -1 ");
        }
        free(_str);
        close(fd[0]);
        return 0;

    }



}