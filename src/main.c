#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include "Client.c"
#include "List.c"
#include "color.c"
#define ENDING_MESSAGE "Fin\n\0"

int serverSocketDescriptor;
List *clientList;


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
 * @param serverSocketDescriptor
 * @return an accepted socket descriptor.
 */
int connectToClient () {
    struct sockaddr_in aC;
    socklen_t lg = sizeof(struct sockaddr_in);

    // Waiting for a clients connection
    int acceptedSocketDescriptor = accept(serverSocketDescriptor, (struct sockaddr*) &aC, &lg);

    // Checking for errors
    if (acceptedSocketDescriptor == -1) {
        throwError("Erreur lors de la connection. \n", 1);
    }
    else {
        printf("Client Connecté. \n");
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
//    else {
//        printf("Message de taille %d recu. \n", size);
//    }
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
    char *message = (char*)malloc(sizeof(char) * (size + 1));

    if(recv(acceptedSocketDescriptor, message, sizeof(char) * size, 0) == -1){
        throwError("Erreur lors de la reception du message. \n", 0);
    }
//    else {
//        printf("Message reçu : %s\n", message);
//    }
    return message;
}

/**
 * For an accepted socket descriptor in params, wait until receiving a message.
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

/**
* Shut down the server.
*/
void closeServer(){
    // If the socket is set, we run shutdown on it
    if(serverSocketDescriptor != -1){
        shutdown(serverSocketDescriptor, 2);
    }
    // End of program with success
    printf("Fin du programme. \n");
    exit(EXIT_SUCCESS);
}


int isSocketConnected (int acceptedSocketDescriptor) {
    int error = 0;
    socklen_t len = sizeof (error);
    int retval = getsockopt (acceptedSocketDescriptor, SOL_SOCKET, SO_ERROR, &error, &len);

    printf("Socket : %d | Status : %d. \n",acceptedSocketDescriptor, retval);
    return retval == 0;
}

/**
 * Shutdown client and kille the thread eassociated.
 * @param acceptedSocketDescriptor
 */
void closeClient (Client *client) {
    printf("Déconnexion du client : %d\n", client->id);
//    shutdown(acceptedSocketDescriptor, 2);    // Commented because doesn't close.
    close(client->acceptedSocketDescriptor);    // Close client's socket.
    delete(clientList, client);     // Remove from the list, to node which contain the client.
    pthread_exit(NULL);     // Kill of the thread associated.
}

/**
 * Send the message to every clients connected, excepted the sender.
 *
 * @param acceptedSocketDescriptorSender
 * @param message
 */
void sendBroadcast (int acceptedSocketDescriptorSender, char *message) {
    printf("sendBroadcast. \n");
    if (isEmpty(clientList)) {
        throwError("Client list empty. ", 0);
    }

    Node *current = next(clientList->head);
    while (current != NULL) {
        if (current->client.acceptedSocketDescriptor != acceptedSocketDescriptorSender && isSocketConnected(current->client.acceptedSocketDescriptor)) {
            sendMessage(current->client.acceptedSocketDescriptor, message);
        }
        current = next(current);
    }
}

/**
 * Receive and print messages received, indefinitely.
 *
 * @param acceptedSocketDescriptor
 */
void readingLoop(Client *client){
    while(1){
        /**
        * Now, we wait for the client to send a message.
        */
        char *message = receiveMessage(client->acceptedSocketDescriptor);
        // If the message is the ending message, then we close the connection with the client.
        if (strcmp(ENDING_MESSAGE, message) == 0) {
            // Close the client.
            closeClient(client);
        }
        printf("Message reçu : %s", message);
        sendBroadcast(client->acceptedSocketDescriptor, message);
    }
}

/**
 * Server side.
 */
int main(int argc, char *argv[]) {
/**
 * Server Start.
 */
    printf("Début programme. \n");

    // Assigning function closeServer() to SIGTERM signal
    signal(SIGTERM, closeServer);   // Signal shutdown from ide.
    signal(SIGINT, closeServer);    // Signal shutdown from ctr+c in terminal.


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
    serverSocketDescriptor = launchServer(atoi(argv[1]));
    // Server is launched

/**
 * Starting server loop
 */
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


    int newClientSocketDescriptor;
    int currentClientCount = 0; // TODO : Remove me when usernames are set.
    Client *newClient;
    clientList = createList();

    while(1){
    /**
     * Connect clients.
     */
        // Waiting for a client connection.
        newClientSocketDescriptor = connectToClient();
        newClient = createClient(currentClientCount, newClientSocketDescriptor);
        // Adding Client to clientList
        add(clientList, *newClient);
        // launch client thread
        if (pthread_create(newClient->thread, NULL, readingLoop, newClient)) {
            throwError("Error:unable to create thread, %d\n", 0);
        }
        // upping currentClientCount
        currentClientCount += 1;
    }
}