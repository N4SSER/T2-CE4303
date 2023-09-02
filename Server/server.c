#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "filter/filter.h"

#define PORT 8080

void sendImage(int connfd, unsigned char* image, int size) 
{
    if (write(connfd, &size, sizeof(int)) <= 0)
    {
        perror("Error sending image size");
        close(connfd);
        exit(1);
    }

    if (write(connfd, image, size) <= 0) 
    {
        perror("Error sending image data");
        close(connfd);
        exit(1);
    }
}


int main()
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);

    while (1) 
    {
        connfd = accept(sockfd, (struct sockaddr*)&cli, &len);
        if (connfd < 0) 
        {
            printf("server accept failed...\n");
            exit(0);
        }
        else
            printf("server accept the client...\n");

        int size;
        if (read(connfd, &size, sizeof(int)) <= 0) 
        {
            perror("Error reading image size");
            close(connfd);
            close(sockfd);
            exit(1);
        }

        unsigned char* image = (unsigned char*)malloc(size);
        if (read(connfd, image, size) <= 0) 
        {
            perror("Error reading image data");
            close(connfd);
            close(sockfd);
            free(image);
        }

        equalizeHistogram((RGBPixel*) image, (int) size/3);
        sendImage(connfd, image, size);
        close(connfd);
    }

    close(sockfd);
    return 0;
}