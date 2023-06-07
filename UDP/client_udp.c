#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT 8081
#define BUFFSIZE 1024

void Die(char *mess) {
  perror(mess);
  exit(1);
}

int main(int argc, char *argv[]) {
  int sock;
  struct sockaddr_in echoserver;
  char buffer[BUFFSIZE];
  unsigned int echolen;
  int received = 0;

  /* Create the UDP socket */
  if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
    Die("Failed to create socket");
  }

  /* Construct the server sockaddr_in structure */
  memset(&echoserver, 0, sizeof(echoserver));
  echoserver.sin_family = AF_INET;
  echoserver.sin_addr.s_addr = INADDR_ANY; // You can specify the server IP address here
  echoserver.sin_port = htons(PORT);

  printf("Bonjour, voici les requêtes disponibles :\nAjouter de l'argent à votre compte : AJOUT <id_client> <id_compte> <password> <somme>\nRetirer de l'argent : RETRAIT <id_client> <id_compte> <password> <somme>\nAfficher votre solde : SOLDE <id_client> <id_compte> <password>\nAfficher les 10 dernières opérations : OPERATIONS <id_client> <id_compte> <password>\n");

  while (1) {
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