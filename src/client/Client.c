
struct Client {
    char *username;
    int acceptedSocketDescriptor;
    pthread_t thread;
};
typedef struct Client Client;

Client *createClient (char *username, int acceptedSocketDescriptor) {
    Client *client = (Client *)malloc(sizeof(Client));
    client->username = username;
    client->acceptedSocketDescriptor = acceptedSocketDescriptor;
    client->thread = (pthread_t)malloc(sizeof(pthread_t));

    return client;
}