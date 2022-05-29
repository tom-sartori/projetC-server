
/**
 * Malloc a struct Command and set attributes.
 *
 * @param name
 * @param regex
 * @param description
 * @return
 */
Command *createCommand (char *name, char *regex, char *description) {
    Command *command = (Command *)malloc(sizeof(Command));

    command->name = name;
    command->regex = regex;
    command->description = description;

    return command;
}

/**
 * Initialize the global list of available commands, commandList.
 */
void initCommandList () {
    commandList[0] = createCommand("help", "^/help *\n$", "Indique la liste des commandes disponibles. \n");
    commandList[1] = createCommand("disconnect", "^/disconnect *\n$", "Se déconnecter. \n");
    commandList[2] = createCommand("users", "^/users *\n$", "Affiche la liste des utilisateurs connectés. \n");
    commandList[3] = createCommand("mp", "^/mp +([^ ]+) +([^ ]{1}.*)\n$", "Envoie un message privé à un utilisateur. /mp username message \n");
    commandList[4] = createCommand("file", "^/file +([^ ]+) +([^ ]+) *\n$", "Mp, envoie ou recoit un fichier placé dans le dossier 'uploads'. /file (username || -send || -get) filename \n");
    commandList[5] = createCommand("files", "^/files +-(c|s) *\n$", "Liste des fichier disponibles dans le répertoire client ou serveur. /files -c || -s \n");
    commandList[6] = createCommand("kick", "^/kick +([^ ]+) *\n$", "Déconnecte un l'utilisateur en paramètre. /kick username \n");
    commandList[7] = createCommand("rename", "^/rename +([^ ]+) *\n$", "Se renommer. /rename username \n");
    commandList[8] = createCommand("channel", "^/channel *\n$", "Indique le channel actuel dans lequel est l'utilisateur. \n");
    commandList[9] = createCommand("channels", "^/channels *\n$", "Liste les channels accessibles. \n");
    commandList[10] = createCommand("join", "^/join +([0-9]{1,2})", "Rejoint le channel spécifié. \n");
    commandList[11] = createCommand("all", "^/all +([^ ]{1}.*)\n$", "Envoi du message à tous les clients de tous les channels. \n");
    commandList[12] = createCommand("disco", "^/disco *\n$", "Active le mode disco. \n");
    commandList[13] = createCommand("pewpew", "^/pewpew *\n$", "Pew pew! \n");
    // Don't forger to update NB_COMMAND in global.c.
}

/**
 * Return true if the message in params contains a command (starts with a "/").
 * @param message
 * @return
 */
int isCommand (char *message) {
    return isMatch(message, "^/");
}

/**
 * Return the Command associated to the message in params.
 *
 * @param message
 * @return
 */
Command *getCommand (char *message) {
    for (int i = 0; i < NB_COMMAND; i++) {

        if (isMatch(message, commandList[i]->regex)) {
            printf("Command : %s. \n", commandList[i]->name);
            return commandList[i];
        }
    }
    return NULL;
}

