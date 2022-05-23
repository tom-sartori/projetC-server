/**
 * For a socket descriptor in params, connect it to the client.
 * Use accept function.
 *
 * @param serverSocket
 * @return an accepted socket descriptor.
 */
int connectToClient (int serverSocket) {
    struct sockaddr_in aC;
    socklen_t lg = sizeof(struct sockaddr_in);

    // Waiting for a clients connection
    int acceptedSocketDescriptor = accept(serverSocket, (struct sockaddr*) &aC, &lg);
    int t = 1;
    setsockopt(acceptedSocketDescriptor,SOL_SOCKET,SO_REUSEADDR,&t,sizeof(int));

    // Checking for errors
    if (acceptedSocketDescriptor == -1) {
        throwError("Erreur lors de la connexion. \n", 1);
    }
    else {
        printf("Client Connecté. \n");
    }
    return acceptedSocketDescriptor;
}

/**
 * Return 1 if the socket in params is still connected to the server socket.
 *
 * @param acceptedSocketDescriptor
 * @return
 */
int isSocketConnected (int acceptedSocketDescriptor) {
    int error = 0;
    socklen_t len = sizeof (error);
    int retval = getsockopt (acceptedSocketDescriptor, SOL_SOCKET, SO_ERROR, &error, &len);

    printf("Socket : %d | Status : %d. \n",acceptedSocketDescriptor, retval);
    return retval == 0;
}

/**
 * Shutdown client and kille the thread eassociated.
 * @param acceptedSocketDescriptor
 */
void closeClient (Client *client) {
    printf("Déconnexion du client : %s\n", client->username);
//    shutdown(acceptedSocketDescriptor, 2);    // Commented because doesn't close.
    close(client->acceptedSocketDescriptor);    // Close client's socket.
    delete(clientList, client);     // Remove from the list, to node which contain the client.
    rk_sema_post(&semaphore);
    pthread_exit(NULL);     // Kill of the thread associated.
}