# README for CSCE 3530 – Programming Assignment #3

## 1. Overview
This project implements a basic C-based web server that:
- Listens on a specified port (using TCP).
- Handles HTTP GET requests from a web browser.
- Serves static `.html` files from the server’s working directory.
- Returns a 404 page if the requested file is not found.
- Shuts down the server gracefully when `exit.html` is requested.

---

## 2. File List
Inside this folder (`SE0598_PA3`), you should see:
- **`wbserver.c`**:  
  The main server source file that implements a minimal HTTP server.
- **`404.html`**:  
  The file returned if a requested page is not found. Must include:  
  `<h1>404 Requesting page not found.</h1>`
- **`exit.html`**:  
  The page that triggers a graceful shutdown. Must include:  
  `<h1>The web server is shutting down...</h1>`
- **`Makefile`**:  
  Used to compile (`make`) and clean (`make clean`) the project.
- **`README.md`**:  
  This file, which explains how to compile, run, and test the code.

> **Note**: You may also add other `.html` files (like `index.html`, `stats.html`, etc.) to test dynamic serving of HTML content.

---

## 3. How to Compile
Make sure you are on one of the Cell machines (`cell01` through `cell18`)(preferably `cell12`) or another Linux environment with a C compiler (`gcc`).

1. Navigate into the folder containing this project (e.g., `SE0598_PA3`).
2. Type:
   ```bash
   make
   ```
This will compile `wbserver.c` and produce an executable named `wbserver`.

To clean up (remove) the generated executable, type:
   ```bash
   make clean
   ```

## 4. How to Run
After compiling, run the server:
   ```bash
   ./wbserver
   ```
The program will prompt you for a port number:
   ```bash
   Enter the port number to run the web server on:
   ```
Enter a valid port (e.g., 8080 or another port not in use, such as 12348).

The server will bind to that port and start listening:
   ```bash
   Web server started on port 12348. Waiting for connections...
   ```

## 5. How to Test with a Browser
Ensure your `.html` files (e.g., `exit.html`, `404.html`) are in the same directory from which you launched `./wbserver`.

Open a browser on the same machine, or on another machine that can reach the server (you might need to open firewall ports if off-campus or behind a firewall).

In the browser’s address bar, navigate to:
   ```bash
   http://http://cell12-cse.eng.unt.edu:12348/index.html
   ```

Check the server’s console output for messages indicating:
   ```bash
   “Requested Page: …”
   ```
   ```bash
   “Status: Found and serviced” for pages that exist
   ```
   ```bash
   “Status: Not found and sent 404.html” for missing pages
   ```

If you request `exit.html` (and that file exists), the server will send it and then gracefully shut down:
   ```bash
   Requested Page: exit.html
   Status: Found and stopping the web server!
   ```

## 6. Common Issues
### Port Already in Use
If you see a “bind failed” error, try a different port or ensure no other server is running on the same port.

### 404 Even Though File Exists
Make sure the `.html` files are in the same folder and the filename matches exactly (case-sensitive).

### Permissions
Ensure your `.html` files are readable (`chmod 644 *.html`).

### Networking
If you test from a different computer, make sure firewalls and network settings allow incoming connections on the chosen port.

## 7. Example Usage
   ```bash
   # compile
   make

   # run
   ./wbserver
   Enter the port number to run the web server on: 12348
   Web server started on port 12348. Waiting for connections...
   ```
In a browser, go to:
   ```bash
   http://cell12-cse.eng.unt.edu:12348/index.html
   ```
Server output might say:
   ```bash
   Requested Page: index.html
   Status: Found and serviced
   ```
If you go to:
   ```bash
    http://cell12-cse.eng.unt.edu:12348/test.html
   ```
And `test.html` doesn’t exist:
   ```bash
   Requested Page: test.html
   Status: Not found and sent 404.html
   ```
Finally, going to:
   ```bash
   http://cell12-cse.eng.unt.edu:12348/exit.html
   ```
might yield:
   ```bash
   Requested Page: exit.html
   Status: Found and stopping the web server!
   Server has shut down.
   ```
