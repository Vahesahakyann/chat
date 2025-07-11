⚙️ Construction Details
This project is a basic client-server chat system built using sockets and the poll() system call to handle multiple client connections concurrently in a single-threaded environment. Here's how it's structured and works step by step:

🧱 Server Construction
Socket Setup

A TCP socket is created using socket(AF_INET, SOCK_STREAM, 0).

It binds to 127.0.0.1:7777 using bind() and begins listening with listen().

Poll Setup

One pollfd is allocated for the server socket (to accept new clients).

An array of 10 pollfd structures is created to manage up to 10 clients.

Client Connection

When poll() detects POLLIN on the server socket, it calls accept() to accept a new client.

The server immediately reads the first message from the new client as the username, and stores it.

Message Handling Loop

poll() checks all client sockets for incoming messages (POLLIN).

For each readable client:

It reads up to 4096 bytes.

If the message contains '>', it splits the string:

Left part is the target username.

Right part is the message content.

It then sends the message only to the user with the matching username.

If '>' is not found, the server treats it as a broadcast and sends it to all other clients except the sender.

Client Disconnects

If poll() detects POLLHUP or read() returns 0, the client has disconnected.

The server closes that file descriptor and marks it as free.

🧱 Client Construction
Socket Connection

Client creates a socket and connects to 127.0.0.1:7777.

Username Registration

Prompts the user to enter a username.

Sends the username to the server immediately after connection.

Communication Loop

Uses poll() to handle both reading from server and writing to it:

If POLLIN is set, it reads messages from server and prints to the terminal.

If POLLOUT is set, it prompts the user to type a message and sends it.

Disconnection

If the server goes down or closes the socket, poll() returns POLLHUP, and the client exits gracefully.

🗂 Memory Management
char** checknames is a 2D dynamic array (allocated via create2dim()) used to store usernames.

Each client socket and its associated state is stored in the struct pollfd client[10] array.

Buffers are dynamically allocated with malloc() and managed carefully to avoid overflows.
