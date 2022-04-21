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
        sendMessage(clientSender->acceptedSocketDescriptor, "Used not found. \n");
    }
    else {
        sendMessage(clientTargeted->acceptedSocketDescriptor, regexGroupList[2]);
    }

    free(regexGroupList[0]);
    free(regexGroupList[1]);
    free(regexGroupList[2]);
}

/**
 * Action called when the user call an unknown commad.
 *
 * @param client
 */
void unknownAction (Client *client) {
    sendMessage(client->acceptedSocketDescriptor, "Commande inconnue. /help pour avoir la liste des commandes. \n");
}

/**
 * Find the wanted command in the message in params (with regex).
 * Call the action attached to this command.
 *
 * @param client
 * @param message
 */
void doCommandAction (Client *client, char *message) {
    Command *command = getCommand(message);

    if (command == NULL) {
        // Unknown action.
        unknownAction(client);
    }
    else if (strcmp("help", command->name) == 0) {
        // Help.
        helpAction(client->acceptedSocketDescriptor);
    }
    else if (strcmp("disconnect", command->name) == 0) {
        // Disconnection.
        disconnectAction(client);
    }
    else if (strcmp("mp", command->name) == 0) {
        // Private message.
        mpAction(client, command, message);
    }
    else {
        // Unknown action.
        unknownAction(client);
    }
}
