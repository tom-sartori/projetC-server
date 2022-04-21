
struct Client {
    int id;
    char *username; // If unique, remove id ?
    int acceptedSocketDescriptor;
    pthread_t thread;
};
typedef struct Client Client;

Client *createClient (int id, int acceptedSocketDescriptor, char *username) {
    Client *client = (Client *)malloc(sizeof(Client));
    client->id = id;
    client->acceptedSocketDescriptor = acceptedSocketDescriptor;
    client->thread = (pthread_t)malloc(sizeof(pthread_t));
    client->username = username;

    return client;
}
