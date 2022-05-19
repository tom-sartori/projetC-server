
struct Client {
    char *username;
    int acceptedSocketDescriptor;
    int indexCurrentChannel;    // Index of the current channel in the list channelList.
    pthread_t thread;
};
typedef struct Client Client;
