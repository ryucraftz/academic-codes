#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    char fileName[100];
    char fileBuffer[2000], caufFile[100];
    char *vfIlep;
    int auFile[700000], vfIle[100000];
    int sd, connfd, len, alen;

    for (int i = 0; i < 100; i++) {
        fileName[i] = '\0';
    }

    struct sockaddr_in servaddr, cliaddr;

    // ----------------------creation of socket----------------------
    sd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sd == -1) {
        printf("Socket not created in server\n");
        exit(0);
    } else {
        printf("Socket created in server\n");
    }

    bzero(&servaddr, sizeof(servaddr));

    // Add the parameters to the socket
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY; // For local host
    servaddr.sin_port = htons(8000); // Set the port number

    memset(&(servaddr.sin_zero), '\0', 8);

    // ----------------------bind the socket----------------------
    if (bind(sd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
        printf("Not binded\n");
    } else {
        printf("Binded\n");
    }

    len = sizeof(cliaddr);

    int choice = -1;
    while (1) {
        char num;
        recvfrom(sd, &num, sizeof(num), 0, (struct sockaddr *)&cliaddr, &len);
        choice = num;

        switch (choice) {
            case 1:
                recvfrom(sd, fileName, 1024, 0, (struct sockaddr *)&cliaddr, &alen);
                printf("NAME OF TEXT FILE RECEIVED : %s\n", fileName);
                FILE *fp;
                printf("Contents in the received text file :\n");
                recvfrom(sd, fileBuffer, 1024, 0, (struct sockaddr *)&cliaddr, &alen);
                printf("%s\n", fileBuffer);
                int fsz = strlen(fileBuffer);
                fp = fopen(fileName, "w");
                if (fp) {
                    fwrite(fileBuffer, fsz, 1, fp);
                    printf("File received successfully.\n");
                } else {
                    printf("Cannot create output file.\n");
                }
                memset(fileName, '\0', sizeof(fileName));
                fclose(fp);
                break;

            case 2:
                recvfrom(sd, fileName, 1024, 0, (struct sockaddr *)&cliaddr, &alen);
                printf("NAME OF AUDIO FILE RECEIVED : %s\n", fileName);
                FILE *afp;
                int numbers;
                afp = fopen(fileName, "w");
                if (afp) {
                    int afsize = recvfrom(sd, auFile, sizeof(auFile), 0, (struct sockaddr *)&cliaddr, &alen);
                    fwrite(auFile, afsize, 1, afp);
                    printf("File received successfully.\n");
                } else {
                    printf("Cannot open output file.\n");
                }
                memset(fileName, '\0', sizeof(fileName));
                fclose(afp);
                break;

            case 3:
                recvfrom(sd, fileName, 1024, 0, (struct sockaddr *)&cliaddr, &alen);
                printf("VIDEO FILE NAME RECEIVED : %s\n", fileName);
                FILE *vfp;
                vfp = fopen(fileName, "w");
                size_t vfsize;
                vfsize = recvfrom(sd, vfIle, sizeof(vfIle), 0, (struct sockaddr *)&cliaddr, &alen);
                if (vfp) {
                    fwrite(vfIle, vfsize, 1, vfp);
                    printf("File received successfully.\n");
                } else {
                    printf("Cannot open output file.\n");
                }
                memset(fileName, '\0', sizeof(fileName));
                fclose(vfp);
                break;
        }
    }

    close(sd);
    return 0;
}
