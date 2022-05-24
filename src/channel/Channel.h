struct Channel {
    char *name;
    int serverSocketDescriptor;
    int isPublic;
    // User list ?
    // isPublic ? : For file socket.
};
typedef struct Channel Channel;