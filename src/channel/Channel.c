#include "conf.h"

/**
 * Malloc and create a channel.
 * Launch socket.
 *
 * @param name
 * @param port
 * @param isPublic
 * @return
 */
Channel *createChannel (char *name, int port, int isPublic, int size) {
    Channel *channel = (Channel *)malloc(sizeof(Channel));

    channel->name = name;   /// TODO : Check unique name.
    channel->port = port;
    channel->serverSocketDescriptor = launchServer(port);
    channel->isPublic = isPublic;
    channel->size = size;
    channel->clientList = createList();

    return channel;
}


void initChannelList () {
    channelList[0] = createChannel(CHANNEL_FILE_NAME, CHANNEL_FILE_PORT, 0, NB_MAX_CLIENT);
    channelList[1] = createChannel(CHANNEL_SWITCH_NAME, CHANNEL_SWITCH_PORT, 0, NB_MAX_CLIENT);
    channelList[2] = createChannel(CHANNEL_DEFAULT_NAME, CHANNEL_DEFAULT_PORT, 0, NB_MAX_CLIENT);
    channelList[3] = createChannel(CHANNEL_1_NAME, CHANNEL_1_PORT, 1, NB_MAX_CLIENT);
    channelList[4] = createChannel(CHANNEL_2_NAME, CHANNEL_2_PORT, 1, DEFAULT_CHANNEL_SIZE);
    channelList[5] = createChannel(CHANNEL_3_NAME, CHANNEL_3_PORT, 1, DEFAULT_CHANNEL_SIZE);
    // Don't forget to update NB_CHANNEL and create at the new index.
}

int isFull (Channel *channel) {
    return channel->size <= channel->clientList->size;
}
