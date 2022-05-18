
List *clientList;

/**
 * Create a socket and return his descriptor.
 *
 * @return the descriptor of the socket created.
 */
int socketCreation () {
    int socketDescriptor = socket(PF_INET, SOCK_STREAM, 0);
    if (socketDescriptor == -1) {
        throwError("Erreur lors de la création du socket. \n", 0);
    }
    else {
        printf("Socket Créé. \n");
    }

    // Fix errors of bind.
    int enable = 1;
    if (setsockopt(socketDescriptor, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        throwError("setsockopt(SO_REUSEADDR) failed", 0);
    }
    return socketDescriptor;
}

/**
 * Create, bind and make listening a socket.
 *
 * @param port
 * @return the descriptor of the socket created.
 */
int launchServer(int port) {
    int socketDescriptor = socketCreation();

    struct sockaddr_in ad;
    ad.sin_family = AF_INET;
    ad.sin_addr.s_addr = INADDR_ANY;
    ad.sin_port = htons(port);

    if (bind(socketDescriptor, (struct sockaddr*)&ad, sizeof(ad)) == -1) {
        throwError("Erreur lors du bind. \n", 1);
    }
    else {
        printf("Socket Nommé\n");
    }

    if (listen(socketDescriptor, 7) == -1) {
        throwError("Erreur lors du listen. \n",  1);
    }
    else {
        printf("Mode écoute\n");
    }
    return socketDescriptor;
}

/**
 * Shut down the server.
 *
 * @param serverSocketDescriptor
 */
void closeSocket(int serverSocketDescriptor){
    // If the socket is set, we run shutdown on it
    if(serverSocketDescriptor != -1){
        shutdown(serverSocketDescriptor, 2);    // Not working for mac.
        close(serverSocketDescriptor);          // Works for mac.
    }
}

void finishProgram () {
    for (int i = 0; i < NB_CHANNEL; i++) {
        closeSocket(channelList[i]->serverSocketDescriptor);
    }

    // End of program with success
    printf("Fin du programme. \n");
    exit(EXIT_SUCCESS);
}
