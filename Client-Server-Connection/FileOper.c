/* --------------------------------------------------------------------------------
Trevor Zabilowicz

CET 440 - Computer Networking 
Server/Client Project - Phase 1
11/11/2021

FILE: FileOper.c

DESCRIPTION:
Contains the functions for reading and writing the current time to the LogDate file.
-------------------------------------------------------------------------------- */

//File Includes
#include "FileOper.h"

//FUNCTION: readFile()
char *readFile() {
    /* ----------------------------------------------------
    DESCRIPTION:
        - Opens the LogDate file in read mode
        - Reads the data from the file
        - Closes the file
    PARAMETERS:
        - None
    RETURNS:    
        - Char array of the read data 
    ----------------------------------------------------- */

    fp = fopen(FILENAME, "r");
    fgets(tempBuff, 255, (FILE*)fp);
    fclose(fp);

    return tempBuff;
}

//FUNCTION: writeFile()
void writeFile() {
    /* ----------------------------------------------------
    DESCRIPTION:
        - creates a time structure to get the current date and 
        time of the Client's machine
        - Opens the LogDate file in write mode 
        - Writes the current date and time to the file
        - Closes the file
    PARAMETERS:
        - None
    RETURNS:    
        - None
    ----------------------------------------------------- */

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    //Write the date and time to the file
    fp = fopen(FILENAME, "w");
    fputs(asctime(tm), fp);
    fclose(fp);
}