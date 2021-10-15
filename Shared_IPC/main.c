#include <stdio.h>       //Input / Output
#include <stdlib.h>      //Handles standard library 
#include <errno.h>       //Error handling on system calls
#include <sys/types.h>   //System defined types
#include <sys/ipc.h>     //IPC permission structure  
#include <sys/shm.h>     //Shared memory requests

#define SHMKEY 0x77000 + 01   //Shared Memory Key
#define STDAMNT 30            //Indexed @ 0, there are 30 users (29 students and Dr. Chen)

typedef struct{
    char id[20];
    char firstname[30];
    char lastname[30];
    int age;
    double gpa;
    int curactive;

    char lastlogged[20];   //Format - YYYY:MM:DD:HH:MM:SS
    char tottime[15];      //Format - DDD:HH:MM:SS

} stud;

struct sharedmem_struct {
    stud students[STDAMNT];
};

int main() {

    int shm_id, i;    
    struct sharedmem_struct *z;       //z points to shared memory array

    //Attempt to get the pre-existing shared memory
    shm_id = shmget(SHMKEY, 0, 0);    //Get pre-existing shared memory
    if(shm_id == -1) {                //Check return value
        printf("\nError: shmget() failed... errno=%i\n", errno);
        perror("shmget()\n");
        exit(-1);
    }
    else {
        printf("shmget() was sucsessful... shm_id=%i\n", shm_id);
    }

    //Attempt to attatch variable z to the shared memory segment 
    z = (struct sm_struct*) shmat(shm_id, 0, SHM_RDONLY);   
    if((int)z == -1) {
        printf("\nError: shmat() failed... errno=%i\n", errno);
        perror("shmat()");
        exit(-1);
    }
    //Else sucessfull

    //Access data from shared memory region
    printf("Ready to read data from shared memory:\n");
    //Reading data from memory (using z)
    printf(z);

    putchar('\n');

    //Detatch from shared memory
    shmdt((void*) z);

    return 0;
}
