README

This repository contains the source code for a client-server TCP application. The application allows clients to interact with a server by performing various banking operations.

To compile and run the code, please follow the instructions below.

1. Clone the repository:
   $ git clone https://github.com/wolfyc/TCP_UDP_BankAccountApp.git
   $ cd repo

2. Make sure you have the GCC compiler installed on your system.

3. Compile the code:
   $ make

   The Makefile provided in the repository will compile the source files and generate the executable files in a folder named "executable_files". If the folder does not exist, it will be created.

4. Start the server:
   $ ./executable_files/server

   The server will start running and will be ready to accept client connections.

5. Open a new terminal and run the client:
   $ ./executable_files/client

   The client program will start and prompt you to enter commands for banking operations.

   Example usage:
   > AJOUT 1 1 p1 1000
   > SOLDE 1 1 p1
   > RETRAIT 1 1 p1 500
   > OPERATIONS 1 1 p1

   Please note that the format for the commands is:
   > OPERATION_TYPE CLIENT_ID ACCOUNT_ID PASSWORD [AMOUNT]

   The available operation types are: AJOUT, RETRAIT, SOLDE, and OPERATIONS.

6. Follow the prompts in the client program to perform various banking operations.

7. To stop the server, press Ctrl+C in the terminal where the server is running.

If you encounter any issues or have any questions, please feel free to contact us.

Enjoy using the client-server TCP banking application!