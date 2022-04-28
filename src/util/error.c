#include <stdio.h>
#include <errno.h>
#include "color.c"


/**
 * Perror the message in params and exit the programme.
 * If hasErrno, print it.
 * @param errorMessage
 * @param hasErrno
 */
void throwError (char *errorMessage, int hasErrno) {
    if (hasErrno) {
        perror(errorMessage);
    }else{
        setRedText();
        printf("%s \n", errorMessage);
        setWhiteText();
    }
    exit(EXIT_FAILURE);
}