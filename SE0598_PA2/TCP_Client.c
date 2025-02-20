#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define BUFFER_SIZE 4096  // for 4KB chunks
#define FILE_SIZE 16384    // 16KB file

// XOR encryption function
void xor_encrypt_decrypt(char *data, int size, char key) {
    for (int i = 0; i < size; i++) {
        data[i] ^= key;
    }
}

int main() {
    // create variables to store user input and other data
    char server_ip[16];
    char input_file_name[256], output_file_name[256];
    char encryption_key;
    int port;
    char encrypted_file_name[300];  

    // Get user input
    printf("Enter the server IP address: ");
    scanf("%15s", server_ip);

    printf("Enter the server port number: ");
    scanf("%d", &port);

    printf("Enter the input file name: ");
    scanf("%255s", input_file_name);

    printf("Enter the output file name: ");
    scanf("%255s", output_file_name);

    printf("Enter the encryption key: ");
    scanf(" %c", &encryption_key);  // Space before %c handles newline issues

    // Create socket and connect to server
    int sockfd;
    struct sockaddr_in servaddr;
    char file_buffer[FILE_SIZE];
    char buffer[BUFFER_SIZE];

    struct timeval start, end;
    double rtt, end_to_end, throughput;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Set server address
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    inet_pton(AF_INET, server_ip, &(servaddr.sin_addr));

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    //printf("Connected to server. Sending file...\n"); // echo print

    // Read input file
    FILE *fp = fopen(input_file_name, "rb");
    if (!fp) {
        perror("Error opening input file");
        exit(1);
    }
    fread(file_buffer, 1, FILE_SIZE, fp);
    fclose(fp);

    // Encrypt data
    xor_encrypt_decrypt(file_buffer, FILE_SIZE, encryption_key);

    // Format the new filename dynamically
    snprintf(encrypted_file_name, sizeof(encrypted_file_name), "encrypted_%s", input_file_name);

    printf("Encrypted file name: %s\n", encrypted_file_name); // To check the encrypted files name
    // Open the file with the new name
    fp = fopen(encrypted_file_name, "wb");
    if (!fp) {
        perror("Error opening encrypted file");
        exit(1);
    }
    
    // Write encrypted data to the file
    fwrite(file_buffer, 1, FILE_SIZE, fp);
    fclose(fp);

    // Start timer before sending
    gettimeofday(&start, NULL);

    // Send 16 KB data in 4 KB chunks
    int sent_size = 0;
    while (sent_size < FILE_SIZE) {
        int chunk_size = (FILE_SIZE - sent_size < BUFFER_SIZE) ? FILE_SIZE - sent_size : BUFFER_SIZE;
        write(sockfd, file_buffer + sent_size, chunk_size);
        sent_size += chunk_size;
    }

    //printf("File sent. Waiting for response...\n"); // echo print

    // Receive 16 KB data in 4 KB chunks
    int received_size = 0;
    while (received_size < FILE_SIZE) {
        int bytes = read(sockfd, buffer, BUFFER_SIZE);
        
        if (bytes <= 0) break;

        // Insert each byte into file_buffer 
        for (int i = 0; i < bytes; i++) {
            file_buffer[received_size + i] = (buffer[i] & 0xFF);  
        }

        received_size += bytes;
    }

    // Stop timer after receiving
    gettimeofday(&end, NULL);

    // Compute RTT
    rtt = ((end.tv_sec - start.tv_sec) * 1000.0) + ((end.tv_usec - start.tv_usec) / 1000.0); //tv_sec is for seconds and tv_usec is for microseconds
    // compute ETE                                                                           //both are converted to miliseconds and added together
    end_to_end = rtt / 2.0;
    // compute throughput
    throughput = (FILE_SIZE * 8.0) / (end_to_end * 1000.0);  // Mbps

    // print the results
    printf("Round Trip Time: %.3f ms\n", rtt);
    printf("End-to-End Time: %.3f ms\n", end_to_end);
    printf("Throughput: %.3f Mbps\n", throughput);

    // Decrypt received file
    xor_encrypt_decrypt(file_buffer, FILE_SIZE, encryption_key);

    // Save decrypted file
    fp = fopen(output_file_name, "wb");
    fwrite(file_buffer, 1, FILE_SIZE, fp);
    fclose(fp);

    //printf("File transfer complete. Closing connection.\n");// echo print

    close(sockfd); // Close socket
    return 0;
}
