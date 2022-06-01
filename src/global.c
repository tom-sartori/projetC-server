#define MAX_SIZE_SENT 512
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

#define NB_MAX_CLIENT 100
List *clientList;

#define NB_COMMAND 14
Command *commandList[NB_COMMAND];

#define NB_CHANNEL 9
Channel *channelList[NB_CHANNEL];
#define INDEX_FILE_CHANNEL 0
#define INDEX_SWITCH_CHANNEL 1
#define INDEX_DEFAULT_CHANNEL 2
#define DEFAULT_CHANNEL_SIZE 10
