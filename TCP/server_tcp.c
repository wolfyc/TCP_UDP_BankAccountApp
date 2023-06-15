#include "header.h"

int main(int argc, char *argv[])
{
  int serversock, clientsock;
  char buffer[BUFFSIZE];
  struct sockaddr_in echoserver, echoclient;
  Account test = account_db[0];
  /*

              /* Create the TCP socket */
  if ((serversock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
  {
    Die("Failed to create socket");
  }
  /* Construct the server sockaddr_in structure */
  memset(&echoserver, 0, sizeof(echoserver)); /* Clear struct */
  echoserver.sin_family = AF_INET;            /* Internet/IP */
  echoserver.sin_addr.s_addr = INADDR_ANY;    //  /* Incoming addr */
  echoserver.sin_port = htons(port);          /* server port */
  /* Bind the server socket */
  if (bind(serversock, (struct sockaddr *)&echoserver,
           sizeof(echoserver)) < 0)
  {
    Die("Failed to bind the server socket");
  }
  /* Listen on the server socket */
  if (listen(serversock, MAXPENDING) < 0)
  {
    Die("Failed to listen on server socket");
  }
  unsigned int clientlen = sizeof(echoclient);
  /* Run until cancelled */
  while (1)
  {
    
    /* Wait for client connection */
    if ((clientsock = accept(serversock, (struct sockaddr *)&echoclient, &clientlen)) < 0)
    {
      Die("Failed to accept client connection");
    }
    fprintf(stdout, "Client connected: %s\nWaiting for instructions from client\n", inet_ntoa(echoclient.sin_addr));
    int id_client, id_account;
    const char *password;
    int amount;
    char answer[BUFFSIZE];
    int do_while_flag = 1;
    do{
      /*reading client request*/
      int bytes_read = read(clientsock, buffer, BUFFSIZE);
            if (bytes_read <= 0) {
                do_while_flag = 0;
                break;
            }
            buffer[BUFFSIZE - 1] = '\0'; 
      
      char* token = strtok(buffer, " ");
      // if client sends "KILL", he gets disconnected
      if (strcmp(token, "KILL") == 0) {
                do_while_flag = 0;
                printf("client request KILLed\n");
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
            amount = atoi(token); //strtod(token, NULL);
            if (AJOUT(id_client, id_account, password, amount)) 
                strcpy(answer, "OK\n");
            else 
                strcpy(answer, "KO\n");

        }else if(strcmp(token, "RETRAIT") == 0){
            token = strtok(NULL, " ");
            id_client = atoi(token);
            token = strtok(NULL, " ");
            id_account = atoi(token);
            token = strtok(NULL, " ");
            password = token;
            token = strtok(NULL, " ");
            amount = strtod(token, NULL);
            if (RETRAIT(id_client,id_account,password,amount))
              strcpy(answer, "OK\n");
            else
              strcpy(answer, "KO\n");

        }else if(strcmp(token, "SOLDE") == 0) {
            token = strtok(NULL, " ");
            id_client = atoi(token);
          
            token = strtok(NULL, " ");
            id_account = atoi(token);
          
            token = strtok(NULL, " ");
            password = token;
            
            
            int account_balance = SOLDE(id_client,id_account,password);
            
            if (account_balance >= 0){
              snprintf(answer, BUFFSIZE, "your account balance is: %d Euro\n", account_balance);
            }else
              strcpy(answer, "KO\n");

        }else if (strcmp(token, "OPERATIONS") == 0) {
            token = strtok(NULL, " ");
            id_client = atoi(token);
            token = strtok(NULL, " ");
            id_account = atoi(token);
            token = strtok(NULL, " ");
            password = token;
            char op_buffer[BUFFSIZE];
            char *resultArray =  OPERATIONS(id_client,id_account,password,op_buffer,BUFFSIZE);
            if (resultArray ){
              snprintf(answer, BUFFSIZE, "Liste of past 10 operations:%s\n", resultArray);
            }
            else
              strcpy(answer, "KO\n");
            }else
              strcpy(answer, "KO\n");
      }
            // sending back feedback
            send(clientsock, answer, strlen(answer), 0);
    }while(do_while_flag);
    close(clientsock);
  }
  return 1;
}
  