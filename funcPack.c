//
// Created by misakisin on 14.03.24.
//

#include "funcPack.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>

#define SHM_SIZE 1024

pid_t create_process() {
    pid_t pid;
    int status;

    pid = fork();
    if(pid < 0) {
        fprintf(stderr, "Помилка виконання виклику fork()");
        return -1;
    }

    if (pid == 0) {
        printf("Дочірній процес PID: %d\n", getpid());
        exit(0);
    } else {
        printf("Батьківський процес PID: %d\n", getpid());
        wait(&status);
        if(WIFEXITED(status)) {
            printf("Дочірній процес завершився з кодом: %d\n", WEXITSTATUS(status));
        } else {
            printf("Дочірній процес завершився аномально\n");
        }
    }

    return pid;
}

void exec_look(){
    execl("/bin/ls", "ls", "-l", NULL);
    //
    char *args[] = {"ls","-l",NULL};
    execv("/bin/ls", args);
    //
    execlp("ls","ls","-l",NULL);
    //
    execvp("ls", args);
}

pid_t shared_memory(){
    pid_t pid;
    int shm_id;
    key_t key = 1234;
    char *shm_ptr;

    //creating shared memory
    if((shm_id = shmget(key,SHM_SIZE, IPC_CREAT | 0666))< 0){
        perror("Помилка створення спільної пам'яті");
        exit(1);
    }
    //connecting to shared memory
    if((shm_ptr = shmat(shm_id, NULL,0)) == (char*)-1){
        perror("Помилка під'єднання");
        exit(1);
    }

    pid = fork();
    if (pid < 0) {
        perror("Помилка виконання виклику fork()");
        exit(1);
    } else if (pid == 0) {
        sprintf(shm_ptr,"Я дочірній процес");
        shmdt(shm_ptr);
        exit(0);
    } else {
        wait(NULL);
        printf("Батьківський процес отримав наступне повідомлення: %s\n", shm_ptr);
        shmdt(shm_ptr);
        shmctl(shm_id, IPC_RMID, NULL);
    }
    return pid;
}