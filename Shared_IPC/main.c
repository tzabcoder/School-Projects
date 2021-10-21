/* --------------------------------------------------------------------------------
Trevor Zabilowicz

CET 440 - Computer Networking 
Server/Client Project - Phase 1
10/21/2021

DESCRIPTION:
This program access shared memory through the IPC (interprocess communication protocol) to
retrieve data stored in memory. The data structure array is read from memory using the struct 
data type. 
The average GPA, average age, current active users, and previous login statistics are parsed from
the data structure and printed in a neat format
-------------------------------------------------------------------------------- */

#include <string.h>	      //strcpy
#include <stdio.h>        //Input / Output
#include <stdlib.h>	      //Handles standard library
#include <errno.h>        //Error handling on system calls
#include <sys/types.h>    //System defined types
#include <sys/ipc.h>	  //IPC Permission
#include <sys/shm.h>	  //Shared memory requests
#include <sys/time.h>     //System time library
#include <unistd.h>
#include <time.h>         //Time functions

//Program Global Variable Definitions
#define SHMKEY 0x77000 + 01    //Shared memory key
#define STDAMNT 30             //Indexed at 0, there are 30 users, including Dr. Chen

//Data Structure
typedef struct {
        char id[20];
        char firstname[30];
        char lastname[30];
        int age;
        double gpa;
        int curactive;

        char lastlogged[20];     //Format - YYYY:MM:DD:HH:MM:SS
        char tottime[15];        //Format - DDD:HH:MM:SS
} stud;

struct sharedmem_struct {
        stud students[STDAMNT];
};

/*Each index corresponds to the month
JAN = 0
FEB = 1
MAR = 2
APR = 3
MAY = 4
JUN = 5
JUL = 6
AUG = 7
SEP = 8
OCT = 9
NOV = 10
DEC = 11
*/
const int MONTH_DAYS[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

//Program Functions
//FUNCTION: average_gpa
double average_gpa(double gpas[]){
    /*
    DESCRIPTION:
        - Accepts a double array of all student's GPAs
        - Sums all of the elements in the array
        - Divides the total by the number of students in
        the class (calculate average)
    PARAMETERS:
        - double gpas[] (array)
    RETURNS:    
        - double avg_gpa (average gpa)
    */

    double total_gpa = 0;
    double avg_gpa;
    int i = 0;

    for (i = 0; i < STDAMNT; i++) {
        total_gpa += gpas[i];
    }

    avg_gpa = total_gpa / STDAMNT;

	return avg_gpa;
}

//FUNCTION: average_age
double average_age(int ages[]) {
    /*
    DESCRIPTION:
        - Accepts an int array of all student's ages
        - Sums all of the elements in the array
        - Divides the total by the number of students in
        the class (calculate average)
    PARAMETERS:
        - int ages[] (array)
    RETURNS:    
        - double avg_age (average age)
    */

    int total_ages = 0;
    double avg_age;
    int i = 0;

    for (i = 0; i < STDAMNT; i++) {
        total_ages += ages[i];
    }

    avg_age = total_ages / STDAMNT;

    return avg_age;
}

int main () {

    //Data Structures for holding user information 
    double gpa_data[STDAMNT];
    int age_data[STDAMNT];

	int shm_id, i;
    struct sharedmem_struct *z  ;            //*z points to the shared memory array
    struct sharedmem_struct *student_data;   //Student information

    //Attempt to get the pre-existing shared memory
    shm_id = shmget(SHMKEY, 0, 0);    //Get pre-existing shared memory
    if (shm_id == -1) {               //Check return value
        //Display failure message
        printf("\nError: shmget() failed... errno=%i\n", errno);
        perror("shmget()\n");
        exit(-1);
    }
    //Else Successful

    //Attempt to attatch variable z to shared memory segment
    z = (struct sharedmem_struct*) shmat(shm_id, 0, SHM_RDONLY);
    if ((int)z == -1) {
        //Display failure message
        printf("\nError: shmat() failed... errno=%i\n", errno);
        perror("shmat()\n");
        exit(-1);
    }
	//Else Successful

    //Add GPA and age to their arrays
    for (i = 0; i < STDAMNT; i++) {
        gpa_data[i] = z->students[i].gpa;
        age_data[i] = z->students[i].age;
    }

    //Calculate Averages
    double avg_gpa = average_gpa(gpa_data);
    double avg_age = average_age(age_data);

    //Display average information
    printf("_____ Class Data _____\n");
    printf("Average GPA: %lf\n", avg_gpa);
    printf("Average Age: %lf\n\n", avg_age);

    //Display active user information
    printf("_____ User Information _____\n");
    printf("Active Users: \n\n");
    for (i = 0; i < STDAMNT; i++) {
        if (z->students[i].curactive) {     //If the user is currently active
            printf("%s %s\n", z->students[i].firstname, z->students[i].lastname);
        }
    }

    //Processing date information
    //Counters for user activity
    int m_t1, m_t2;
    int one_day_ago, two_day_ago;
    int one_day_count = 0;
    int two_day_count = 0;
    int three_day_count = 0;
    int login_day;
    int login_month;
    int curr_day;
    char time_buffer[20];
    char day_temp[3];

    //Getting the current date and time information
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    curr_day = (int)tm.tm_mday;
 
    printf("\n_____ LogIn Statistics _____\n");
    for (i = 0; i < STDAMNT; i++) {
        strcpy(time_buffer, z->students[i].lastlogged);

        strncpy(day_temp, time_buffer+8, 2);     //get day from time buffer
        login_day = atoi(day_temp);
        strncpy(day_temp, time_buffer+5, 2);     //get month from time buffer
        login_month = atoi(day_temp);
        //Adjust for array processing (account for index)
        login_month = login_month - 1;

        //Calculate previous days
        one_day_ago = curr_day - 1;      //One day ago
        two_day_ago = curr_day - 2;      //Two days ago

        //Adjust necessary date 
        if (one_day_ago <= 0 ) {         
            m_t1 = login_month - 1;

            if (m_t1 < 0) {     //Roll back to previous month
                m_t1 = 11;
                one_day_ago = MONTH_DAYS[m_t1];
            }
            else {
                one_day_ago = MONTH_DAYS[m_t1];
            }
        } 
        if (two_day_ago <= 0) {
            m_t2 = login_month - 1;

            if (m_t2 < 0) {     //Roll back to previous month
                m_t2 = 11;
                two_day_ago = MONTH_DAYS[m_t2] - 1;
            }
            else {
                two_day_ago = MONTH_DAYS[m_t2] - 1;
            }
        }

        //Check last log in
        if (login_day == curr_day){         //Check today log in
            one_day_count++;
        }
        if (login_day == one_day_ago) {     //Check yesterday login
            two_day_count++;
        }
        if (login_day == two_day_ago) {     //Check two days ago
            three_day_count++;
        }
    }

    //Display info
    printf("Users logged in today: %i\n", one_day_count);
    printf("Users logged in yesterday: %i\n", two_day_count);
    printf("Users loggin in two days ago: %i\n", three_day_count);
    
    //Detatch Variable from shared memory
    shmdt((void*) z);

    return 0;
}