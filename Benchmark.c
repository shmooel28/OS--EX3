// gcc Benchmark.c -o bench
#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<time.h>
#include<string.h>

int checkSum(char* str)
{
    int sum = 0;
    int ans = 0;
    for (size_t i = 0; i < 100000000/sizeof(char); i++)
    {
        sum+=str[i];
    }
    ans=~sum; //1's complement of sum
    return ans;
}
int checkSumValid(char *str, int checksum)
{
    int sum = 0;
    int ans = 0;
    for (size_t i = 0; i < 100000000/sizeof(char); i++)
    {
        sum+=str[i];
    }
    sum += checksum;
    ans =~ sum;
    return ans;

}

int main()
{
    FILE *f1;
    f1 = fopen("test.txt","w+");
    char *str = malloc(100000000);
    if(!str)
    {
        perror("cant creat 100mb\n");
        exit(1);
    }
    srand(time(NULL));
    for (size_t i = 0; i < 100000000/sizeof(char); i++)
    {
        char rnd = '0'+(random()%2);
        str[i] =  rnd;
    }
    printf("size of str = %ld\n",sizeof(str));
    fprintf(f1,"%s",str);
    int senderChecksum = checkSum(str);
    printf("the check sum is: %d\n",senderChecksum);
    pid_t pid1, pid2;
    pid1 = fork();
    if (pid1 == 0)
    {
        clock_t start, end;
        start = clock();
        printf("process 1 progres\n");
        end = clock();
        printf("the time take is: %f\n", ((double)(end-start))/CLOCKS_PER_SEC);

    }
    else
    {
        pid2 = fork();
        if(pid2 == 0)
        {
            clock_t start, end;
            start = clock();
            printf("process 2 progres\n");
            end = clock();
            printf("the time take is: %f\n", ((double)(end-start))/CLOCKS_PER_SEC);
        }
        else{
            printf("parent progress\n");
        }
    }
    fclose(f1);
    
}