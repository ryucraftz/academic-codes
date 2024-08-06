#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int sd;
    struct sockaddr_in servaddr;

    // Create socket
    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd == -1) {
        printf("Socket not created in client\n");
        exit(0);
    } else {
        printf("Socket created in client\n");
    }

    // Set server address
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address
    servaddr.sin_port = htons(8000); // Server port number

    // Set choice to send a text file
    char choice = 1;
    sendto(sd, &choice, sizeof(choice), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));

    // Send file name
    char fileName[100] = "test.txt";
    sendto(sd, fileName, sizeof(fileName), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));

    // Send file content
    char fileBuffer[2000] = "This is a test text file.";
    sendto(sd, fileBuffer, sizeof(fileBuffer), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));

    close(sd);
    return 0;
}
