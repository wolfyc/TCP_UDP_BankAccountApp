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
#define PORT 8081

typedef struct operations_ {
    char type_operation[10];
    char date_operation[20];
    int operation_amount;
} Operations;

typedef struct account_ {
    int id_client;
    int id_account;
    char password[32];
    int balance;
    Operations operations[10];
    int operation_count;
} Account;

Account account_db[] = {
    {1, 1, "p1", 5000, {{"", "", 0}}, 0},
    {2, 2, "p2", 3000, {{"", "", 0}}, 0},
    {3, 3, "p", 3999, {{"", "", 0}}, 0}
};

int account_db_size = sizeof(account_db) / sizeof(account_db[0]);

void save_operation(Account *account, const char *type_operation, int amount);
Account* find_account_by_ID(int id_client, int id_account, const char *password);
int AJOUT(int id_client, int id_account, const char *password, int amount);
int RETRAIT(int id_client, int id_account, const char *password, int amount);
int SOLDE(int id_client, int id_account, const char *password);
char* OPERATIONS(int id_client, int id_account, const char *password, char *buffer, size_t buffer_size);
void Die(char *mess);
void HandleClient(int sock);

void save_operation(Account *account, const char *type_operation, int amount) {
    printf("save operation 1\n");
    Operations *operation = &(account->operations[account->operation_count % 10]);
    printf("save operation 2\n");
    strncpy(operation->type_operation, type_operation, sizeof(operation->type_operation) - 1);
    printf("save operation 3\n");
    operation->type_operation[sizeof(operation->type_operation) - 1] = '\0';
    printf("save operation 4\n");
    time_t current_time = time(NULL);
    printf("save operation 5\n");
    strftime(operation->date_operation, sizeof(operation->date_operation), "%Y-%m-%d %H:%M:%S", localtime(&current_time));
    printf("save operation 6\n");
    operation->operation_amount = amount;
    printf("save operation 7\n");
    account->operation_count++;
    printf("save operation 8\n");
}

Account* find_account_by_ID(int id_client, int id_account, const char *password) {
    for (int i = 0; i < account_db_size; i++) {
        if (account_db[i].id_client == id_client && account_db[i].id_account == id_account &&
            strcmp(account_db[i].password, password) == 0) {
            return &account_db[i];
        }
    }
    return NULL;
}

int AJOUT(int id_client, int id_account, const char *password, int amount) {
    Account *account = find_account_by_ID(id_client, id_account, password);
    if (account) {
        account->balance += amount;
        printf("save operation %d \n",account->balance);
        save_operation(account, "AJOUT", amount);
        printf("save operation\n");
        return 1;
        printf("save operation\n");
    }
    return -1;
}

int RETRAIT(int id_client, int id_account, const char *password, int amount) {
    Account *account = find_account_by_ID(id_client, id_account, password);
    if (account && account->balance >= amount) {
        account->balance -= amount;
        save_operation(account, "RETRAIT", amount);
        return 1;
    }
    return -1;
}

int SOLDE(int id_client, int id_account, const char *password) {
    Account *account = find_account_by_ID(id_client, id_account, password);
    if (account) {
        return account->balance;
    }
    return -1;
}

char* OPERATIONS(int id_client, int id_account, const char *password, char *buffer, size_t buffer_size) {
    Account *account = find_account_by_ID(id_client, id_account, password);
    if (account) {
        int start;
        if (account->operation_count > 10) {
            start = account->operation_count - 10;
        } else {
            start = 0;
        }
        buffer[0] = '\0';
        for (int i = start; i < account->operation_count; i++) {
            Operations *operation = &(account->operations[i % 10]);
            char line[128];
            snprintf(line, sizeof(line), "%s %s %d\n", operation->type_operation, operation->date_operation, operation->operation_amount);
            strncat(buffer, line, buffer_size - strlen(buffer) - 1);
        }
        return buffer;
    }
    return NULL;
}

void Die(char *mess) {
    perror(mess);
    exit(1);
}

void HandleClient(int sock) {
    char buffer[BUFFSIZE];
    int received = -1;
    char *message_client = "\nServer status READY";
    
    if ((received = recv(sock, buffer, BUFFSIZE, 0)) < 0) {
        Die("Failed to receive initial bytes from client");
    }
    printf("Received message: %s\n", buffer);
    
    while (received > 0) {
        if (send(sock, buffer, received, 0) != received) {
            Die("Failed to send bytes to client");
        }
        buffer[received] = '\0';
        printf("Sent message back: %s\n", buffer);
        
        if ((received = recv(sock, buffer, BUFFSIZE, 0)) < 0) {
            Die("Failed to receive additional bytes from client");
        }
    }
    
    close(sock);
}

#endif // HEADER_H