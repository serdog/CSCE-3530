/* Server Code */
/* Run the server on cell04 and the server listens on port 3169 */

/* Count Server Code with TCP Socket Implementation */
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>

#define MAX_BUFFER 256

// Function to convert a string to lowercase
void to_lowercase(char *str) {
    while (*str) {
        if (*str >= 'A' && *str <= 'Z') {
            *str = *str + ('a' - 'A');
        }
        str++;
    }
}

// Function to count vowels in a given string
int count_vowels(const char *str) {
    int count = 0;
    while (*str) {
        char c = *str | 32;
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            count++;
        }
        str++;
    }
    return count;
}

// Function to convert number words to digits (0-99)
void convert_numbers(char *str) {
    const char *words[] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten",
                           "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen",
                           "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"};
    int values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 30, 40, 50, 60, 70, 80, 90};
    
    char result[MAX_BUFFER] = "";
    char temp[MAX_BUFFER];
    strcpy(temp, str);
    to_lowercase(temp);

    char *tokens[MAX_BUFFER / 2];
    int token_count = 0;
    char *token = strtok(temp, " ");
    while (token && token_count < MAX_BUFFER / 2) {
        tokens[token_count++] = token;
        token = strtok(NULL, " ");
    }

    int last_value = -1;
    int has_last_value = 0;

    for (int i = 0; i < token_count; i++) {
        int len = strlen(tokens[i]);

        // Remove punctuation if present at the end of the word
        if (len > 0 && (tokens[i][len - 1] < 'a' || tokens[i][len - 1] > 'z')) {
            tokens[i][len - 1] = '\0';
        }

        int found = -1;
        for (int j = 0; j < 28; j++) {
            if (strcmp(tokens[i], words[j]) == 0) {
                found = values[j];
                break;
            }
        }

        if (found != -1) { 
            if (has_last_value && last_value >= 20 && found < 10) {  
                last_value += found;
            } else {
                if (has_last_value) {
                    char num_str[5];
                    sprintf(num_str, "%d", last_value);
                    strcat(result, num_str);
                    strcat(result, " ");
                }
                last_value = found;
                has_last_value = 1;
            }
        } else { 
            if (has_last_value) {  
                char num_str[5];
                sprintf(num_str, "%d", last_value);
                strcat(result, num_str);
                strcat(result, " ");
                has_last_value = 0;
            }
            strcat(result, tokens[i]);
            strcat(result, " ");
        }
    }

    if (has_last_value) {  // Ensure last number is appended
        char num_str[5];
        sprintf(num_str, "%d", last_value);
        strcat(result, num_str);
    }

    strcpy(str, result);
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port_number>\n", argv[0]);
        exit(1);
    }

    int port = atoi(argv[1]);
    int listen_fd, conn_fd;
    struct sockaddr_in servaddr;
    char buffer[MAX_BUFFER];
    char original[MAX_BUFFER];

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);

    if (bind(listen_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    listen(listen_fd, 10);
    printf("Server started. Listening on port %d...\n", port);

    while (1) {
        printf("Waiting for a client to connect...\n");
        conn_fd = accept(listen_fd, (struct sockaddr *)NULL, NULL);
        if (conn_fd < 0) {
            perror("Accept failed");
            continue;
        }

        printf("Client connected!\n");
        while (1) {
            bzero(buffer, MAX_BUFFER);
            int n = read(conn_fd, buffer, sizeof(buffer) - 1);
            if (n <= 0) {
                printf("Client disconnected.\n");
                close(conn_fd);
                break;
            }

            buffer[n] = '\0';
            strcpy(original, buffer);
            printf("Received from client: %s\n", buffer);

            if (strncmp(buffer, "quit", 4) == 0) {
                printf("Server shutting down.\n");
                write(conn_fd, "cya!\n", 5);
                close(conn_fd);
                return 0;
            }

            convert_numbers(buffer);
            int vowel_count = count_vowels(original);
            char response[MAX_BUFFER];
            snprintf(response, sizeof(response), "Processed: %s\nNumber of vowels: %d\n", buffer, vowel_count);
            write(conn_fd, response, strlen(response));
        }
    }

    close(listen_fd);
    return 0;
}
