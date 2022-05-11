#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>

#include "global.c"
#include "util/semaphore.c"
#include "util/color.c"
#include "util/error.c"
#include "util/regex.c"
#include "util/util.c"

#include "client/Client.c"
#include "client/List.c"

#include "command/Command.c"

#include "socket/serverSocket.c"
#include "socket/clientSocket.c"
#include "socket/receive.c"
#include "socket/send.c"
#include "command/action.c"



/**
 * Wait for the client to send a valid username.
 * If the username already exists, ask again.
 * If the client logout, it returns NULL.
 *
 * @param newClientSocketDescriptor
 * @return the username or NULL if the client logout.
 */
char *askForUsername (int newClientSocketDescriptor) {
    char *username;
    int hasUniqueUsername = 0;
    do {
        username = receiveMessage(newClientSocketDescriptor);
        if (isMatch(username, commandList[1]->regex)) {
            return NULL;
        }
        else if(contains(clientList, username) == NULL){
            sendMessageInt(newClientSocketDescriptor,201);
            hasUniqueUsername = 1;
        }
        else{
            sendMessageInt(newClientSocketDescriptor,409);
        }
    }
    while(!hasUniqueUsername);

    return username;
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
        printf("%s: %s", client->username, message);
        // If the message is a command, do the command, else send broadcast to all connected clients
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

    // Launch second socket for files.
    serverFileSocketDescriptor = launchServer(PORT_SOCKET_FILE);

    rk_sema_init(&semaphore, NB_MAX_CLIENT);
    initCommandList();

    clientList = createList();
    int newClientSocketDescriptor;
    Client *newClient;

/**
 * Starting server loop
 */

    /**
     * Connect clients.
     */
    while(1){
        // Wait for a place.
        rk_sema_wait(&semaphore);
        // Waiting for a client connection.
        newClientSocketDescriptor = connectToClient(serverSocketDescriptor);
        char *username = askForUsername(newClientSocketDescriptor);
        if (username == NULL) {
            // User has been logout.
            close(newClientSocketDescriptor);
            rk_sema_post(&semaphore);
        }
        else {
            newClient = createClient(username, newClientSocketDescriptor);
            // Adding Client to clientList
            add(clientList, *newClient);
            // launch client thread
            if (pthread_create(newClient->thread, NULL, readingLoop, newClient)) {
                throwError("Error : unable to create thread, %d\n", 0);
            }
        }
    }
}
