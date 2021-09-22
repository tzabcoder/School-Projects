/*
*   CET 440 - Lab 3 
*   Trevor Zabilowicz
* 
*   Cipher.h
*
*   Description:
*       Header file that contains function declarations and variables 
*       used throughout the program.
*/

#ifndef CIPHER_H_
#define CIPHER_H_

//Constant Definitions
#define BUFF_SIZE 256
#define MAX_ASCII_LEN 94

//Function Definitions
char* Encrypt(char*, char*);
char* GenerateCipher();
void Display(char*, char*);

#endif