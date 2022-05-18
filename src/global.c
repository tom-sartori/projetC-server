#define MAX_SIZE_SENT 512
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
#define NB_MAX_CLIENT 100
List *clientList;

#define PORT_SOCKET_FILE 3333
#define NB_COMMAND 8
Command *commandList[NB_COMMAND];

#define NB_CHANNEL 3
Channel *channelList[NB_CHANNEL];
