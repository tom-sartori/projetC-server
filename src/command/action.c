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
        if (isSocketConnected(current->client.acceptedSocketDescriptor)) {
            strcat(text, current->client.username);
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

    Client *clientTargeted = contains(clientList, regexGroupList[1]);

    if (clientTargeted == NULL) {
        /// TODO Send status ?
        sendMessage(clientSender->acceptedSocketDescriptor, "User not found. \n");
    }
    else {
        // TODO Rajouter le pseudo du sender
        // TODO Faire une fonction du realloc

//        regexGroupList[2] = realloc(regexGroupList[2], sizeof(char)* strlen(regexGroupList[2])+2);
//        strcat(regexGroupList[2], "\n");
        // TODO Faire un check client regexp pour savoir si le message est un mp et l'afficher en bleu si c'est le cas
        char* messageToSend = (char*)malloc((strlen(regexGroupList[2])+2)+ strlen(clientSender->username+2)+3);
        strcat(messageToSend, "MP ");
        strcat(messageToSend, clientSender->username);
        strcat(messageToSend, ": ");
        strcat(messageToSend,regexGroupList[2]);
        strcat(messageToSend,"\n");
//        sendMessage(clientTargeted->acceptedSocketDescriptor, regexGroupList[2]);
        sendMessage(clientTargeted->acceptedSocketDescriptor, messageToSend);
        free(messageToSend);
    }
    free(regexGroupList[0]);
    free(regexGroupList[1]);
    free(regexGroupList[2]);
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

void renameAction (Client *client, Command *command, char *message) {
    // Get regex groups.
    char *regexGroupList[3];
    getRegexGroup(regexGroupList, message, command->regex);
    // regexGroupList[1] = new username.

    strcpy(client->username, regexGroupList[1]);
    sendMessage(client->acceptedSocketDescriptor, "Nom d'utilisateur mis à jour. \n");

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
    int bufferSize = 100;
    char buffer[bufferSize];
    bzero(buffer, bufferSize);

    sprintf(buffer, "%d", client->indexCurrentChannel);
//    strcat(buffer, sprintf(NULL));
    strcat(buffer, " : ");
    strcat(buffer, channelList[client->indexCurrentChannel]->name);
    strcat(buffer, "\n");

    sendMessage(client->acceptedSocketDescriptor, buffer);
}

/**
 * Action which send to the client a list of every public sockets.
 *
 * @param client
 */
void channelsAction (Client *client) {
    int bufferSize = 300;
    char resultString[bufferSize];
    bzero(resultString, bufferSize);

    for (int i = 0; i < NB_CHANNEL; i++) {
        if (channelList[i]->isPublic) {
            strcat(resultString, channelList[i]->name);
            strcat(resultString, "\n");
        }
    }
    sendMessage(client->acceptedSocketDescriptor, resultString);
}
