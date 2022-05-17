
/**
 * Transfer data received from the source to the targeted client.
 * Do not store the file on the server.
 *
 * @param sourceSocket
 * @param targetedSocket
 */
void transferFile (int sourceSocket, int targetedSocket) {
    // Get file blocSize.
    long filelen;
    recv(sourceSocket, &filelen, sizeof(long), 0);
    send(targetedSocket, &filelen, sizeof(long), 0);

    long blocSize = MAX_SIZE_SENT;
    char subBuffer[blocSize];
    for (int i = 0; i < filelen; i += MAX_SIZE_SENT) {
        blocSize = (i + MAX_SIZE_SENT < filelen) ? MAX_SIZE_SENT : filelen - i; // Calculate the bloc's blocSize.
        recv(sourceSocket, subBuffer, blocSize, 0);
        send(targetedSocket, subBuffer, blocSize, 0);
        bzero(subBuffer, MAX_SIZE_SENT);    // Clear the buffer.
    }

    printf("File transferred. \n");
}