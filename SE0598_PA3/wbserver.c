#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <netdb.h>
#include <errno.h>
#include <sys/stat.h>

// You may adjust these if desired:
#define MAX_REQUEST_SIZE 8192
#define SERVER_BACKLOG   5

// Function to get the current time in HTTP date format.
void get_gmt_date(char *date_str, size_t max_size) {
    // Get system time
    time_t now = time(NULL);
    struct tm gm_time;
    gmtime_r(&now, &gm_time);
    // Format into HTTP-style date, e.g. "Wed, 26 Mar 2025 22:04:41 GMT"
    strftime(date_str, max_size, "%a, %d %b %Y %H:%M:%S GMT", &gm_time);
}

// Function to send a file (and build the proper header).
// Returns 1 if the requested file was "exit.html", 0 otherwise.
int send_file_response(int client_fd, const char *filename) {
    // If the requested file is exit.html, we want to shut down after sending
    int shutdown_flag = 0;
    if (strcmp(filename, "exit.html") == 0) {
        shutdown_flag = 1;
    }

    // Try opening the requested file
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        // ---- FILE NOT FOUND => send a 404 response ----

        // Attempt to open 404.html
        FILE *fp404 = fopen("404.html", "rb");
        size_t filelen = 0;

        if (fp404) {
            fseek(fp404, 0, SEEK_END);
            filelen = ftell(fp404);
            fseek(fp404, 0, SEEK_SET);
        }

        // Prepare a 404 header
        char date_str[128];
        get_gmt_date(date_str, sizeof(date_str));

        char header[512];
        snprintf(header, sizeof(header),
                 "HTTP/1.1 404 Not Found\r\n"
                 "Content-Type: text/html\r\n"
                 "Date: %s\r\n"
                 "Content-Length: %zu\r\n"
                 "\r\n",
                 date_str, (size_t)filelen);

        write(client_fd, header, strlen(header));

        // If we have a 404.html file, send it; otherwise, send fallback text
        if (!fp404) {
            const char *fallback = "<h1>404 Page Not Found</h1>\n";
            write(client_fd, fallback, strlen(fallback));
        } else {
            char filebuf[1024];
            size_t bytes_read;
            while ((bytes_read = fread(filebuf, 1, sizeof(filebuf), fp404)) > 0) {
                write(client_fd, filebuf, bytes_read);
            }
            fclose(fp404);
        }

        // Return a special code to indicate 404
        return 2;
    }

    // ---- FILE FOUND => send a 200 OK response ----

    // If it's exit.html, remember that for returning 1
    struct stat st;
    stat(filename, &st);
    size_t file_size = st.st_size;

    char date_str[128];
    get_gmt_date(date_str, sizeof(date_str));

    // Minimal content-type detection
    const char *content_type = "text/html";

    char header[512];
    snprintf(header, sizeof(header),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: %s\r\n"
             "Date: %s\r\n"
             "Content-Length: %zu\r\n"
             "\r\n",
             content_type, date_str, file_size);

    write(client_fd, header, strlen(header));

    // Send file data
    char filebuf[4096];
    size_t bytes_read;
    while ((bytes_read = fread(filebuf, 1, sizeof(filebuf), fp)) > 0) {
        write(client_fd, filebuf, bytes_read);
    }
    fclose(fp);

    // If this was exit.html, return 1; else 0 for success
    return shutdown_flag ? 1 : 0;
}


int main() {
    // Prompt user for port
    int port;
    printf("Enter the port number to run the web server on: ");
    scanf("%d", &port);

    // Create a socket
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        perror("socket() failed");
        exit(EXIT_FAILURE);
    }

    // Allow reuse of the address (avoid "Address already in use" errors)
    int optval = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    // Bind to the requested port
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port        = htons(port);

    if (bind(listen_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind() failed");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(listen_fd, SERVER_BACKLOG) < 0) {
        perror("listen() failed");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    printf("Web server started on port %d. Waiting for connections...\n", port);

    int stop_server = 0;
    while (!stop_server) {
        // Accept a new connection
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        int client_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &addr_len);
        if (client_fd < 0) {
            perror("accept() failed");
            continue; // keep going
        }

        // We have a new connection, read the HTTP request
        char request_buf[MAX_REQUEST_SIZE];
        memset(request_buf, 0, sizeof(request_buf));

        // Read up to MAX_REQUEST_SIZE - 1 bytes
        ssize_t bytes_received = read(client_fd, request_buf, sizeof(request_buf) - 1);
        if (bytes_received <= 0) {
            // Something went wrong or client disconnected
            close(client_fd);
            continue;
        }

        // We expect something like: "GET /index.html HTTP/1.1\r\nHost: ...\r\n..."
        // Find the requested file name (between "GET " and " HTTP/")
        // This is a simplistic approach—real HTTP parsing can be more complex.
        char method[8], path[1024], version[16];
        sscanf(request_buf, "%s %s %s", method, path, version);

        // Typically, `path` will be something like "/index.html"
        // We want to skip the leading '/' if present
        char *file_requested = path;
        if (path[0] == '/') {
            file_requested = &path[1];
        }
        if (strlen(file_requested) == 0) {
            // If path is "/", let's default to "index.html"
            file_requested = "index.html";
        }

        printf("\nRequested Page: %s\n", file_requested);

        // Send the requested file (or a 404 response)
        int code = send_file_response(client_fd, file_requested);
        switch (code) {
            case 2:
                // 2 => we just sent a 404 response
                printf("Status: Not found and sent 404.html\n");
                break;
            case 1:
                // 1 => user requested exit.html => shut down after sending
                printf("Status: Found and stopping the web server!\n");
                stop_server = 1;
                break;
            default:
                // 0 => success, we found and served the file
                printf("Status: Found and serviced\n");
                break;
        }

        // Close the connection after serving
        close(client_fd);

    }

    // Cleanup
    close(listen_fd);
    printf("Server has shut down.\n");
    return 0;
}
