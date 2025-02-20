# Compiler
CC = gcc
CFLAGS = -Wall -Wextra -g

# Source files
CLIENT_SRC = TCP_Client.c
SERVER_SRC = TCP_Server.c

# Executable names
CLIENT_EXE = client
SERVER_EXE = server

# Default target: Compile both client and server **only if needed**
all: $(CLIENT_EXE) $(SERVER_EXE)
	@echo "Local IP Address: " `hostname -I`

# Compile the client only if needed
$(CLIENT_EXE): $(CLIENT_SRC)
	$(CC) $(CFLAGS) -o $(CLIENT_EXE) $(CLIENT_SRC)

# Compile the server only if needed
$(SERVER_EXE): $(SERVER_SRC)
	$(CC) $(CFLAGS) -o $(SERVER_EXE) $(SERVER_SRC)

# Clean up executables and user-generated files
clean:
	rm -f $(CLIENT_EXE) $(SERVER_EXE) *.txt encrypted_*

# .PHONY ensures `clean` always runs
.PHONY: all clean client server
