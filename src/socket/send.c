/**
 * For an accepted socket descriptor in params, send a message of type int.
 * This message is usually used to send the message's size which will be sent after. Or also, it sends a status.
 *
 * @param acceptedSocketDescriptor
 * @param messageInt
 */
void sendMessageInt (int acceptedSocketDescriptor, int messageInt) {
    // Sending the message's size to the client.
    if(send(acceptedSocketDescriptor, &messageInt, sizeof(int), 0) == -1){
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
 * Send the message to every clients connected, excepted the sender.
 *
 * @param acceptedSocketDescriptorSender
 * @param message
 */
void sendBroadcast (Client *client, char *message) {
    printf("sendBroadcast. \n");
    if (isEmpty(clientList)) {
        throwError("Client list empty. ", 0);
    }

    Node *current = next(clientList->head);
    while (current != NULL) {
        if (current->client.acceptedSocketDescriptor != client->acceptedSocketDescriptor && isSocketConnected(current->client.acceptedSocketDescriptor)) {
            char* name = client->username;
            char* msgWithClientName = (char*)malloc(sizeof(char)*(strlen(name)+strlen(message)));
            strcat(msgWithClientName, name);
            strcat(msgWithClientName,": ");
            strcat(msgWithClientName, message);
            //message[strlen(message)-1] = '\0';
            //msgWithClientName = strcat(message,name);
            //strcat(msgWithClientName, "\n");
            sendMessage(current->client.acceptedSocketDescriptor, msgWithClientName);
        }
        current = next(current);
    }
}
