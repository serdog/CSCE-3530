/* Client Code */
/* Connect to server listening on cell01 port 3169 */

/* Fixed TCP Client Code with User-Specified Port and Quit Handling */
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "10.144.192.226" // Replace with the correct server IP if needed

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port_number>\n", argv[0]);
        exit(1);
    }

    int sockfd, n, port;
    char input[256], recvline[256];
    struct sockaddr_in servaddr;

    port = atoi(argv[1]); // Get port number from command line argument

    /* Create socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);

    /* Convert IP address */
    inet_pton(AF_INET, SERVER_IP, &(servaddr.sin_addr));

    /* Connect to the server */
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    printf("Connected to the server on port %d. Type 'quit' to exit.\n", port);

    while (1) {
        printf("Enter a sentence: ");
        fgets(input, sizeof(input), stdin);

        /* Send input to the server */
        write(sockfd, input, strlen(input));

        /* Check for quit command */
        if (strcmp(input, "quit\n") == 0 || strcmp(input, "quit") == 0) {
            printf("Quit command received. Closing client.\n");
            break;
        }

        /* Read response from server */
        bzero(recvline, sizeof(recvline));
        n = read(sockfd, recvline, sizeof(recvline) - 1);
        if (n > 0) {
            recvline[n] = '\0';
            printf("%s", recvline);
        } else {
            printf("Server connection lost. Exiting...\n");
            break;
        }
    }

    close(sockfd);
    return 0;
}
