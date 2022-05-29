#include "Command.c"
#include "action.c"

/**
 * Find the wanted command in the message in params (with regex).
 * Call the action attached to this command.
 *
 * @param client
 * @param message
 */
void doCommandAction (Client *client, char *message) {
    Command *command = getCommand(message);

    if (command == NULL) {
        // Unknown action.
        unknownAction(client);
    }
    else if (strcmp("help", command->name) == 0) {
        // Help.
        helpAction(client->acceptedSocketDescriptor);
    }
    else if (strcmp("disconnect", command->name) == 0) {
        // Disconnection.
        disconnectAction(client);
    }
    else if (strcmp("users", command->name) == 0) {
        // List of users.
        usersAction(client);
    }
    else if (strcmp("mp", command->name) == 0) {
        // Private message.
        mpAction(client, command, message);
    }
    else if (strcmp("file", command->name) == 0) {
        // File sending.
        fileAction(command, message);
    }
    else if (strcmp("files", command->name) == 0) {
        // Files listing.
        filesAction(client);
    }
    else if (strcmp("kick", command->name) == 0) {
        // Kick someone.
        kickAction(client, command, message);
    }
    else if (strcmp("rename", command->name) == 0) {
        // Rename username.
        renameAction(client, command, message);
    }
    else if (strcmp("channel", command->name) == 0) {
        // Current user channel.
        channelAction(client);
    }
    else if (strcmp("channels", command->name) == 0) {
        // List of channels.
        channelsAction(client);
    }
    else if (strcmp("join", command->name) == 0) {
        // Join a channel.
        joinAction(client, command, message);
    }
    else if (strcmp("all", command->name) == 0) {
        // Broadcast message to every clients.
        allAction(client, command, message);
    }
    else if (strcmp("disco", command->name) == 0) {
        // Do nothing, it's handled client-side
    }
    else if (strcmp("pewpew", command->name) == 0) {
        // Do nothing, it's handled client-side
    }
    else {
        // Unknown action.
        unknownAction(client);
    }
}