struct Channel {
    char *name;
    int port;
    int serverSocketDescriptor;
    int isPublic;
    // User list ?
    // isPublic ? : For file socket.
};
typedef struct Channel Channel;