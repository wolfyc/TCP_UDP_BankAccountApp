#include "header.h"

int main(int argc, char *argv[])
{
  int serversock, clientsock;
  struct sockaddr_in echoserver, echoclient;
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
    //HandleClient(clientsock);
  }
}
