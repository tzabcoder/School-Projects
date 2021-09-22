/*
*   CET 440 - Lab 3 
*   Trevor Zabilowicz
* 
*   main.c
*
*   Description:
*       Entry point of the program. Starts by calling the GenerateCipher()
*       function to randomly generate the cipher used for encryption. After
*       generating the cipher, main() prompts for a user inputted message.
*       Then Encrypt() is called to encrypt the message using the randomly 
*       generated cipher.
*/

#include <stdio.h>
#include <stdlib.h>
#include "Cipher.h"

int main() {
    char inp_buff[BUFF_SIZE];

    //Create the cipher
    //FUNCTION -> defined in Cipher.h, initialized in Cipher.c
    char* cipher = GenerateCipher();

    //User input message
    printf("Enter a message to encrypt: ");
    fgets(inp_buff, BUFF_SIZE, stdin);

    //Encrypt the message
    //FUNCTION -> defined in Cipher.h, initialized in Cipher.c
    char* encrypted_msg = Encrypt(inp_buff, cipher);

    //Display encrypted message and the cipher
    //FUNCTION -> defined in Cipher.h, initialized in Cipher.c
    Display(encrypted_msg, cipher);

    //Free memory used by malloc() arrays
    free(cipher);
    free(encrypted_msg);

    return 0;
}
