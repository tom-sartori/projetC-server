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
        if (isSocketConnected(current->client->acceptedSocketDescriptor)) {
            char* msgWithClientName;
            if(current->client->acceptedSocketDescriptor != client->acceptedSocketDescriptor){
                msgWithClientName = (char*)malloc(strlen(client->username)+strlen(message)+3);
                bzero(msgWithClientName, strlen(client->username)+strlen(message)+3);
                strcat(msgWithClientName, client->username);
            }
            else{
                msgWithClientName = (char*)malloc(strlen(message)+5);
                bzero(msgWithClientName, strlen(message)+5);
                strcat(msgWithClientName, "Me");
            }
            strcat(msgWithClientName,": ");
            strcat(msgWithClientName, message);
            sendMessage(current->client->acceptedSocketDescriptor, msgWithClientName);
            free(msgWithClientName);
        }
        current = next(current);
    }
}

/**
 * Send the file in params to the client's socket also in params.
 * The file is sent by blocs of size MAX_SIZE_SENT.
 *
 * @param clientSocketForFile
 * @param file
 */
void sendFile(int clientSocketForFile, FILE *file){
    // Get file blocSize.
    long fileSize;
    fseek(file, 0, SEEK_END);           // Jump to the end of the file.
    fileSize = ftell(file);             // Get the current byte offset in the file.
    rewind(file);                      // Jump back to the beginning of the file.

    // Send file blocSize to the server.
    send(clientSocketForFile, &fileSize, sizeof(long), 0);

    // Send the file bloc per bloc.
    long blocSize = MAX_SIZE_SENT;
    char subBuffer[blocSize];
    for (int i = 0; i < fileSize; i += MAX_SIZE_SENT) {
        blocSize = (i + MAX_SIZE_SENT < fileSize) ? MAX_SIZE_SENT : fileSize - i;   // Calcul the bloc's size.
        fread(subBuffer, blocSize, 1, file); // Read in the file.

        if (send(clientSocketForFile, subBuffer, sizeof(subBuffer), 0) == -1) {   // Send bloc of data.
            throwError("Error in sending file. \n", 1);
        }
        bzero(subBuffer, MAX_SIZE_SENT);    // Clear the buffer.
    }

    fclose(file); // Close the file
    printf("File sent. \n");
}
