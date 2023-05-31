#ifndef HEADER_H
#define HEADER_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>

#define MAXPENDING 5    /* Max connection requests */
#define BUFFSIZE 1024
#define port 8081


typedef struct operations_ {
    char type_operation[10];
    char date_operation[20];
    double operation_amount;
} Operations;

typedef struct account_{
    int id_client;
    int id_account;
    char password[32];
    double balance;
    Operations operations[11];
    int Operation_nbr;
} Account;
Account account_db[] = {
    // {id_client, id_compte, password, solde, {{OPERATIONS}}, nombre_operations}
    {1, 1001, "password1", 5000.0, {{"", "", 0}}, 0},
    {2, 1002, "password2", 3000.0, {{"", "", 0}}, 0},
    {3, 0000, "p", 3999.0, {{"", "", 0}}, 0}
};
int account_db_nbr = sizeof(account_db) / sizeof(account_db[0]);

void save_operation(Account *Account, const char *type_operation, double amount);
Account* find_accound_by_ID(int id_client, int id_Account, const char *password);
int AJOUT(int id_client, int id_Account, const char *password, double somme);
int RETRAIT(int id_client, int id_Account, const char *password, double somme);
double SOLDE(int id_client, int id_Account, const char *password);
char *OPERATIONS(int id_client, int id_Account, const char *password, char *buffer, size_t buffer_size);
void Die(char *mess);
void HandleClient(int sock);

void save_operation(Account* account, const char* type_operation, double amount) {
    Operations* operation = &(account->operations[account->Operation_nbr % 10]);
    strncpy(operation->type_operation, type_operation, sizeof(operation->type_operation) - 1); // avoid overflow
    operation->type_operation[sizeof(operation->type_operation) - 1] = '\0'; //specify the last caracter as NULL
    time_t current_time = time(NULL);
    strftime(operation->date_operation, sizeof(operation->date_operation), "%Y-%m-%d %H:%M:%S", localtime(&current_time));
    operation->operation_amount = amount;
    account->Operation_nbr++;}

Account* find_account_by_ID(int id_client, int id_account, const char* password) {
    for (int i = 0; i < account_db_nbr; i++) {
        if (account_db[i].id_client == id_client && account_db[i].id_account == id_account &&
    strcmp(account_db[i].password, password) == 0) {
    return &account_db[i];
        }
    }
    return NULL;
}

int AJOUT(int id_client, int id_account, const char* password, double amount) {
    Account* account = find_account_by_ID(id_client, id_account, password);
    if (account) {
        account->balance += amount;
        // Record the operation
        save_operation(account, "AJOUT", amount);
        return 1;
    }
    return -1; //account is NULL
}

int RETRAIT(int id_client, int id_account, const char* password, double amount) {
    Account* account = find_account_by_ID(id_client, id_account, password);
    if (account && account->balance >= amount) {
        account->balance -= amount;
        save_operation(account, "RETRAIT", amount); // Record the operation
        return 1;
    }
    return 0;
}

double SOLDE(int id_client, int id_account, const char* password) {
    Account* account = find_account_by_ID(id_client, id_account, password);
    if (account) {
        return account->balance;
    }
    return -1.0;
}

char* OPERATIONS(int id_client, int id_account, const char* password, char* buffer, size_t buffer_size) {
    Account* account = find_account_by_ID(id_client, id_account, password);
    if (account) {
        int start = (account->Operation_nbr > 10) ? account->Operation_nbr - 10 : 0;
        buffer[0] = '\0';
        for (int i = start; i < account->Operation_nbr; i++) {
            Operations* operation = &(account->operations[i % 10]);
            char line[128];
            snprintf(line, sizeof(line), "%s %s %.3lf\n", operation->type_operation, operation->date_operation, operation->operation_amount);
            strncat(buffer, line, buffer_size - strlen(buffer) - 1);
        }
        return buffer;
    }
    return NULL;
}
void HandleClient(int sock) {
    char buffer[BUFFSIZE];
    int received = -1;
    char *message_client = "\nServer status READY";
    /* Receive message */
    if ((received = recv(sock, buffer, BUFFSIZE, 0)) < 0) {
      Die("Failed to receive initial bytes from client");
    }
    printf("buffer Message received is: %s\n", buffer);
    /* Send bytes and check for more incoming data in loop */
    while(received > 0){
      /* Send back received data */
      if (send(sock, buffer, received, 0) != received) {
        Die("Failed to send bytes to client");
      }
      buffer[received] = '\0';
      //send(sock,message_client, strlen(message_client), 0);
      printf("\nMessage sent back is : %s\n", buffer);
      /* Check for more data */
      if ((received = recv(sock, buffer, BUFFSIZE, 0)) < 0) {
        Die("Failed to receive additional bytes from client");
      }
    }
        close(sock);
    }
    void Die(char *mess)
    {
        perror(mess);
        exit(1);
    }

#endif // SERVER_H
