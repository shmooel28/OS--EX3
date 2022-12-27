#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include<sys/wait.h>
#include <string.h>
#include <pthread.h>
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



void* readFile(void* arg){

    print_time("shared memory between threads start");
    FILE* f1 = fopen("file1.txt", "r");
    char * str = (char *)arg;
    fgets(str, DATA_SIZE, f1);
    fclose(f1);

}

void* writeFile(void* arg){
    char * str = (char *)arg;
    FILE* f2 = fopen("file2.txt", "w");
    fwrite(str, DATA_SIZE, sizeof(char), f2);
    fclose(f2);
}

int main(){
    pthread_t _read , _write;
    char *str = (char *) malloc(DATA_SIZE * sizeof(char));
    if(pthread_create(&_read, NULL, &readFile, str) !=0){
         perror("Failed to created thread");
    }
    if(pthread_join(_read, NULL)!=0){
        perror("Faild runing the thread");
    }
    if(pthread_create(&_write, NULL, &writeFile, str) !=0){
         perror("Failed to created thread");
    }
    if(pthread_join(_write, NULL)!=0){
        perror("Faild runing the thread");
    }
    FILE * f1 = fopen("file1.txt", "r");
    FILE * f2 = fopen("file1.txt", "r");
    if(checksum(f1) == checksum(f2)){
        print_time("shared memory between threads end");
    }
    else{
        printf("shared memory between threads end: -1");
    }

    free(str);
    return 0;
}