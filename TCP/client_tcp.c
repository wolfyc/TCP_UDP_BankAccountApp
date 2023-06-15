#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT 8084
#define BUFFSIZE 1024

void Die(char *mess) { perror(mess); exit(1); }

int main() {
  int sock;
  struct sockaddr_in echoserver;
  char buffer[BUFFSIZE];
  unsigned int echolen;
  
  // Informations d'interface utilisateur
  char *AJOUT = "AJOUT  <id_client  id_account  password  montant>";
  char *RETRAIT = "RETRAIT <id_client id_account password montant>";
  char *SOLDE = "SOLDE <id_client id_account password+espace>";
  char *OPERATIONS = "OPERATIONS <id_client  id_account  password+espace>";
  

  /* Créer la socket TCP */
  if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    Die("Échec de création de la socket");
  }

  /* Construire la structure sockaddr_in du serveur */
  memset(&echoserver, 0, sizeof(echoserver));       /* Effacer la structure */
  echoserver.sin_family = AF_INET;      /* Internet/IP */
  echoserver.sin_addr.s_addr = INADDR_ANY;  /* Adresse IP */
  echoserver.sin_port = htons(PORT);       /* Port du serveur */

  /* Établir la connexion */
  if (connect(sock,(struct sockaddr *) &echoserver,sizeof(echoserver)) < 0) {
    Die("Échec de connexion avec le serveur");
  }

  /* Interface utilisateur */
  printf("Bonjour, voici les requêtes disponibles :\nAjouter de l'argent à votre compte : %s\nRetirer de l'argent : %s\nAfficher votre solde : %s\nAfficher les 10 dernières opérations : %s\n", AJOUT, RETRAIT, SOLDE, OPERATIONS);

  while (1) {
        printf("\nEntrez votre requête en respectant la syntaxe ci-dessus :\n");
        fgets(buffer, BUFFSIZE, stdin);
        send(sock, buffer, strlen(buffer), 0);
        printf("Requête envoyée\n");
        
        echolen = read(sock, buffer, BUFFSIZE);
        buffer[echolen] = '\0';
        printf("Réponse du serveur : %s\n", buffer);
    }

  close(sock);
  exit(0);
}