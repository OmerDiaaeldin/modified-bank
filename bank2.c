#include<stdio.h>
#include<semaphore.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>
#include<stdlib.h>
#define SHM_KEY 12345
#define SHM_SIZE 1024

void update(char *shm, int amount){
    int balance = atoi(shm);
    printf("Old balance: %d\n",balance);
    printf("Bank 2 change: %d\n", amount);
    sprintf(shm,"%d",balance+amount);
    printf("New balance: %d\n",balance+amount);
}

int main(){
    sem_t *s1, *s2;
    s1 = sem_open("/first", O_EXCL);
    s2 = sem_open("/second", O_EXCL);
    
    int shmid;
    int key = SHM_KEY;
    char *shm;
    /* create shared memory segment */
    shmid = shmget(key, SHM_SIZE, 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }
    /* attach shared memory segment */
    shm = shmat(shmid, NULL, 0);
    if (shm == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    int balance = atoi(shm);

    sem_wait(s2);
    update(shm,10);
    sem_post(s1);
}