#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define BUF_SIZE 1024

int isNumber(char* str){
    for(int i = 0; i < strlen(str); i++){
        if(!isdigit(str[i])){
            return 0;
        }
    }
    return 1;
}


int main (int argc, char * argv [ ])
{


    if (argc != 4) {
        printf("Usage: %s <old-file> <new-file> <numbers-value> \n", argv[0]);
        exit(-1);
    }

    if(!isNumber(argv[3])){
        printf("Wrong numbers value argument: %s\n", argv[3]);
        exit(-4);
    }


    char* dot_vol = argv [3];

    int int_dot = atoi(dot_vol);

    if (int_dot<0){
        printf("Wrong numbers value argument: %s\n", argv[3]);
        exit(-4);
    }




    int inputFd, outputFd, openFlags;
    mode_t filePerms;
    ssize_t numRead;
    char buf[BUF_SIZE];



    /* Открытие файлов ввода и вывода */
    inputFd = open (argv[1], O_RDONLY);
    if (inputFd == -1) {
        printf ("Error opening file %s\n", argv[1]) ; exit(-2);
    }

    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH; /* rw - rw - rw - */
    outputFd = open (argv [2], openFlags, filePerms);
    if (outputFd == -1) {
        printf ("Error opening file %s\n ", argv[2]) ; exit(-3);
    }



    int j = 0;
    /* Перемещение данных до достижения конца файла ввода или возникновения ошибки */
    while ((numRead = read (inputFd, buf, BUF_SIZE)) > 0) {
        int i;
        for (i = 0; i < numRead; i++) {
            if (buf[i] >= 48 && buf[i] <= 57 && j<int_dot) {
                j++;
                if (write(outputFd, " ", 2) != 2) {
                    printf ("couldn't write whole buffer\n");
                    exit(-4);
                }
            } else {
                if (write(outputFd, &buf[i], 1) != 1) {
                    printf ("couldn't write whole buffer\n");
                    exit(-4);
                }
            }
        }
    }
    printf("(PID: %d), File %s, numbers = %d\n", getpid(), argv[1], j);

    if (close (inputFd ) == -1 ) {
        printf ("close input error\n"); exit(-6);
    }
    if (close (outputFd ) == -1 ) {
        printf ("close output error\n"); exit(-7);
    }

    exit(EXIT_SUCCESS);
}