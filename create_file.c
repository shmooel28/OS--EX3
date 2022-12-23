#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int create_file(FILE* f1 , char* str){

    
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
    //printf("size of str = %ld\n",sizeof(str));
    fprintf(f1,"%s",str);

}

int main(){
    char *str = malloc(100000000);
    FILE *f1;
    f1 = fopen("file1.txt","w+");
    create_file(f1, str);

    fclose(f1);
    free(str);
}