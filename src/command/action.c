#include <dirent.h>

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
    // Create thread
    // Create socket
    // Send socket port
    // Wait client connection at new socket
    // Recv data and write into file
    // Client close socket, then exit thread

    char *regexGroupList[3];
    getRegexGroup(regexGroupList, message, command->regex);

    pthread_t fileThread;
    pthread_create(&fileThread, NULL, receiveFile, regexGroupList[1]);
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
