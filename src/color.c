#include <stdio.h>
/**
 * Set color text to Red, bold or not depending on the function you call
 * Red: \033[0;31m -> color to display errors
 */

void setRedText(){
    printf("\033[0;31m");
}

void setRedTextBold(){
    printf("\033[1;31m");
}

/**
 * Set color text to Green, bold or not depending on the function you call
 * Green \033[0;32m -> used to tell if a message was well sent / well received
 */

void setGreenText(){
    printf("\033[0;32m");
}

void setGreenTextBold(){
    printf("\033[1;32m");
}

/**
 * Set color text to Blue, bold or not depending on the function you call
 * Blue \033[0;34m -> to display the private messages you receive
 */

void setBlueText(){
    printf("\033[0;34m");
}

void setBlueTextBold(){
    printf("\033[1;34m");
}

/**
 * Set color text to Cyan, bold or not depending on the function you call
 * Cyan \033[0;36m -> to display the private messages you send
 */

void setCyanText(){
    printf("\033[0;36m");
}

void setCyanTextBold(){
    printf("\033[1;36m");
}

/**
 * Set color text to White, bold or not depending on the function you call
 * White \033[0;37m -> default color
 */

void setWhiteText(){
    printf("\033[0;37m");
}

void setWhiteTextBold(){
    printf("\033[1;37m");
}

/**
 * Set color text to Black, bold or not depending on the function you call
 * Black \033[0;37m -> default color
 */

void setBlackText(){
    printf("\033[0;30m ");
}

void setBlackTextBold(){
    printf("\033[1;30m ");
}

/**
* Set color text to Purple, bold or not depending on the function you call
* Purple \033[0;35m -> global message you send
*/

void setPurpleText(){
    printf("\033[0;35m");
}

void setPurpleTextBold(){
    printf("\033[1;35m");
}
