# TCP Count Server & Client

## **Description**
This project implements a TCP-based client-server application where:
- The **server** listens for connections, gets input from clients, and send it back
- The **client** connects to the server, sends 4KB chunks of encrypted data, and receives the same chunks back.
- The server exits when it sends back all of the 4KB chunks (16KB's in total)
-The client exits when it receives the data from the server and outputs the calculated results onto the terminal.

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
and the local IP adress to use on the client side (use this while running the client)

```sh
make client
```
This generates only the client executable
- `client` 
```sh
make server
```
This generates only the server executable:
- `server` 
To clean up compiled files, run:
```sh
make clean
```

## **Usage**
### **Starting the Server**
Run the server with:
```sh
./server
Enter the server port number: <port number>
```
Example:
```sh
./server
Enter the server port number: 8080
```
The server will start and wait for client connections.

### **Running the Client**
Run the client in a seperate window with: (use the IP address given when make is used)
```sh
./client
Enter the server IP address: <IP adress>
Enter the server port number: <port number>
Enter the input file name: <input file name>
Enter the output file name: <output file name>
Enter the encryption key: <encryption key(single ASCII character)>
```
Example:
```sh
./client
Enter the server IP address: 10.144.192.224 (cell machine 2)
Enter the server port number: 8080
Enter the input file name: input.txt
Enter the output file name: output.txt
Enter the encryption key: a
```
The client will output the results 

### **Example Interaction**
#### **Client Input:**
```sh
./client
Enter the server IP address: 10.144.192.224
Enter the server port number: 8080
Enter the input file name: input.txt
Enter the output file name: output.txt
Enter the encryption key: a
Round Trip Time: 23.601 ms
End-to-End Time: 11.800 ms
Throughput: 11.107 Mbps
File transfer complete. Closing connection.
```
#### **Server Response:**
```sh
Server started. Listening on port 8080...
Client connected! Receiving file...
File received. Echoing back to client...
File echoed back successfully.
```

## **Exit Condition**
- The **client** can exit when it outputs all of the results.
- The **server** shuts down after it sends all of the data.

## **Notes**
- The server must be started before the client and must be started on any cell machine.
- The client can connect from any cell machine.
- The program ensures a stable connection and sends back the recieved data

