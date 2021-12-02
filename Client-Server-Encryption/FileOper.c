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

//FUNCTION: validate_password()
int validate_password(char password[]) {

    /* ----------------------------------------------------
    DESCRIPTION:
        - Validates whether or not the password meets the 
        specified criteria
    PARAMETERS:
        - char password[]
    RETURNS:    
        - int flag
    ----------------------------------------------------- */

    //password flags 
    int upper_flag = 0;
    int lower_flag = 0;
    int num_flag = 0;
    int special_flag = 0;
    int temp;
    int i = 0;

    //Check if at least 8 characters
    int pass_len = strlen(password);
    
    //Check if lower case 
    for (i = 0; i < pass_len; i++) {
        temp = (int)password[i];
        if (temp >= 97 && temp <= 122) {
            lower_flag = 1;
            break;
        }
        else {
            lower_flag = 0;
        }
    }

    //Check if upper case
    for (i = 0; i < pass_len; i++) {
        temp = (int)password[i];
        if (temp >= 65 && temp <= 90) {
            upper_flag = 1;
            break;
        }
        else {
            upper_flag = 0;
        }
    }

    //Check if numbber
    for (i = 0; i < pass_len; i++) {
        temp = (int)password[i];
        if (temp >= 48 && temp <= 57) {
            num_flag = 1;
            break;
        }
        else {
            num_flag = 0;
        }
    }

    //Check if special character
    for (i = 0; i < pass_len; i++) {
        temp = (int)password[i];
        if (temp == 33||temp == 35||temp == 36||temp == 38||temp == 42||temp == 45||temp == 63||temp == 64||temp == 94||temp == 95) {
            special_flag = 1;
            break;
        }
        else {
            special_flag = 0;
        }
    }

    if (pass_len < 8) {
        return 1;
    }
    if (upper_flag == 0) {
        return 2;
    }
    if (lower_flag == 0) {
        return 3;
    }
    if (num_flag == 0) {
        return 4;
    }
    if (special_flag == 0) {
        return 5;
    }

    return 0;
}

//FUNCTION: encrypt_data()
void encrypt_data(char * ptext, char * ctext)
{
    /* ----------------------------------------------------
    DESCRIPTION:
        - Opens KeyFile and reads permutation
        - Encrypts data passed to function
        - Organizes data via pointers
    PARAMETERS:
        - char *ptext
        - char *ctext
    RETURNS:    
        - None
    ----------------------------------------------------- */

	int i = 0;
    int length;
	int pool[CSIZE];
	int key[CSIZE];
    FILE *fp;

    fp = fopen("KeyFile.txt", "r");
    while (!feof (fp)) {
        fscanf(fp, "%d", &key[i]);
        i += 1;
    }
    fclose(fp);

	length=strlen(ptext);
	
	for (i=0; i<length; i++)
	{
		ctext[i]=key[ptext[i]-ASCIIS]+ASCIIS;  //The first keyboard printable character have an ASCII code 32, defined as ASCIIS.
	}
}

//FUNCTION: decrypt_data()
void decrypt_data(char *ptext, char *ctext) {
    /* ----------------------------------------------------
    DESCRIPTION:
        - Opens KeyFile and reads permutation
        - Decrypts data passed to function
        - Organizes data via pointers
    PARAMETERS:
        - char *ptext
        - char *ctext
    RETURNS:    
        - None
    ----------------------------------------------------- */

    int i = 0;
    int j = 0;
    int length;
    int key[CSIZE];
    int ascii_temp;
    int ascii_add = 0;
    FILE *fp;

    fp = fopen("KeyFile.txt", "r");
    while (!feof (fp)) {
        fscanf(fp, "%d", &key[i]);
        i += 1;
    }
    fclose(fp);

    length = strlen(ptext);

    for (i = 0; i < CSIZE; i++) {
        key[i] = key[i] + 32;
    }

    for (i = 0; i < length; i++) {
        ascii_temp = ptext[i];
        for (j = 0; j < CSIZE; j++) {
            if (ascii_temp == key[j]) {
                ctext[i] = j + ASCIIS;
                break;
            }
        }
    }
}

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