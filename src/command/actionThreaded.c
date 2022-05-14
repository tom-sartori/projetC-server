/**
 * Client send a file which will be stored on the server.
 *
 * @param filename
 * @return
 */
void *receiveFileThreaded (char *filename) {
    // Connection to the file socket.
    int clientFileSocket = connectToClient(serverFileSocketDescriptor);

    receiveFile(clientFileSocket, filename);
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
    int clientFileSocket = connectToClient(serverFileSocketDescriptor);

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

    pthread_exit(NULL);
}
