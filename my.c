#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "share.h"

#define BUFFER_SIZE 256

int main()
{
    char send[BUFFER_SIZE],receive[BUFFER_SIZE];


    char destName[]="/dev/";
    strcat(destName,DEVICE_NAME);

    int fd=open(destName,O_RDWR);
    if(fd<0){perror("***error, can not open the file***\n"); return 0;}

    printf("Write a message: ");

    scanf("%s",send);

    int ret=write(fd,send,strlen(send));
    if(ret<0){perror("***error, can not write to file***\n"); return 0;}

    printf("Press ENTER to read data...\n");
    getchar();

    printf("read data: ");
    ret=read(fd,receive,256);
    if(ret<0){perror("***error, can not read the file***\n"); return 0;}

    printf("%s\n",receive);

    return 0;
}