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
    //int test = ntohl(size);
    char *message = (char*)malloc(sizeof(char) * (size + 1));

    if(recv(acceptedSocketDescriptor, message, sizeof(char) * size, 0) == -1){
        throwError("Erreur lors de la reception du message. \n", 0);
    }
//    else {
//        printf("Message reçu : %s\n", message);
//    }
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