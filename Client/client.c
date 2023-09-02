#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#define MAX_BUFFER_SIZE 1024
#define PORT 8080
#define SA struct sockaddr


void sendImage(int sockfd, unsigned char* image, int size) 
{
   
    if (write(sockfd, &size, sizeof(int)) <= 0) 
    {
        perror("Error sending image size");
        close(sockfd);
        exit(1);
    }

    if (write(sockfd, image, size) <= 0)
    {
        perror("Error sending image data");
        close(sockfd);
        exit(1);
    }
}

void receiveImage(int sockfd, int width, int height, int channels, const char* outputPath) 
{
    int size;
    if (read(sockfd, &size, sizeof(int)) <= 0) 
    {
        perror("Error receiving image size");
        close(sockfd);
        exit(1);
    }

    unsigned char* image = (unsigned char*)malloc(size);
    if (read(sockfd, image, size) <= 0) 
    {
        perror("Error receiving image data");
        close(sockfd);
        free(image);
        exit(1);
    }
    
    if (!stbi_write_png(outputPath, width, height, channels, image, 0)) 
    {
        printf("Error saving image: %s\n", stbi_failure_reason());
    }
     else 
    {
        printf("Image saved successfully to %s\n", outputPath);
    }

    free(image);
}

int main() 
{
    int sockfd;
    struct sockaddr_in servaddr;
    int idx = 0;
    while(1)
    {
        idx++;
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) 
        {
            printf("socket creation failed...\n");
            exit(0);
        } 

        bzero(&servaddr, sizeof(servaddr));

        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        servaddr.sin_port = htons(PORT);

        if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) 
        {
            printf("connection with the server failed...\n");
            exit(0);
        } 
    
        char imagePath[MAX_BUFFER_SIZE];
        printf("Enter the path to the image file: ");
        fgets(imagePath, MAX_BUFFER_SIZE, stdin);
        if(imagePath == "exit")
            break;
        imagePath[strcspn(imagePath, "\n")] = '\0';

        int width, height, channels;
        unsigned char* image = stbi_load(imagePath, &width, &height, &channels, 3); 

        sendImage(sockfd, image, width * height * channels);

        strcat(imagePath, "eq.png");
        receiveImage(sockfd, width, height, channels, imagePath);
        stbi_image_free(image);
    }

    close(sockfd);
    return 0;
}