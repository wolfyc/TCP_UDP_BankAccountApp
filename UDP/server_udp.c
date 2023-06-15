#include "header.h"

int main()
{
  int serversock;
  char buffer[BUFFSIZE];
  struct sockaddr_in echoserver, echoclient;


  /* Crée le socket UDP */
  if ((serversock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
  {
    Die("Échec de la création du socket");
  }

  /* Construit la structure sockaddr_in du serveur */
  memset(&echoserver, 0, sizeof(echoserver)); /* Efface la structure */
  echoserver.sin_family = AF_INET;            /* Internet/IP */
  echoserver.sin_addr.s_addr = INADDR_ANY;    /* Adresse entrante */
  echoserver.sin_port = htons(PORT);          /* Port du serveur */

  /* Lie le socket du serveur */
  if (bind(serversock, (struct sockaddr *)&echoserver, sizeof(echoserver)) < 0)
  {
    Die("Échec de la liaison du socket du serveur");
  }

  unsigned int clientlen = sizeof(echoclient);

  /* S'exécute jusqu'à annulation */
  while (1)
  {
    fprintf(stdout, "En attente d'instructions du client\n");

    int id_client, id_account;
    const char *password;
    int amount;
    char answer[BUFFSIZE];
    int do_while_flag = 1;

    do
    {
      int bytes_received = recvfrom(serversock, buffer, BUFFSIZE, 0, (struct sockaddr *)&echoclient, &clientlen);
      if (bytes_received < 0)
      {
        Die("Échec de la réception de la demande du client");
        do_while_flag = 0;
      }

      buffer[bytes_received] = '\0';


      char *token = strtok(buffer, " ");
      if (token != NULL)
      {
        if (strcmp(token, "KILL") == 0)
        {
          do_while_flag = 0;
          printf("Demande KILLée\n");
        }
        else if (strcmp(token, "AJOUT") == 0)
        {
          token = strtok(NULL, " ");
          id_client = atoi(token);
          token = strtok(NULL, " ");
          id_account = atoi(token);
          token = strtok(NULL, " ");
          password = token;
          token = strtok(NULL, " ");
          amount = atoi(token);

          if (AJOUT(id_client, id_account, password, amount) == 1)
            strcpy(answer, "AJOUT OK\n");

          else
            strcpy(answer, "KO AJOUT\n");
          printf("Opération AJOUT\n");
        }
        else if (strcmp(token, "RETRAIT") == 0)
        {
          token = strtok(NULL, " ");
          id_client = atoi(token);
          token = strtok(NULL, " ");
          id_account = atoi(token);
          token = strtok(NULL, " ");
          password = token;
          token = strtok(NULL, " ");
          amount = atoi(token);

          if (RETRAIT(id_client, id_account, password, amount) == 1)
            strcpy(answer, "RETRAIT OK\n");

          else
            strcpy(answer, "KO RETRAIT\n");
          printf("Opération RETRAIT\n");
        }
        else if (strcmp(token, "SOLDE") == 0)
        {
          token = strtok(NULL, " ");
          id_client = atoi(token);

          token = strtok(NULL, " ");
          id_account = atoi(token);

          token = strtok(NULL, " ");
          password = token;

          int account_balance = SOLDE(id_client, id_account, password);

          if (account_balance >= 0)
          {
            snprintf(answer, BUFFSIZE, "Votre solde est de : %d euros\n", account_balance);
          }
          else
            strcpy(answer, "KO SOLDE\n");
          printf("Opération SOLDE\n");
        }
        else if (strcmp(token, "OPERATIONS") == 0)
        {
          token = strtok(NULL, " ");
          id_client = atoi(token);
          token = strtok(NULL, " ");
          id_account = atoi(token);
          token = strtok(NULL, " ");
          password = token;

          char op_buffer[BUFFSIZE];
          char *resultArray = OPERATIONS(id_client, id_account, password, op_buffer, BUFFSIZE);

          if (resultArray)
          {
            snprintf(answer, BUFFSIZE, "Liste des 10 dernières opérations :\n%s\n", resultArray);
          }
          else
            strcpy(answer, "KO OPERATIONS\n");
          printf("Opération OPERATIONS\n");
        }
        else
          strcpy(answer, "Opération/demande inconnue\n");
      }

      int bytes_sent = sendto(serversock, answer, strlen(answer), 0, (struct sockaddr *)&echoclient, clientlen);

      if (bytes_sent < 0)
      {
        Die("Échec de l'envoi de la réponse au client");
      }
      printf("En attente d'une nouvelle demande si disponible\n");
    } while (do_while_flag);
  }

  return 0;
}
