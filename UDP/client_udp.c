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

int main() {
  int sock;
  struct sockaddr_in echoserver;
  char buffer[BUFFSIZE];
  unsigned int echolen;

  /* Créer la socket UDP */
  if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
    Die("Échec de création de la socket");
  }

  /* Construire la structure sockaddr_in du serveur */
  memset(&echoserver, 0, sizeof(echoserver));
  echoserver.sin_family = AF_INET;
  echoserver.sin_addr.s_addr = INADDR_ANY; // Vous pouvez spécifier l'adresse IP du serveur ici
  echoserver.sin_port = htons(PORT);

  printf("Bonjour, voici les requêtes disponibles :\n\nAjouter de l'argent à votre compte : AJOUT <id_client> <id_compte> <password> <somme>\nRetirer de l'argent : RETRAIT <id_client> <id_compte> <password> <somme>\nAfficher votre solde : SOLDE <id_client> <id_compte> <password>\nAfficher les 10 dernières opérations : OPERATIONS <id_client> <id_compte> <password>\n");

  while (1) {

    printf("\nVeuillez entrer votre requête en respectant la syntaxe ci-dessus :\n");
    fgets(buffer, BUFFSIZE, stdin);
    buffer[strcspn(buffer, "\n")] = '\0'; // Supprimer le caractère de nouvelle ligne en fin de chaîne

    /* Envoyer la requête au serveur */
    sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&echoserver, sizeof(echoserver));
    printf("Requête envoyée\n");

    /* Recevoir et afficher la réponse du serveur */
    echolen = recvfrom(sock, buffer, BUFFSIZE, 0, NULL, NULL);
    buffer[echolen] = '\0';
    printf("Réponse du serveur : %s\n", buffer);
  }

  close(sock);
  exit(0);
}
