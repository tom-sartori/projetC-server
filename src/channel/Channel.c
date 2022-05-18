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
Channel *createChannel (char *name, int port, int isPublic) {
    Channel *channel = (Channel *)malloc(sizeof(Channel));

    channel->name = name;   /// TODO : Check unique name.
    channel->serverSocketDescriptor = launchServer(port);
    channel->isPublic = isPublic;

    return channel;
}


void initChannelList () {
    channelList[0] = createChannel(CHANNEL_FILE_NAME, CHANNEL_FILE_PORT, 0);
    channelList[1] = createChannel(CHANNEL_1_NAME, CHANNEL_1_PORT, 1);
    channelList[2] = createChannel(CHANNEL_2_NAME, CHANNEL_2_PORT, 1);
    // Don't forget to update NB_CHANNEL and create at the new index.
}
