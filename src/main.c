#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>

#include "util/error.c"
#include "util/color.c"

#include "client/Client.c"
#include "client/List.c"

#include "command/regex.c"
#include "command/Command.c"


#include "socket/serverSocket.c"
#include "socket/clientSocket.c"
#include "socket/receive.c"
#include "socket/send.c"
#include "command/action.c"



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
        printf("%s: %s", client->username, message);
        // If the message is the ending message, then we close the connection with the client.
        /// TODO Check if message is function.
        if (isCommand(message)) {
            doCommandAction(client, message);
        }
        else {
            sendBroadcast(client, message);
        }
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

    initCommandList();

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
        int hasUniqueUsername = 0;
        char *username;
        while(!hasUniqueUsername){
            username = receiveMessage(newClientSocketDescriptor);
            if(contains(clientList, username) == NULL){
                sendMessageInt(newClientSocketDescriptor,201);
                hasUniqueUsername = 1;
            }
            else{
                sendMessageInt(newClientSocketDescriptor,409);
            }
        }
        newClient = createClient(currentClientCount, newClientSocketDescriptor, username);
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
