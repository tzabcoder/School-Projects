/*
*   CET 440 - Lab 3 
*   Trevor Zabilowicz
* 
*   Cipher.c
*
*   Description:
*       Contains the Encrypt(), GenerateCipher() and Display() functions
*       used by the main program.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "Cipher.h" 

//Contains the ASCII chars 
const char ASCII_chars[94] = {
'!', '\"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/',
'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?', '@',
'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^', '_', '`', 'a', 'b', 
'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's',
't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~'
};

/*
*   FUNCTION: char* Encrypt(char*, char*)
* 
*   PARAMETERS: 
*       char* buffer - user inputted message buffer 
*       char* cipher - randomly generated cipher
* 
*   DESCRIPTION:
*       Gets the length of the user inputted message. Creates a dynamic
*       char array with buffer length. Gets each character in the buffer 
*       and matches that char to the ASCII_chars array. If the chars 
*       are equal, the corresponding cipher position is inputted into the 
*       new encrypted message.
* 
*   RETURNS:
*       char* msg - encrypted message
*/
char* Encrypt(char* buffer, char* cipher) {
    int i, j;
    char c;
    int buff_len = strlen(buffer); //Get length of message 
    char* msg = malloc(buff_len);

    for(i = 0; i < buff_len; i++) {
        c = buffer[i];

        if(c == ' ') {
            msg[i] = ' ';
        }
        else {
            for(j = 0; j < MAX_ASCII_LEN; j++) {
                //Input the encrypted char into the new message
                if(c == ASCII_chars[j]) {
                    msg[i] = cipher[j];
                    break;
                }
            }
        }
    }

    return msg;
}   

/*
*   FUNCTION: char* GenerateCipher()
* 
*   PARAMETERS: 
*       NONE
* 
*   DESCRIPTION:
*       Function generates a random cipher and scrambles the original
*       ASCII_chars array. Generates random number and if that position
*       is not filled in the new cipher, in the random position of the
*       new cipher, put the looped char of the original ASCII_chars
*       array.
* 
*   RETURNS:
*       char* cipher - new randomly generated cipher
*/
char* GenerateCipher() {
    int i = 0;
    int flag;
    int random;

    //Initialize the Cipher
    char* cipher = malloc(MAX_ASCII_LEN); 
    for(i = 0; i < MAX_ASCII_LEN; i++) {
        cipher[i] = ' ';
    }

    srand(time(NULL));

    for(i = 0; i < MAX_ASCII_LEN; i++) {
        flag = 1;

        while(flag == 1) {
            random = rand() % 94;

            if(cipher[random] == ' ') {
                cipher[random] = ASCII_chars[i];
                flag = 0;
            }
            else {
                flag = 1;
            }
        }
    }

    return cipher;
}

/*
*   FUNCTION: void Display(char*, char*)
* 
*   PARAMETERS: 
*       char* encrypted_mag - encrypted message
*       char* cipher - randomly generated cipher
* 
*   DESCRIPTION:
*       Displays the encrypted message along with the original ASCII_chars
*       and the corresponding positions of the randomly generated cipher
* 
*   RETURNS:
*       NONE 
*/
void Display(char* encrypted_msg, char* cipher) {
    int i;

    printf("Encrypted Message: %s", encrypted_msg);
    printf("\n");
    for(i = 0; i < MAX_ASCII_LEN; i++) {
        printf("%c=%c ", ASCII_chars[i], cipher[i]);
    }
}