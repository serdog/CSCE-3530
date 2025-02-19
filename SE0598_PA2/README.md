# TCP Count Server & Client

## **Description**
This project implements a TCP-based client-server application where:
- The **server** listens for connections, processes input sentences from clients, and converts number words (0-99) into digits while counting vowels in the sentence.
- The **client** connects to the server, sends sentences, and receives processed responses.
- The server and client programs exit when the user inputs `quit`.

## **Files**
- `TCP_Server.c`: Implements the server (`server`).
- `TCP_Client.c`: Implements the client (`client`).
- `Makefile`: Automates the compilation process.

## **Compilation**
To compile both the server and client, run:
```sh
make
```
This generates two executables:
- `server` 
- `client` 

To clean up compiled files, run:
```sh
make clean
```

## **Usage**
### **Starting the Server**
Run the server with:
```sh
./ctserver <port_number>
```
Example:
```sh
./ctserver 3169
```
The server will start and wait for client connections.

### **Running the Client**
Run the client in a seperate window with:
```sh
./client <port_number>
```
Example:
```sh
./client 3169
```
The client will prompt the user to enter a sentence.

### **Example Interaction**
#### **Client Input:**
```sh
Input: The cost for ten oranges is fifteen dollars!
```
#### **Server Response:**
```sh
Processed: The cost for 10 oranges is 15 dollars!
Number of vowels: 13
```
#### **Client Input:**
```sh
Input: quit
```
then the server exits and displays in its own window
```
Server shutting down.
```

## **Exit Condition**
- The **client** can exit by entering `quit`.
- The **server** shuts down when a client sends `quit`.

## **Notes**
- The server must be started before the client and must be started on cell04.
- The client can connect from any cell machine.
- The program ensures all number words (0-99) are converted properly.

