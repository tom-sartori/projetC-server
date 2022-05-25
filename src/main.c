#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>

#include "client/Client.h"
#include "client/Node.h"
#include "client/List.h"
#include "channel/Channel.h"
#include "command/Command.h"
#include "global.c"

#include "util/color.c"
#include "util/error.c"
#include "util/regex.c"
#include "util/util.c"
#include "util/semaphore.c"

#include "client/Client.c"
#include "client/List.c"

#include "socket/serverSocket.c"
#include "socket/clientSocket.c"
#include "socket/receive.c"
#include "socket/send.c"
#include "socket/transfer.c"

#include "util/ask.c"

#include "command/router.c"

#include "channel/Channel.c"




/**
 * Receive and print messages received, indefinitely.
 *
 * @param client
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
            sendToChannel(client, message);
        }
    }
}

void *createNewClient (int newClientSocketDescriptor) {
    char *username = askForUsername(newClientSocketDescriptor);
    if (username == NULL) {
        // User has been logout.
        close(newClientSocketDescriptor);
        rk_sema_post(&semaphore);
        pthread_exit(NULL);
    }
    else {
        Client *newClient = createClient(username, newClientSocketDescriptor, INDEX_DEFAULT_CHANNEL);
        newClient->thread = pthread_self();

        // Adding Client to clientList
        add(clientList, newClient);
        add(channelList[INDEX_DEFAULT_CHANNEL]->clientList, newClient);
        // launch client thread

        readingLoop(newClient);
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

    // Assigning function closeSocket() to SIGTERM signal
    signal(SIGTERM, finishProgram);   // Signal shutdown from ide.
    signal(SIGINT, finishProgram);    // Signal shutdown from ctr+c in terminal.


/**
 * Launch server.
 */
    // Initialize the list of channels and launch them.
    initChannelList();
    // Servers are launched


    rk_sema_init(&semaphore, NB_MAX_CLIENT);
    initCommandList();

    clientList = createList();
    int newClientSocketDescriptor;
    pthread_t pthread;

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
        newClientSocketDescriptor = connectToClient(channelList[INDEX_DEFAULT_CHANNEL]->serverSocketDescriptor);    // Connect to default socket.
        // launch client thread
        if (pthread_create(&pthread, NULL, createNewClient, newClientSocketDescriptor)) {
            throwError("Error : unable to create thread, %d\n", 0);
        }
    }
}
