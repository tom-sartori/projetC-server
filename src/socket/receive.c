/**
 * For an accepted socket descriptor in params, wait until receiving an int message.
 * This message is usually the size of the message which will be sent just after.
 *
 * @param acceptedSocketDescriptor
 * @return the size of the message which will be sent after.
 */
int receiveMessageInt (int acceptedSocketDescriptor) {
    int size;
    if(recv(acceptedSocketDescriptor, &size, sizeof(int), 0) == -1){
        throwError("Erreur lors de la reception du message. \n", 0);
    }
//    else {
//        printf("Message de taille %d recu. \n", size);
//    }
    return size;
}

/**
 * For an accepted socket descriptor in params, wait until receiving a message of the size in params.
 *
 * @param acceptedSocketDescriptor
 * @param size
 * @return the gotten message.
 */
char *receiveMessageString (int acceptedSocketDescriptor, int size) {
    char *message = (char*)malloc(sizeof(char) * (size + 1));
    bzero(message, sizeof(char) * (size + 1));

    if(recv(acceptedSocketDescriptor, message, sizeof(char) * size, 0) == -1){
        throwError("Erreur lors de la reception du message. \n", 0);
    }

    return message;
}

/**
 * For an accepted socket descriptor in params, wait until receiving a message.
 * @param acceptedSocketDescriptor
 * @return
 */
char *receiveMessage (int acceptedSocketDescriptor) {
    // Receive messageSize.
    int messageSize = receiveMessageInt(acceptedSocketDescriptor);
    // Receive message.
    return receiveMessageString(acceptedSocketDescriptor, messageSize);
}

/**
 * Receive a file from a client.
 *
 * @param clientFileSocket
 * @param fileName
 */
void receiveFile(int clientFileSocket, char *fileName){
    // Get file blocSize.
    long filelen;
    recv(clientFileSocket, &filelen, sizeof(long), 0);

    // Get current path.
    char filePath[200];
    getUploadDirectoryPath(filePath);
    strcat(filePath, fileName);

    // File creation.
    FILE *file;
    file = fopen(filePath, "wb");


    long blocSize = MAX_SIZE_SENT;
    char subBuffer[blocSize];
    for (int i = 0; i < filelen; i += MAX_SIZE_SENT) {
        blocSize = (i + MAX_SIZE_SENT < filelen) ? MAX_SIZE_SENT : filelen - i; // Calculate the bloc's blocSize.
        recv(clientFileSocket, subBuffer, blocSize, 0);
        fwrite(subBuffer, blocSize, 1, file);
        bzero(subBuffer, MAX_SIZE_SENT);    // Clear the buffer.
    }

    fclose(file);
    printf("File imported. \n");
}
