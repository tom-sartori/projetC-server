/**
 * Client send a file which will be stored on the server.
 *
 * @param filename
 * @return
 */
void *receiveFileThreaded (void *filename) {
    filename = (char *)filename;

    // Connection with the client to the switch socket. After that, send the new port.
    sendPort(channelList[INDEX_FILE_CHANNEL]->port);

    // Connection to the file socket.
    int clientFileSocket = connectToClient(channelList[0]->serverSocketDescriptor);

    receiveFile(clientFileSocket, filename);

    free(filename);
    pthread_exit(NULL);
}

/**
 * The client ask for a file which is stored on the server.
 *
 * @param filename
 * @return
 */
void *sendFileThreaded (void *filename) {
    filename = (char *)filename;

    // Connection with the client to the switch socket. After that, send the new port.
    sendPort(channelList[INDEX_FILE_CHANNEL]->port);

    // Connection to the file socket.
    int clientFileSocket = connectToClient(channelList[0]->serverSocketDescriptor);

    // Get current path.
    char filePath[200];
    getUploadDirectoryPath(filePath);
    strcat(filePath, filename);

    FILE *file;
    file = fopen(filePath, "rb");
    if (file != NULL) {
        // File exists.
        printf("File exists. \n");
        sendMessageInt(clientFileSocket, 204);
        sendFile(clientFileSocket, file);
    }
    else {
        // File doesn't exists.
        printf("File not found. \n");
        sendMessageInt(clientFileSocket, 404);
    }

    free(filename);
    pthread_exit(NULL);
}

// Struct used for threaded function.
struct paramFileThreaded {
    char *message;
    char *username;
};
/**
 * Threaded function called when a user wanna mp a file to another user.
 * Check if targeted user is connected and call transferFile(...).
 * @param param
 * @return
 */
void *mpFileThreaded (void *voidParam) {
    struct paramFileThreaded *param = (struct paramFileThreaded *)voidParam;

    // Connection with the clientTargeted to the switch socket. After that, send the new port.
    sendPort(channelList[INDEX_FILE_CHANNEL]->port);

    // Connection to the file socket.
    int clientSourceSocket = connectToClient(channelList[INDEX_FILE_CHANNEL]->serverSocketDescriptor);

    // Check if clientTargeted with the username in param exists.
    Client *clientTargeted = contains(clientList, param->username);
    if (clientTargeted == NULL || ! isSocketConnected(clientTargeted->acceptedSocketDescriptor)) {
        // Client doesn't exist or isn't connected.
        printf("Targeted clientTargeted invalid. \n");
        sendMessageInt(clientSourceSocket, 404);
    }
    else {
        // Client exists and is connected.
        printf("Targeted clientTargeted valid. \n");
        // Inform the sender that everything is ok.
        sendMessageInt(clientSourceSocket, 204);
        // Send message to the targeted clientTargeted. He will try to connect to the socket dedicated to file.
        sendMessage(clientTargeted->acceptedSocketDescriptor, param->message);


        // Connection with the clientTargeted to the switch socket. After that, send the new port.
        sendPort(channelList[INDEX_FILE_CHANNEL]->port);

        // Connection of the targeted clientTargeted at the socket dedicated to file.
        int clientTargetedSocket = connectToClient(channelList[INDEX_FILE_CHANNEL]->serverSocketDescriptor);

        transferFile(clientSourceSocket, clientTargetedSocket);
    }

    free(param->message);
    free(param->username);
    free(param);

    printf("File transfered. \n");
    pthread_exit(NULL);
}
