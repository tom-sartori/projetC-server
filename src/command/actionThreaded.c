/**
 * Client send a file which will be stored on the server.
 *
 * @param filename
 * @return
 */
void *receiveFileThreaded (char *filename) {
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
void *mpFileThreaded (struct paramFileThreaded *param) {
    // Connection to the file socket.
    int clientSourceSocket = connectToClient(channelList[0]->serverSocketDescriptor);

    // Check if client with the username in param exists.
    Client *client = contains(clientList, param->username);
    if (client == NULL || ! isSocketConnected(client->acceptedSocketDescriptor)) {
        // Client doesn't exist or isn't connected.
        printf("Targeted client invalid. \n");
        sendMessageInt(clientSourceSocket, 404);
    } else {
        // Client exists and is connected.
        printf("Targeted client valid. \n");
        // Inform the sender that everything is ok.
        sendMessageInt(clientSourceSocket, 204);
        // Send message to the targeted client. He will try to connect to the socket dedicated to file.
        sendMessage(client->acceptedSocketDescriptor, param->message);

        // Connection of the targeted client at the socket dedicated to file.
        int clientTargetedSocket = connectToClient(channelList[2]->serverSocketDescriptor);

        transferFile(clientSourceSocket, clientTargetedSocket);
    }

    free(param->message);
    free(param->username);
    free(param);

    pthread_exit(NULL);
}
