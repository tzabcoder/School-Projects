#include <string.h>	  //strcpy
#include <stdio.h>        //Input / Output
#include <stdlib.h>	  //Handles standard library
#include <errno.h>        //Error handling on system calls
#include <sys/types.h>    //System defined types
#include <sys/ipc.h>	  //IPC Permission
#include <sys/shm.h>	  //Shared memory requests

//Program Definitions
#define SHMKEY 0x77000 + 01  //Shared memory key
#define STDAMNT 30           //Indexed at 0, there are 30 users, including Dr. Chen

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

//Program Functions
//Calculate the Average GPA
double average_gpa(double gpas[]){
    double total_gpa = 0;
    double avg_gpa;
    int i = 0;

    for (i = 0; i < STDAMNT; i++) {
        total_gpa += gpas[i];
    }

    avg_gpa = total_gpa / STDAMNT;

	return avg_gpa;
}

//Calculate the Average Age
double average_age(int ages[]) {
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
                printf("\nError: shmget() failed... errno=%i\n", errno);
                perror("shmget()\n");
                exit(-1);
    }
	else {
        printf("shmget() passed... shm_id=%i\n", shm_id);
    }

    //Attempt to attatch variable z to shared memory segment
    z = (struct sharedmem_struct*) shmat(shm_id, 0, SHM_RDONLY);
    if ((int)z == -1) {
            printf("\nError: shmat() failed... errno=%i\n", errno);
            perror("shmat()\n");
            exit(-1);
    }
	//Else Successful

    for (i = 0; i < STDAMNT; i++) {
        gpa_data[i] = z->students[i].gpa;
        age_data[i] = z->students[i].age;
    }

    double avg_gpa = average_gpa(gpa_data);
    double avg_age = average_age(age_data);

    printf("_____ Class Data _____\n");
    printf("Average GPA: %lf\n", avg_gpa);
    printf("Average Age: %lf\n\n", avg_age);

    printf("_____ User Information _____\n");
    printf("Active Users: \n");
    for (i = 0; i < STDAMNT; i++) {
        printf("%s %s\n", z->students[i].firstname, z->students[i].lastname);
    }
    
    //Detatch Variable from shared memory
    shmdt((void*) z);

    return 0;
}