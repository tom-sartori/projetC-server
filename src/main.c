#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#define MAX_CLIENT_NUMBER 1000

int socketDescriptor = -1;
int clientList[MAX_CLIENT_NUMBER];
int currentClientCount = 0;


/**
 * Perror the message in params and exit the programme.
 * If hasErrno, print it.
 * @param errorMessage
 * @param hasErrno
 */
void throwError (char *errorMessage, int hasErrno) {
    perror(errorMessage);
    if (hasErrno) {
        printf("%s", strerror(errno));
    }
    exit(EXIT_FAILURE);
}

/**
 * Create a socket and return his descriptor.
 *
 * @return the descriptor of the socket created.
 */
int socketCreation () {
    int socketDescriptor = socket(PF_INET, SOCK_STREAM, 0);
    if (socketDescriptor == -1) {
        throwError("Erreur lors de la création du socket. \n", 0);
    }
    else {
        printf("Socket Créé. \n");
    }
    return socketDescriptor;
}

/**
 * Create, bind and make listening a socket.
 *
 * @param port
 * @return the descriptor of the socket created.
 */
int launchServer(int port) {
    int socketDescriptor = socketCreation();

    struct sockaddr_in ad;
    ad.sin_family = AF_INET;
    ad.sin_addr.s_addr = INADDR_ANY;
    ad.sin_port = htons(port);

    if (bind(socketDescriptor, (struct sockaddr*)&ad, sizeof(ad)) == -1) {
        throwError("Erreur lors du bind. \n", 1);
    }
    else {
        printf("Socket Nommé\n");
    }

    if (listen(socketDescriptor, 7) == -1) {
        throwError("Erreur lors du listen. \n",  1);
    }
    else {
        printf("Mode écoute\n");
    }
    return socketDescriptor;
}

/**
 * For a socket descriptor in params, connect it to the client.
 * Use accept function.
 *
 * @param socketDescriptor
 * @return an accepted socket descriptor.
 */
int connectToClient (int socketDescriptor) {
    struct sockaddr_in aC;
    socklen_t lg = sizeof(struct sockaddr_in);

    // Waiting for a clients connection
    int acceptedSocketDescriptor = accept(socketDescriptor, (struct sockaddr*) &aC, &lg);

    // Checking for errors
    if (acceptedSocketDescriptor == -1) {
        throwError("Erreur lors de la connection. \n", 1);
    }
    else {
        printf("Client Connecté\n");
    }
    return acceptedSocketDescriptor;
}

/**
 * For an accepted socket descriptor in params, wait until receiving an int message.
 * This message is usually the size of the message which will be sent just after.
 *
 * @param acceptedSocketDescriptor
 * @return the size of the message which will be sent after.
 */
int receiveMessageInt (int acceptedSocketDescriptor) {
    int size;
    if(recv(acceptedSocketDescriptor, &size, sizeof(int), 0) == -1){
        throwError("Erreur lors de la reception du message. \n", 0);
    }
    else {
        printf("Message reçu : %d\n", size);
    }
    return size;
}

/**
 * For an accepted socket descriptor in params, wait until receiving a message of the size in params.
 *
 * @param acceptedSocketDescriptor
 * @param size
 * @return the gotten message.
 */
char *receiveMessageString (int acceptedSocketDescriptor, int size) {
    //int test = ntohl(size);
    char *msg = (char*)malloc(sizeof(char)*(size+1));

    if(recv(acceptedSocketDescriptor, msg, sizeof(char)*size, 0)==-1){
        throwError("Erreur lors de la reception du message. \n", 0);
    }
    else {
        printf("Message reçu : %s\n", msg);
    }
    return msg;
}

/**
 *
 * @param acceptedSocketDescriptor
 * @return
 */
char *receiveMessage (int acceptedSocketDescriptor) {
    // Receive messageSize.
    int messageSize = receiveMessageInt(acceptedSocketDescriptor);
    // Receive message.
    return receiveMessageString(acceptedSocketDescriptor, messageSize);
}

/**
 * For an accepted socket descriptor in params, send a message of type int.
 * This message is usually used to send the message's size which will be sent after.
 *
 * @param acceptedSocketDescriptor
 * @param messageSize
 */
void sendMessageInt (int acceptedSocketDescriptor, int messageSize) {
    // Sending the message's size to the client.
    if(send(acceptedSocketDescriptor, &messageSize, sizeof(int), 0) == -1){
        throwError("Erreur lors de l'envoi du message. \n", 1);
    }
    else {
        printf("Taille du message envoyée. \n");
    }
}

/**
 * For an accepted socket descriptor and a message size, send the message in params.
 *
 * @param acceptedSocketDescriptor
 * @param message
 * @param size
 */
void sendMessageString (int acceptedSocketDescriptor, char *message, int size) {
    // Sending the message to the client.
    if(send(acceptedSocketDescriptor, message, sizeof(char) * size, 0) == -1){
        throwError("Erreur lors de l'envoi du message. \n", 1);
    }
    else {
        printf("Message envoyé. \n");
    }
}

/**
 * Firstly, send the messageSize through the accepted socket descriptor.
 * Secondly, send the message.
 *
 * @param acceptedSocketDescriptor
 * @param message
 * @param messageSize
 */
void sendMessage (int acceptedSocketDescriptor, char *message) {
    int messageSize = (int)strlen(message);
    printf("Size of sent message: %d \n", messageSize);
    // Send message messageSize.
    sendMessageInt(acceptedSocketDescriptor, messageSize);
    // Send message.
    sendMessageString(acceptedSocketDescriptor, message, messageSize);
}

void closeServer(){
    /**
    * Shutting down the server.
    */

    // If the socket is set, we run shutdown on it
    if(socketDescriptor != -1){
        shutdown(socketDescriptor, 2);
    }
    // End of program with success
    printf("Fin du programme. \n");
    exit(EXIT_SUCCESS);
}

void readingLoop(int acceptedSocketDescriptor){
    while(1){
        /**
        * Now, we wait for the client 2 to send a message.
        */
        char* message = receiveMessage(acceptedSocketDescriptor);
        printf("%s \n", message);
    }
}

/**
 * Server side.
 */
int main(int argc, char *argv[]) {

    //Assigning function closeServer() to SIGTERM signal
    signal(SIGTERM, closeServer);

/**
 * Server Start.
 */
    printf("Début programme. \n");


/**
 * Check arguments.
 */
    if (argc != 2) {
        printf("Nombre d'arguments invalide. Utilisation :\n");
        printf("%s PORT\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Error if port < 1024 (argv 2)
    if(atoi(argv[1]) < 1024){
        throwError("Erreur: le port doit être supérieur à 1024. \n", 0);
    }


/**
 * Launch server.
 */
    socketDescriptor = launchServer(atoi(argv[1]));

    // Server is launched

/**
 * Starting server loop
 */
    int newClient;
    // create pthread list
    pthread_t pthread[MAX_CLIENT_NUMBER];

    // if new client
        //recup socket -> ajoute a liste a l'indice current
        // crée le thread a l'indice current
            // Fonction de lecture a l'infini qui affiche les messages qu'elle reçoit
            /// 2 Si message reçu fin,
                // shutdown client (voir si c'est suffisant pour shutdown le client où s'il faut le faire à la main)
                // on kill le thread
                // Amélioration implémenter une liste chainée
            /// 3 Broadcast
            /// 4 commandes -> MP, etc
        // current += 1




    while(1){
    /**
     * Connect clients.
     */
        // Waiting for a client connection.
        newClient = connectToClient(socketDescriptor);

        // adding client socket to clientList
        clientList[currentClientCount] = newClient;


        // launch client thread

        int testThread = pthread_create(&pthread[currentClientCount],NULL,readingLoop,newClient);

        if (testThread) {
            throwError("Error:unable to create thread, %d\n", 0);
        }


        // upping currentClientCount
        currentClientCount+=1;

    }
    /**
 * shutting down clients stream
 * shutdown(newClient, 2);
 */
}