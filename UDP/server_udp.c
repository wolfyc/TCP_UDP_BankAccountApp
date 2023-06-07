#include "header.h"

int main(int argc, char *argv[])
{
  int serversock;
  char buffer[BUFFSIZE];
  struct sockaddr_in echoserver, echoclient;
  Account test = account_db[0];

  /* Create the UDP socket */
  if ((serversock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
  {
    Die("Failed to create socket");
  }

  /* Construct the server sockaddr_in structure */
  memset(&echoserver, 0, sizeof(echoserver)); /* Clear struct */
  echoserver.sin_family = AF_INET;            /* Internet/IP */
  echoserver.sin_addr.s_addr = INADDR_ANY;    /* Incoming addr */
  echoserver.sin_port = htons(PORT);          /* Server port */

  /* Bind the server socket */
  if (bind(serversock, (struct sockaddr *)&echoserver, sizeof(echoserver)) < 0)
  {
    Die("Failed to bind the server socket");
  }

  unsigned int clientlen = sizeof(echoclient);

  /* Run until cancelled */
  while (1)
  {
    /* Receive client request */
    int bytes_received = recvfrom(serversock, buffer, BUFFSIZE, 0, (struct sockaddr *)&echoclient, &clientlen);
    if (bytes_received < 0)
    {
      Die("Failed to receive client request");
    }

    fprintf(stdout, "Client connected: %s\nWaiting for instructions from client\n", inet_ntoa(echoclient.sin_addr));

    int id_client, id_account;
    const char *password;
    int amount;
    char answer[BUFFSIZE];
    int do_while_flag = 1;

    do
    {
      buffer[bytes_received] = '\0';

      char *token = strtok(buffer, " ");

      if (token != NULL)
      {
        if (strcmp(token, "KILL") == 0)
        {
          do_while_flag = 0;
          printf("client request KILLed\n");
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
            strcpy(answer, "OoooK\n");
          else
            strcpy(answer, "KO\n");
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
            strcpy(answer, "OK\n");
          else
            strcpy(answer, "KO\n");
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
            snprintf(answer, BUFFSIZE, "your account balance is: %d Euro\n", account_balance);
          }
          else
            strcpy(answer, "KO\n");
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
            snprintf(answer, BUFFSIZE, "List of past 10 operations:\n%s\n", resultArray);
          }
          else
            strcpy(answer, "KO\n");
        }
        else
          strcpy(answer, "KO\n");
      }
      printf("sent messageé");
      /* Send back the response */
      //*
      int bytes_sent = sendto(serversock, answer, strlen(answer), 0, (struct sockaddr *)&echoclient, clientlen);
      
      if (bytes_sent < 0)
      {
        Die("Failed to send response to client");
      }
      //*/
    } while (do_while_flag);
  }

  return 0;
}