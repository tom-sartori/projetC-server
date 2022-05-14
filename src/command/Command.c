struct Command {
    char *name;
    char *regex;
    char *description;
    /// TODO : Add action function associated.
};
typedef struct Command Command;

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

Command *commandList[NB_COMMAND];
/**
 * Initialize the global list of available commands, commandList.
 */
void initCommandList () {
    commandList[0] = createCommand("help", "^/help *\n$", "Indique la liste des commandes disponibles. \n");
    commandList[1] = createCommand("disconnect", "^/disconnect *\n$", "Déconnecte l'utilisateur. \n");
    commandList[2] = createCommand("users", "^/users *\n$", "Affiche la liste des utilisateurs connectés. \n");
    commandList[3] = createCommand("mp", "^/mp +([^ ]+) +([^ ]{1}.*)\n$", "Envoi un message privé. /mp username message \n");
    commandList[4] = createCommand("file", "^/file +([^ ]+) +([^ ]+) *\n$", "Envoi ou recoie un fichier placé dans le dossier 'uploads'. /file -send || -get filename \n");
    commandList[5] = createCommand("files", "^/files +-(c|s) *\n$", "Liste des fichier disponibles dans le répertoire client ou serveur. /files -c || -s \n");
    // Don't forger to update NB_COMMAND.
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

