Project Name: UDP Client-Server Communication

Author: ZAIDI Mohamed & YANNIS MRAIHI

Date: 30 mai 2023

Version: 1.0

----------------------------------------------------
DESCRIPTION

This project demonstrates a client-server communication using UDP (User Datagram Protocol). The server and client programs are implemented in C language. The server program listens for client requests, performs specific operations based on the request, and sends a response back to the client. The client program allows users to send requests to the server and display the server's response.

----------------------------------------------------
REQUIREMENTS

To compile and run this project, you need:

1. C Compiler (e.g., GCC)
2. Unix-like operating system (Linux, macOS) or a compatible environment (e.g., Cygwin on Windows)

----------------------------------------------------
FILES

The project consists of the following files:

1. server.c - Contains the implementation of the server program
2. client.c - Contains the implementation of the client program
3. header.h - Header file containing function prototypes and necessary includes for the server and client programs

----------------------------------------------------
COMPILING THE PROJECT


Run the MAKE commande on your terminal or Follow these steps to compile the project:  

1. Open a terminal or command prompt.
2. Navigate to the project directory.
3. Compile the server program by executing the following command:
     gcc server.c -o server
4. Compile the client program by executing the following command:
     gcc client.c -o client

----------------------------------------------------
RUNNING THE PROJECT

To run the project, follow these steps:

1. Open two separate terminal windows or command prompts.
2. In the first window, navigate to the project directory.
3. Start the server by running the following command:
     ./server
   The server will start listening for incoming client requests.
4. In the second window, navigate to the project directory.
5. Run the client program by executing the following command:
     ./client
   The client program will display a menu of available requests.
6. Enter a request according to the provided syntax and press Enter.
   The client will send the request to the server and display the server's response.
7. Repeat step 6 to send more requests and receive responses.

----------------------------------------------------
SUPPORTED REQUESTS

The client program supports the following requests:

1. AJOUT <id_client> <id_compte> <password> <amount>
   - Adds the specified amount to the account identified by the client ID and account ID.

2. RETRAIT <id_client> <id_compte> <password> <amount>
   - Withdraws the specified amount from the account identified by the client ID and account ID.

3. SOLDE <id_client> <id_compte> <password>
   - Retrieves the balance of the account identified by the client ID and account ID.

4. OPERATIONS <id_client> <id_compte> <password>
   - Retrieves the list of the last 10 operations performed on the account identified by the client ID and account ID.

Note: Replace <id_client>, <id_compte>, <password>, and <amount> with the actual values when sending requests.

----------------------------------------------------
CONTACT

For any inquiries or further information, feel free to contact the project author:

ZAIDI Mohamed or Yannis MRAIHI
zaidimedh@gmail.com
----------------------------------------------------
