/* --------------------------------------------------------------------------------
Trevor Zabilowicz

CET 440 - Computer Networking 
Server/Client Project - Phase 1
11/11/2021

FILE: FileOper.h

DESCRIPTION:
Header file that contains the constants and function definitions for reading and 
writing the last login time to a file.
Meant to track the user's login time for future connections.
-------------------------------------------------------------------------------- */

#ifndef FILEOPER_H_
#define FILEOPER_H_

//File Includes
#include <stdio.h>
#include <time.h>

//Constant filename for reading and writing
const static char FILENAME[] = "LogDate.txt";

//Program Variables 
char tempBuff[255];
FILE *fp;

//Function Definitions
char *readFile();        //Read data from LogDate file 
void writeFile();        //Write data to LogDate file

#endif