#include <dirent.h>
#include "actionThreaded.c"

/**
 * Regroup every actions which are called by commands.
 */


/**
 * Help action which send to the user a list of commands that he can use.
 *
 * @param acceptedSocketDescriptor
 */
void helpAction (int acceptedSocketDescriptor) {
    char *text = (char *) malloc(sizeof(char) * 1000);
    strcat(text, "Help : \n");

    for (int i = 0; i < NB_COMMAND; i++) {
        strcat(text, commandList[i]->name);
        strcat(text, " : ");
        strcat(text, commandList[i]->description);
    }

    strcat(text, "\n");

    sendMessage(acceptedSocketDescriptor, text);

    free(text);
}

/**
 * Disconnect action which is called by the method /disconnect.
 *
 * @param client
 */
void disconnectAction (Client *client) {
    closeClient(client);
}

void usersAction (Client *client) {
    char *text = (char *) malloc(sizeof(char) * 1000);
    strcpy(text, "Liste des utilisateurs : \n");

    Node *current = next(clientList->head);
    while (current != NULL) {
        if (isSocketConnected(current->client->acceptedSocketDescriptor)) {
            strcat(text, current->client->username);
            strcat(text, "\n");
        }
        current = next(current);
    }

    strcat(text, "\n");
    sendMessage(client->acceptedSocketDescriptor, text);
    free(text);
}

/**
 * Private message action which is called by command /mp user message.
 *
 * @param clientSender
 * @param command
 * @param message
 */
void mpAction (Client *clientSender, Command *command, char *message) {
    // Get other username and message.
    // If username exists.
    // Send message to him.
    // Else
    // Inform clientSender that username doesn't exist.

    char *regexGroupList[3];
    getRegexGroup(regexGroupList, message, command->regex);
    // regexGroupList[0] == full message.
    // regexGroupList[1] == username targeted.
    // regexGroupList[2] == message to send to the targeted client.

    Client *clientTargeted = contains(clientList, regexGroupList[1]);

    if (clientTargeted == NULL) {
        sendMessage(clientSender->acceptedSocketDescriptor, "User not found. \n");
    }
    else {
        size_t sizeOfMessage = (strlen(clientTargeted->username) + strlen(regexGroupList[2]) + 8) * sizeof(char);

        // Message for sender.
        char messageForSender[sizeOfMessage];
        char *tabSender[] = {"Me->", clientTargeted->username, regexGroupList[2], "\n" };
        strcatArray(messageForSender, (int)sizeOfMessage, tabSender, 4);

        // Message for target.
        char messageForTarget[sizeOfMessage];
        char *tabTarget[] = { clientSender->username, "->Me : ", regexGroupList[2], "\n" };
        strcatArray(messageForTarget, (int)sizeOfMessage, tabTarget, 4);

        sendMessage(clientTargeted->acceptedSocketDescriptor, messageForTarget);
        sendMessage(clientSender->acceptedSocketDescriptor, messageForSender);
    }
//    free(regexGroupList[0]);
//    free(regexGroupList[1]);
//    free(regexGroupList[2]);
}

/**
 * Action used to send a file to the server.
 * Create a thread which use fileActionThreaded(...).
 *
 * @param command
 * @param message
 */
void fileAction (Command *command, char *message) {
    // /file -send filename 			Envoie un fichier au serveur.
    // /file -get filename				Récupère un fichier du serveur.
    // /file username filename 		    Envoie un fichier à un autre utilisateur connecté.

    // Get regex groups.
    char *regexGroupList[3];
    getRegexGroup(regexGroupList, message, command->regex);
    // regexGroupList[0] = message
    // regexGroupList[1] = -send || -get || username
    // regexGroupList[2] = filename

    pthread_t fileThread;

    if (strcmp("-send", regexGroupList[1]) == 0) {
        // User send file to the server.
        printf("file -send filename\n");
        pthread_create(&fileThread, NULL, receiveFileThreaded, regexGroupList[2]);
    } else if (strcmp("-get", regexGroupList[1]) == 0) {
        // User get file from the server.
        printf("file -get filename\n");
        pthread_create(&fileThread, NULL, sendFileThreaded, regexGroupList[2]);
    } else {
        // User send file to other user.
        printf("file username filename\n");

        struct paramFileThreaded *param;
        param = malloc(sizeof(struct paramFileThreaded));
        param->message = message;
        param->username = regexGroupList[1];    // -send || -get || username

        pthread_create(&fileThread, NULL, mpFileThreaded, param);

        free(regexGroupList[0]);
//    free(regexGroupList[1]);
//    free(regexGroupList[2]);
    }
}

/**
 * Action called when the user call an unknown command.
 *
 * @param client
 */
void unknownAction (Client *client) {
    sendMessage(client->acceptedSocketDescriptor, "Commande inconnue. /help pour avoir la liste des commandes. \n");
}

/**
 * Print directory's names within uploads/.
 *
 * @param client
 */
void filesAction (Client *client) {
    // Get uploads path.
    char uploadDirectoryPath[200];
    getUploadDirectoryPath(uploadDirectoryPath);

    // Get directory.
    DIR *directory;
    struct dirent *file;
    directory = opendir(uploadDirectoryPath);
    if (directory == NULL) {
        throwError("Unable to open the directory. \n", 0);
    }

    char stringNameFile[300];   // String containing name of files within /uploads.
    bzero(stringNameFile, 300);    // Clear the buffer.
    int n = 0;
    while ((file = readdir(directory)) != NULL) {
        if (strcmp(file->d_name, ".") != 0 && strcmp(file->d_name, "..") != 0) {  // Don't take files : "." et "..".
//            printf("File %d : %s\n", n, file->d_name);
            strcat(stringNameFile, "\t");
            strcat(stringNameFile, file->d_name); // Add file to the list.
            strcat(stringNameFile, "\n");
            n++;
        }
    }
    closedir(directory);
    strcat(stringNameFile, "\n");
    sendMessage(client->acceptedSocketDescriptor, stringNameFile);
}

/**
 * Action called by an user to disconnect another user.
 *
 * @param clientKicker
 * @param command
 * @param message
 */
void kickAction (Client *clientKicker, Command *command, char *message) {
    // Get regex groups.
    char *regexGroupList[3];
    getRegexGroup(regexGroupList, message, command->regex);
    // regexGroupList[1] = username to kick.

    Client *clientToKick = contains(clientList, regexGroupList[1]);
    if (clientToKick == NULL) {
        // User not found.
        sendMessage(clientKicker->acceptedSocketDescriptor, "Client not found. \n");
    }
    else {
        // User to kick found.
        sendMessage(clientToKick->acceptedSocketDescriptor, "/disconnect\n");
        sendMessage(clientKicker->acceptedSocketDescriptor, "Client kicked. \n");
    }

    free(regexGroupList[0]);
    free(regexGroupList[1]);
    free(regexGroupList[2]);
}

/**
 * Action which rename the user.
 *
 * @param client
 * @param command
 * @param message
 */
void renameAction (Client *client, Command *command, char *message) {
    // Get regex groups.
    char *regexGroupList[3];
    getRegexGroup(regexGroupList, message, command->regex);
    // regexGroupList[1] = new username.

    if (contains(clientList, regexGroupList[1])) {
        // Username already exists.
        sendMessage(client->acceptedSocketDescriptor, "This username is already used. \n");
    }
    else {
        // Username valid.
        strcpy(client->username, regexGroupList[1]);
        sendMessage(client->acceptedSocketDescriptor, "Username updated. \n");
    }

    free(regexGroupList[0]);
    free(regexGroupList[1]);
    free(regexGroupList[2]);
}

/**
 * Action which send to the client his current channel.
 *
 * @param client
 */
void channelAction (Client *client) {
    int bufferSize = 200;
    char buffer[bufferSize];
    getFullChannelName(client->indexCurrentChannel, buffer, bufferSize);

    printf("channelAction : %s", buffer);
    sendMessage(client->acceptedSocketDescriptor, buffer);
}

/**
 * Action which send to the client a list of every public sockets.
 *
 * @param client
 */
void channelsAction (Client *client) {
    int bufferSize = 800;
    char buffer[bufferSize];
    bzero(buffer, bufferSize);

    int subBufferSize = 200;
    char subBuffer[subBufferSize];

    for (int i = 0; i < NB_CHANNEL; i++) {
        if (channelList[i]->isPublic) {
            strcat(buffer, "\t");
            getFullChannelName(i, subBuffer, subBufferSize);
            strcat(buffer, subBuffer);
        }
    }

    strcat(buffer, "\n");

    sendMessage(client->acceptedSocketDescriptor, buffer);
}

/**
 * Switch a client from a channel to another.
 *
 * @param client
 * @param command
 * @param message
 */
void joinAction (Client *client, Command *command, char *message) {
    // Check that wanted channel exists.
    // Send the port of this socket.
    // Wait for client's connection to the new socket.
    // Close old socket. Or close from client ?
    // Modify client->socketDescriptor.

    // Get regex groups.
    char *regexGroupList[3];
    getRegexGroup(regexGroupList, message, command->regex);
    int channelIndex = atoi(regexGroupList[1]);

    // Connect to the switch socket.
    int clientSwitchSocket = connectToClient(channelList[INDEX_SWITCH_CHANNEL]->serverSocketDescriptor);

    if (channelIndex == client->indexCurrentChannel) {
        // User already in this channel.
        // Send error status.
        sendMessageInt(clientSwitchSocket, 400);
    }
    else if (channelIndex < 0 || channelIndex >= NB_CHANNEL || ! channelList[channelIndex]->isPublic) {
        // Channel invalid.
        // Send error status.
        sendMessageInt(clientSwitchSocket, 404);
    }
    else if (isFull(channelList[channelIndex])) {
        // Channel already full.
        // Send error status.
        sendMessageInt(clientSwitchSocket, 409);
    }
    else {
        // Valid channel.
        // Connect to switch channel and send port in it.
        sendMessageInt(clientSwitchSocket, channelList[channelIndex]->port);
        close(clientSwitchSocket);

        int newClientSocket = connectToClient(channelList[channelIndex]->serverSocketDescriptor);

        int oldSocket = client->acceptedSocketDescriptor;
        int oldIndexChannel = client->indexCurrentChannel;
        client->acceptedSocketDescriptor = newClientSocket;
        client->indexCurrentChannel = channelIndex;


        char *resetMessage = "\0";  // Need to send this reset message because the client is blocked in receiveMessage(...).
        sendMessageInt(oldSocket, 1);  // Client blocked in recv int with the old socket.
        sendMessageString(newClientSocket, resetMessage, 1);

        // Get old channel name.
        char oldChannelName[100];
        getChannelName(channelIndex, oldChannelName, 100);
        sendMessage(oldSocket, oldChannelName);    // Send the new channel name to the user.
        close(oldSocket);

        delete(channelList[oldIndexChannel]->clientList, client);
        add(channelList[channelIndex]->clientList, client);
    }

    free(regexGroupList[0]);
    free(regexGroupList[1]);
    free(regexGroupList[2]);
}

/**
 * Send a message to every clients.
 *
 * @param client
 * @param command
 * @param message
 */
void allAction (Client *client, Command *command, char *message) {
    if (isEmpty(clientList)) {
        throwError("Client list empty. \n", 0);
    }

    // Get regex groups.
    char *regexGroupList[3];
    getRegexGroup(regexGroupList, message, command->regex);
    // regexGroupList[1] == message to send.

    Node *current = next(clientList->head);
    while (current != NULL) {
        if (isSocketConnected(current->client->acceptedSocketDescriptor)) {
            char *msgWithClientName;
            size_t msgSize;
            if(current->client->acceptedSocketDescriptor != client->acceptedSocketDescriptor){
                msgSize = strlen(client->username) + strlen(regexGroupList[1]) + 10;
                msgWithClientName = (char*)malloc(msgSize);
                bzero(msgWithClientName, msgSize);
                strcat(msgWithClientName, client->username);
                strcat(msgWithClientName, "->ALL");
            }
            else {
                msgSize = strlen(regexGroupList[1]) + 12;
                msgWithClientName = (char*)malloc(msgSize);
                bzero(msgWithClientName, msgSize);
                strcat(msgWithClientName, "Me->");
                strcat(msgWithClientName, "ALL");
            }
            strcat(msgWithClientName," : ");
            strcat(msgWithClientName, regexGroupList[1]);
            strcat(msgWithClientName, "\n");
            sendMessage(current->client->acceptedSocketDescriptor, msgWithClientName);
            free(msgWithClientName);
        }
        current = next(current);
    }

    free(regexGroupList[0]);
    free(regexGroupList[1]);
    free(regexGroupList[2]);
}
