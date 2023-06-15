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
    int operation_amount;
} Operations;

typedef struct account_{
    int id_client;
    int id_account;
    char password[32];
    int balance;
    Operations operations[11];
    int Operation_nbr;
} Account;
Account account_db[] = {
    // {id_client, id_compte, password, solde, {{OPERATIONS}}, nombre_operations}
    {1, 1, "p1", 5000, {{"", "", 0}}, 0},
    {2, 2, "p2", 3000, {{"", "", 0}}, 0},
    {3, 3, "p", 3999, {{"", "", 0}}, 0}
};
int account_db_nbr = sizeof(account_db) / sizeof(account_db[0]);

/** Enregistre une opération dans le tableau des opérations d'un compte
  *
  * @param *account pointeur vers la structure du compte
  * @param *type_operation pointeur vers une chaîne de caractères décrivant l'opération
  * @param amount entier représentant le montant de la transaction
  * @return void
  */
void save_operation(Account *account, const char *type_operation, int amount);

/** Recherche un compte en faisant correspondre ses identifiants et son mot de passe
  *
  * @param id_client entier pour l'ID du client
  * @param id_account entier pour l'ID du compte
  * @param *password pointeur vers le mot de passe
  * @return un pointeur vers une structure Account
  */
Account* find_account_by_ID(int id_client, int id_account, const char *password);

/** Ajoute un montant au solde du compte
  *
  * @param id_client entier pour l'ID du client
  * @param id_account entier pour l'ID du compte
  * @param *password pointeur vers le mot de passe
  * @param amount entier représentant le montant à ajouter au solde
  * @return 1 en cas de succès, -1 en cas d'échec
  */
int AJOUT(int id_client, int id_account, const char *password, int amount);

/** Retire un montant spécifique du solde du compte
  *
  * @param id_client entier pour l'ID du client
  * @param id_account entier pour l'ID du compte
  * @param *password pointeur vers le mot de passe
  * @param amount entier représentant le montant à retirer du solde
  * @return 1 en cas de succès, -1 en cas d'échec
  */
int RETRAIT(int id_client, int id_account, const char *password, int amount);

/** Renvoie le solde du compte
  *
  * @param id_client entier pour l'ID du client
  * @param id_account entier pour l'ID du compte
  * @param *password pointeur vers le mot de passe
  * @return le solde en cas de succès, -1 en cas d'échec
  */
int SOLDE(int id_client, int id_account, const char *password);

/** Renvoie l'historique des opérations du compte (les 10 dernières opérations)
  *
  * @param id_client entier pour l'ID du client
  * @param id_account entier pour l'ID du compte
  * @param *password pointeur vers le mot de passe
  * @param buffer chaîne de caractères pour enregistrer les 10 opérations du compte
  * @param buffer_size entier pour la taille du buffer
  * @return le buffer contenant les 10 dernières opérations en cas de succès, NULL en cas d'échec
  */
char *OPERATIONS(int id_client, int id_account, const char *password, char *buffer, size_t buffer_size);

/** Gère la communication avec un client connecté au socket
  *
  * @param sock entier représentant le descripteur de socket
  * @return void
  */
void HandleClient(int sock);

void Die(char *mess);



void save_operation(Account* account, const char* type_operation, int amount) {
    Operations* operation = &(account->operations[account->Operation_nbr % 10]);
    strncpy(operation->type_operation, type_operation, sizeof(operation->type_operation) - 1); // avoid overflow
    operation->type_operation[sizeof(operation->type_operation) - 1] = '\0'; //specify the last caracter as NULL
    time_t current_time = time(NULL);
    strftime(operation->date_operation, sizeof(operation->date_operation), "%Y-%m-%d %H:%M:%S", localtime(&current_time));
    operation->operation_amount = amount;
    account->Operation_nbr++;
    }

Account* find_account_by_ID(int id_client, int id_account, const char* password) {
    for (int i = 0; i < account_db_nbr; i++) {
        if (account_db[i].id_client == id_client && account_db[i].id_account == id_account &&
    strcmp(account_db[i].password, password) == 0) {
        printf("I am inside  find_account_by_ID\n");
    return &account_db[i];
        }
    }
    return NULL;
}

int AJOUT(int id_client, int id_account, const char* password, int amount) {
    Account* account = find_account_by_ID(id_client, id_account, password);
    printf("password est %s\n", account->password);
    if (account) {
        printf("b4 balance is %d\n", account->balance);
        account->balance += amount;
        printf("balance is %d\n", account->balance);
        // Record the operation
        save_operation(account, "AJOUT", amount);
        return 1;
    }
    return -1; //account is NULL
}

int RETRAIT(int id_client, int id_account, const char* password, int amount) {
    Account* account = find_account_by_ID(id_client, id_account, password);
    if (account && account->balance >= amount) {
        account->balance -= amount;
        save_operation(account, "RETRAIT", amount); // Record the operation
        return 1;
    }
    
    return -1;
}

int SOLDE(int id_client, int id_account, const char* password) {
    Account* account = find_account_by_ID(id_client, id_account, password);
    if (account) {
        return account->balance;
    }
    
    return -1;
}

char* OPERATIONS(int id_client, int id_account, const char* password, char* buffer, size_t buffer_size) {
    Account* account = find_account_by_ID(id_client, id_account, password);
    if (account) {
        int start;
        if (account->Operation_nbr > 10) start= account->Operation_nbr - 10; else start = 0;
        buffer[0] = '\0';
        for (int i = start; i < account->Operation_nbr; i++) {
            Operations* operation = &(account->operations[i % 10]);
            char line[128];
            snprintf(line, sizeof(line), "%s %s %d\n", operation->type_operation, operation->date_operation, operation->operation_amount);
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
