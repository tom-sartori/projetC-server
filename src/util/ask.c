
/**
 * Wait for the client to send a valid username.
 * If the username already exists, ask again.
 * If the client logout, it returns NULL.
 *
 * @param newClientSocketDescriptor
 * @return the username or NULL if the client logout.
 */
char *askForUsername (int newClientSocketDescriptor) {
    char *username;
    int hasUniqueUsername = 0;
    do {
        username = receiveMessage(newClientSocketDescriptor);
        if (isMatch(username, commandList[1]->regex)) {
            return NULL;
        }
        else if(isMatch(username, "^([^/ ])*$") != 0 && contains(clientList, username) == NULL){
            sendMessageInt(newClientSocketDescriptor,201);
            hasUniqueUsername = 1;
        }
        else{
            sendMessageInt(newClientSocketDescriptor,409);
        }
    }
    while(!hasUniqueUsername);

    return username;
}