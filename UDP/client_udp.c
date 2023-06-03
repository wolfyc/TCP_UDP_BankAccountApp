#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define port 8081
#define BUFFSIZE 1024

void Die(char *mess) { perror(mess); exit(1); }

int main(int argc, char *argv[]) {
  int sock;
  struct sockaddr_in echoserver;
  char buffer[BUFFSIZE];
  unsigned int echolen;
  int received = 0;
  // Human interface information
  char *AJOUT = "AJOUT  <id_client  id_account  password  amount>";
  char *RETRAIT = "RETRAIT <id_client id_account password amount>";
  char *SOLDE = "SOLDE <id_client id_account password+space >";
  char *OPERATIONS = "OPERATIONS <id_client  id_account  password+space>";
  char *hello = "Hello from client !";

  /* Create the UDP socket */
  if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
    Die("Failed to create socket");
  }
  /* Construct the server sockaddr_in structure */
  memset(&echoserver, 0, sizeof(echoserver));       /* Clear struct */
  echoserver.sin_family = AF_INET;      /* Internet/IP */
  echoserver.sin_addr.s_addr = INADDR_ANY;//inet_addr(argv[1]);  /* IP address */
  echoserver.sin_port = htons(port);       /* server port */
  
  while (1) {
    printf("Bonjour, voici les requêtes disponibles :\nAjouter de l'argent à votre compte : %s\nRetirer de l'argent : %s\nAfficher votre solde : %s\nAfficher les 10 dernières opérations : %s\n", AJOUT, RETRAIT, SOLDE, OPERATIONS);
    printf("\nEnter your request, and please mind the syntax above:\n");

    fgets(buffer, BUFFSIZE, stdin);
    buffer[strcspn(buffer, "\n")] = '\0'; // Remove trailing newline character

    /* Send the request to the server */
    sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&echoserver, sizeof(echoserver));
    printf("Request sent\n");

    /* Receive and print the server response */
    echolen = recvfrom(sock, buffer, BUFFSIZE, 0, NULL, NULL);
    buffer[echolen] = '\0';
    printf("Server response: %s\n", buffer);
  }

  close(sock);
  exit(0);
}