
Client *createClient (char *username, int acceptedSocketDescriptor, int indexCurrentChannel) {
    Client *client = (Client *)malloc(sizeof(Client));
    client->username = username;
    client->acceptedSocketDescriptor = acceptedSocketDescriptor;
    client->indexCurrentChannel = indexCurrentChannel;
    client->thread = (pthread_t)malloc(sizeof(pthread_t));

    return client;
}
