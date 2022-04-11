#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]) {

/*--------------------------------------------------------------------

                        SETTING UP THE SERVER

--------------------------------------------------------------------*/

    printf("Début programme\n");

    if (argc != 2) {
        printf("Nombre d'arguments invalide. Utilisation :\n");;
        printf("%s PORT\n", argv[0]);
        return EXIT_FAILURE;
    }

    int dS = socket(PF_INET, SOCK_STREAM, 0);
    if (dS == -1) {
        printf("Erreur lors de la création du socket. \n");
        return EXIT_FAILURE;
    } else { printf("Socket Créé\n"); }


    struct sockaddr_in ad;
    ad.sin_family = AF_INET;
    ad.sin_addr.s_addr = INADDR_ANY ;
    ad.sin_port = htons(atoi(argv[1])) ;

    if (bind(dS, (struct sockaddr*)&ad, sizeof(ad)) == -1) {
        printf("Erreur lors du bind. \n");
        printf("%s", strerror(errno));
        return EXIT_FAILURE;
    } else { printf("Socket Nommé\n"); }

    if (listen(dS, 7) == -1) {
        printf("Erreur lors du listen. \n");
        printf("%s", strerror(errno));
        return EXIT_FAILURE;
    } else { printf("Mode écoute\n"); }

    // Server is launched

/*---------------------------------------------
 *
 *              Handling clients
 *
 * --------------------------------------------
 */

    struct sockaddr_in aC;
    socklen_t lg = sizeof(struct sockaddr_in);

    // Waiting for a clients connection
    int dSC1 = accept(dS, (struct sockaddr*) &aC,&lg);
    // Waiting for the second client to connect
    int dSC2 = accept(dS, (struct sockaddr*) &aC,&lg);


    // Checking for errors
    if (dSC1 == -1 || dSC2 == -1) {
        printf("Erreur lors de la connection. \n");
        printf("%s", strerror(errno));
        return EXIT_FAILURE;
    } else { printf("Client Connecté\n"); }


    // First client will wait for a message

    char* clientStatus = "wait\0";

    int sendReturn = send(dSC1, clientStatus, sizeof(char)*5, 0);
    // verif erreur
    if(sendReturn == -1){
        printf("Erreur lors de l'envoi du message. \n");
        printf("%s", strerror(errno));
        return EXIT_FAILURE;
    } else { printf("Message Envoyé \n"); }

    // Second client is invited to send a message

    clientStatus = "send\0";

    int sendReturn2 = send(dSC2, clientStatus, sizeof(char)*5, 0);
    // verif erreur
    if(sendReturn2 == -1){
        printf("Erreur lors de l'envoi du message. \n");
        printf("%s", strerror(errno));
        return EXIT_FAILURE;
    } else { printf("Message Envoyé \n"); }

    // Now we wait for the client n°2 to send a message

    int size;
    if(recv(dSC2, &size, sizeof(int), 0)==-1){
        printf("Erreur lors de la reception du message. \n");
        return EXIT_FAILURE;
    } else { printf("Message reçu : %d\n", size); }

    int test = ntohl(size);

    char* msg = (char*)malloc(sizeof(char)*(test+1));

    if(recv(dSC2, msg, sizeof(char)*test, 0)==-1){
        printf("Erreur lors de la reception du message. \n");
        return EXIT_FAILURE;
    } else { printf("Message reçu : %s\n", msg); }

    // Sending the message to first client

    int sendSizeReturn = send(dSC1, &size, sizeof(int), 0);
    // verif erreur
    if(sendSizeReturn == -1){
        printf("Erreur lors de l'envoi du message. \n");
        printf("%s", strerror(errno));
        return EXIT_FAILURE;
    } else { printf("Taille du message envoyé \n"); }


    int sendReturn3 = send(dSC1, msg, sizeof(char)*(test+1), 0);
    // verif erreur
    if(sendReturn3 == -1){
        printf("Erreur lors de l'envoi du message. \n");
        printf("%s", strerror(errno));
        return EXIT_FAILURE;
    } else { printf("Message Envoyé \n"); }

    /*------------------------------------------------------------------------
     *
     *                      Shutting down the server
     *
     * ------------------------------------------------------------------------
     */

    shutdown(dSC1, 2);
    shutdown(dSC2, 2);
    shutdown(dS, 2);
    printf("Fin du programme. \n");
}