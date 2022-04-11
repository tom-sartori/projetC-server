#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]) {

    printf("Début programme\n");

    if (argc != 2) {
        printf("Nombre d'arguments invalide. Utilisation :\n");;
        printf("%s PORT\n", argv[0]);
        exit(0);
    }

    int dS = socket(PF_INET, SOCK_STREAM, 0);
    if (dS == -1) {
        printf("Erreur lors de la création du socket. \n");
        exit(0);
    } else { printf("Socket Créé\n"); }



    struct sockaddr_in ad;
    ad.sin_family = AF_INET;
    ad.sin_addr.s_addr = INADDR_ANY ;
    ad.sin_port = htons(atoi(argv[1])) ;

    if (bind(dS, (struct sockaddr*)&ad, sizeof(ad)) == -1) {
        printf("Erreur lors du bind. \n");
        printf("%s", strerror(errno));
        exit(0);
    } else { printf("Socket Nommé\n"); }

    if (listen(dS, 7) == -1) {
        printf("Erreur lors du listen. \n");
        printf("%s", strerror(errno));
        exit(0);
    } else { printf("Mode écoute\n"); }

    struct sockaddr_in aC ;
    socklen_t lg = sizeof(struct sockaddr_in) ;
    int dSC = accept(dS, (struct sockaddr*) &aC,&lg) ;

    if (dSC == -1) {
        printf("Erreur lors de la connection. \n");
        printf("%s", strerror(errno));
        exit(0);
    } else { printf("Client Connecté\n"); }

    int size;
    if(recv(dSC, &size, sizeof(int), 0)==-1){
        printf("Erreur lors de la reception du message. \n");
        exit(0);
    } else { printf("Message reçu : %d\n", size); }

    int test = ntohl(size);

    char* msg = (char*)malloc(sizeof(char)*(test+1));

    if(recv(dSC, msg, sizeof(char)*test, 0)==-1){
        printf("Erreur lors de la reception du message. \n");
        exit(0);
    } else { printf("Message reçu : %s\n", msg); }

    int r = 10 ;

    if (send(dSC, &r, sizeof(int), 0) == -1) {
        printf("Erreur lors de l'envoi du message. \n");
        printf("%s", strerror(errno));
        exit(0);
    } else { printf("Message Envoyé\n"); }

    shutdown(dSC, 2);
    shutdown(dS, 2);
    printf("Fin du programme. \n");
}