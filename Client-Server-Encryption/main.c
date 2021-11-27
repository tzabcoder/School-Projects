#include <stdio.h>
#include <string.h>

int validate_password(char password[]) {

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

int main() {

    char password[25];
    int valid_flag = 1;

    printf("_____ Client-Server Connection _____\n");
    printf("Password Requirements: \n");
    printf("1.) At least 8 chracters\n");
    printf("2.) At least 1 upper case character\n");
    printf("3.) At least 1 lower case character\n");
    printf("4.) At least 1 number\n");
    printf("5.) At least 1 special character (!@#$^&*-_)\n\n");

    while (valid_flag != 0) {
        printf("Enter a password: ");
        scanf("%s", &password);

        valid_flag = validate_password(password);

        //8 Characters
        if (valid_flag == 1) {
            printf("Invalid password - needs to be 8 characters long...\n");
        }
        //Upper Case
        if (valid_flag == 2) {
            printf("Invalid password - must be at least 1 upper case character...\n");
        }
        //Lower case
        if (valid_flag == 3) {
            printf("Invalid password - must be at least 1 lower case character...\n");
        }
        //Number
        if (valid_flag == 4) {
            printf("Invalid password - must be at least 1 number...\n");
        }
        //Special character
        if (valid_flag == 5) {
            printf("Invalid password - must be at least 1 special character...\n");
        }
    }

    //Encrypt the password using the substitution cipher and the KEY FILE
    //Then connect to the server

    return 0;
}
