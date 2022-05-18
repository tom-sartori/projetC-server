#define MAX_SIZE_SENT 512
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

#define NB_MAX_CLIENT 100
List *clientList;

#define NB_COMMAND 9
Command *commandList[NB_COMMAND];

#define NB_CHANNEL 3
Channel *channelList[NB_CHANNEL];
