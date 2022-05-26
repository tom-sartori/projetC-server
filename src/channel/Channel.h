struct Channel {
    char *name;
    int port;
    int serverSocketDescriptor;
    int isPublic;   // Used for file and default socket.
    int size;
    List *clientList;
};
typedef struct Channel Channel;

int isFull (Channel *channel);
void getChannelName (int channelIndex, char *buffer, int bufferSize);
void getFullChannelName (int channelIndex, char *buffer, int bufferSize);
