#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#define port 8080
#define BUFFSIZE 1024
void Die(char *mess) { perror(mess); exit(1); }

int main(int argc, char *argv[]) {
  int sock;
  struct sockaddr_in echoserver;
  char buffer[BUFFSIZE];
  unsigned int echolen;
  int received = 0;

  char *AJOUT = "AJOUT  <id_client  id_compte  password  somme>";
  char *RETRAIT = "RETRAIT <id_client id_compte password somme>";
  char *SOLDE = "SOLDE <id_client id_compte password>";
  char *OPERATIONS = "OPERATIONS <id_client  id_compte  password>";
  char *hello = "Hello from client !";
/*
  if (argc != 4) {
    fprintf(stderr, "USAGE: TCPecho <server_ip> <word>\n");
    exit(1);
  }*/

  /* Create the TCP socket */
  if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    Die("Failed to create socket");
  }
  /* Construct the server sockaddr_in structure */
  //memset(&echoserver, 0, sizeof(echoserver));       /* Clear struct */
  echoserver.sin_family = AF_INET;      /* Internet/IP */
  echoserver.sin_addr.s_addr = INADDR_ANY;//inet_addr(argv[1]);  /* IP address */
  echoserver.sin_port = htons(port);       /* server port */
  /* Establish connection */
  if (connect(sock,(struct sockaddr *) &echoserver,sizeof(echoserver)) < 0) {
    Die("Failed to connect with server");
  }
  /*Human interface*/
  printf("Bonjour, voici les requêtes disponibles :\nAjouter de l'argent à votre compte : %s\nRetirer de l'argent : %s\nAfficher votre solde : %s\nAfficher les 10 dernieres operations : %s\n", AJOUT, RETRAIT, SOLDE, OPERATIONS);
  
  while (1) {
        printf("\nEnter your request, and please mind the syntaxe above \n");
        fgets(buffer, BUFFSIZE, stdin);
        send(sock, buffer, strlen(buffer), 0);
        printf("Request sent\n");
        
        echolen = read(sock, buffer, BUFFSIZE);
        buffer [echolen ] = '\0';
        printf("server response %s\n", buffer);
    }

  /* Send the word to the server 
  echolen = strlen(argv[3]);
  if (send(sock, argv[0], echolen, 0) != echolen) {
    Die("Mismatch in number of sent bytes");
  }
  /* Receive the word back from the server 
  //*
  fprintf(stdout, "Received: ");
  while (received < echolen) {
    int bytes = 0;
    if ((bytes = recv(sock, buffer, BUFFSIZE-1, 0)) < 1) {
      Die("Failed to receive bytes from server");
    }
    received += bytes;
    buffer[bytes] = '\0';        //*/
    /* Assure null terminated string 
    fprintf(stdout, buffer);
  }
  fprintf(stdout, "\n");*/
  close(sock);
  exit(0);
       }
