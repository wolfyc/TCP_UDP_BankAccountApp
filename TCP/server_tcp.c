#include "header.h"

int main()
{
  int serversock, clientsock;
  char buffer[BUFFSIZE];
  struct sockaddr_in echoserver, echoclient;
  

  /* Créer la socket TCP */
  if ((serversock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
  {
    Die("Échec de création de la socket");
  }

  /* Construire la structure sockaddr_in du serveur */
  memset(&echoserver, 0, sizeof(echoserver)); /* Effacer la structure */
  echoserver.sin_family = AF_INET;            /* Internet/IP */
  echoserver.sin_addr.s_addr = INADDR_ANY;    /* Adresse entrante */
  echoserver.sin_port = htons(port);          /* Port du serveur */

  /* Lier la socket du serveur */
  if (bind(serversock, (struct sockaddr *)&echoserver,
           sizeof(echoserver)) < 0)
  {
    Die("Échec de liaison de la socket du serveur");
  }

  /* Écouter sur la socket du serveur */
  if (listen(serversock, MAXPENDING) < 0)
  {
    Die("Échec d'écoute sur la socket du serveur");
  }

  unsigned int clientlen = sizeof(echoclient);

  /* Exécuter jusqu'à annulation */
  while (1)
  {
    
    /* Attendre la connexion du client */
    if ((clientsock = accept(serversock, (struct sockaddr *)&echoclient, &clientlen)) < 0)
    {
      Die("Échec d'acceptation de la connexion du client");
    }

    fprintf(stdout, "Client connecté : %s\nEn attente d'instructions du client\n", inet_ntoa(echoclient.sin_addr));

    int id_client, id_account;
    const char *password;
    int amount;
    char answer[BUFFSIZE];
    int do_while_flag = 1;

    do {
      /* Lecture de la requête du client */
      int bytes_read = read(clientsock, buffer, BUFFSIZE);
      if (bytes_read <= 0) {
        do_while_flag = 0;
        break;
      }
      buffer[BUFFSIZE - 1] = '\0'; 

      char* token = strtok(buffer, " ");

      if (strcmp(token, "KILL") == 0) {
        do_while_flag = 0;
        printf("Requête KILLed\n");
      }

      if (token != NULL) {

        if (strcmp(token, "AJOUT") == 0) {
          token = strtok(NULL, " ");
          id_client = atoi(token);
          token = strtok(NULL, " ");
          id_account = atoi(token);
          token = strtok(NULL, " ");
          password = token;
          token = strtok(NULL, " ");
          amount = atoi(token);
          
          if (AJOUT(id_client, id_account, password, amount)) 
            strcpy(answer, "AJOUT OK\n");
          else 
            strcpy(answer, "KO\n");

          printf("Opération AJOUT\n");

        } else if(strcmp(token, "RETRAIT") == 0) {
          token = strtok(NULL, " ");
          id_client = atoi(token);
          token = strtok(NULL, " ");
          id_account = atoi(token);
          token = strtok(NULL, " ");
          password = token;
          token = strtok(NULL, " ");
          amount = strtod(token, NULL);
          
          if (RETRAIT(id_client, id_account, password, amount))
            strcpy(answer, "RETRAIT OK\n");
          else
            strcpy(answer, "KO\n");

          printf("Opération RETRAIT\n");

        } else if(strcmp(token, "SOLDE") == 0) {
          token = strtok(NULL, " ");
          id_client = atoi(token);
          token = strtok(NULL, " ");
          id_account = atoi(token);
          token = strtok(NULL, " ");
          password = token;           
          int account_balance = SOLDE(id_client, id_account, password);

          if (account_balance >= 0) {
            snprintf(answer, BUFFSIZE, "Votre solde est de : %d Euro\n", account_balance);
          } else
            strcpy(answer, "KO\n");

          printf("Opération SOLDE\n");

        } else if (strcmp(token, "OPERATIONS") == 0) {
          token = strtok(NULL, " ");
          id_client = atoi(token);
          token = strtok(NULL, " ");
          id_account = atoi(token);
          token = strtok(NULL, " ");
          password = token;
          char op_buffer[BUFFSIZE];
          char *resultArray = OPERATIONS(id_client, id_account, password, op_buffer, BUFFSIZE);

          if (resultArray) {
            snprintf(answer, BUFFSIZE, "Liste des 10 dernières opérations : %s\n", resultArray);
          } else
            strcpy(answer, "KO\n");

          printf("Opération OPERATIONS\n");
        } else
          {
            strcpy(answer, "Opération/demande inconnue\n");
            printf("Opération/demande inconnue\n");
          }
      }

      // Envoi de la réponse au client
      send(clientsock, answer, strlen(answer), 0);
      printf("En attente d'une nouvelle demande si disponible\n");
    } while(do_while_flag);

    close(clientsock);
  }

  return 1;
}
