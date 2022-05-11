#include <regex.h>

/**
 * Return true if the message match with the regex in params.
 *
 * @param message
 * @param reg
 * @return
 */
int isMatch (char *message, char *reg) {
    regex_t regex;
    int regexValue;
    int isMatch = 0;

    regexValue = regcomp(&regex, reg, REG_EXTENDED);
    if (regexValue) {
        throwError("Could not compile regex. \n", 0);
    }

    //Execute regular expression.
    regexValue = regexec(&regex, message, 0, NULL, 0);

    if (!regexValue) {
//        printf("Regex match. \n");
        isMatch = 1;
    }
    else if (regexValue == REG_NOMATCH) {
//        printf("Regex no match. \n");
        isMatch = 0;
    }
    else {
        throwError("Regex error. \n", 0);
    }

    // Free memory allocated to the pattern buffer by regcomp().
    regfree(&regex);
    return isMatch;
}

/**
 * Match the message with the regex and set groups gotten in the list.
 *
 * @param list : size 3.
 * @param message
 * @param regexString
 */
void getRegexGroup (char *list[], char *message, char *regexString) {
    regex_t regexCompiled;

    size_t maxGroups = 3;
    regmatch_t groupArray[maxGroups];

    if (regcomp(&regexCompiled, regexString, REG_EXTENDED)){
        throwError("Could not compile regex. \n", 0);
    }

    if (regexec(&regexCompiled, message, maxGroups, groupArray, 0) == 0) {
        for (int i = 0; i < maxGroups; i++) {
            if (groupArray[i].rm_so == (size_t)-1) {
                break;  // No more groups
            }

            char sourceCopy[strlen(message) + 1];
            strcpy(sourceCopy, message);
            sourceCopy[groupArray[i].rm_eo] = 0;

            list[i] = (char *)malloc(strlen(sourceCopy + groupArray[i].rm_so));
            strcpy(list[i], sourceCopy + groupArray[i].rm_so);
        }
    }
    regfree(&regexCompiled);
}

