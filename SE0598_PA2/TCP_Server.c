#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 4096  // 4 KB chunks
#define FILE_SIZE 16384    // 16 KB file

int main() {
    // Get port number from user
    int port;
    printf("Enter the port number: ");
    scanf("%d", &port);

    // Create socket and bind to port
    int listen_fd, conn_fd;
    struct sockaddr_in servaddr;
    char buffer[BUFFER_SIZE];
    char file_buffer[FILE_SIZE];
    int received_size = 0;

    // Create socket
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Bind to port
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);

    if (bind(listen_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    // Listen for incoming connections
    listen(listen_fd, 1);
    printf("Server started. Listening on port %d...\n", port);

    // Accept connection
    conn_fd = accept(listen_fd, (struct sockaddr *)NULL, NULL);
    if (conn_fd < 0) {
        perror("Accept failed");
        exit(1);
    }

    printf("Client connected! Receiving file...\n"); // echo print

    // Receive 16 KB data in 4 KB chunks
    while (received_size < FILE_SIZE) {
        int bytes = read(conn_fd, buffer, BUFFER_SIZE);
        
        if (bytes <= 0) break;

        // Insert each byte into file_buffer using bitwise operations
        for (int i = 0; i < bytes; i++) {
            file_buffer[received_size + i] = (buffer[i] & 0xFF);  // Ensuring byte-level write
        }

        received_size += bytes;
    }


    printf("File received. Echoing back to client...\n"); // echo print

    // Echo back the received file in 4 KB chunks
    int sent_size = 0;
    while (sent_size < FILE_SIZE) {
        int chunk_size = (FILE_SIZE - sent_size < BUFFER_SIZE) ? FILE_SIZE - sent_size : BUFFER_SIZE;
        write(conn_fd, file_buffer + sent_size, chunk_size);
        sent_size += chunk_size;
    }

    printf("File echoed back successfully.\n");// echo print
    close(conn_fd); // Close connection
    close(listen_fd); // Close socket
    
    return 0;
}
