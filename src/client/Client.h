
struct Client {
    char *username;
    int acceptedSocketDescriptor;
    pthread_t thread;
};
typedef struct Client Client;