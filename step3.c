#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

void err_sys(char *mess) { perror(mess); exit(1); }

int main(int argc, char *argv[]) {
    int pid, ppid;
    int i, top;
    int returnedpid;
    sem_t *psem1;
    sem_t *psem2;
    int  sem_value;
    char filename[64] = "log2.txt";
    int result;

    /* Open psem1 */
    psem1 = (sem_t*) sem_open("/sem3", O_CREAT, 0644, 1);
    if (psem1 == SEM_FAILED) {
        err_sys("Open psem1");
    }
    psem2 = (sem_t*) sem_open("/sem4", O_CREAT, 0644, 0);
    if (psem1 == SEM_FAILED) {
        err_sys("Open psem2");
    }
	
    /* Create a new process and print */
        returnedpid = fork();
        if(fork()==0){
                //child process

                int rep;
                char msg[255];
                while(1) {
                        FILE *toWrite = fopen(filename, "a+");
                        sem_wait(psem1);
                        sem_getvalue(psem2, &sem_value);
                        if (sem_value==1){
                                fclose(toWrite);
                                sem_destroy(psem1);
                                sem_destroy(psem2);
                                exit(0);
                        }
                        printf("P1: Enter the number of iterations (0-9): \n");
                        scanf("%d", &rep);
                        if(rep!=0){
                                printf("P1: Enter the word to be written: \n");
                                scanf("%s", msg);
                                strcat(msg, "\n");
                        } else {
                                fclose(toWrite);
                                sem_post(psem2);
                                exit(0);
                        }
                        while(i<rep){
                                 fprintf(toWrite, msg);
                                 i++;
                        }
                        result = sem_getvalue(psem1, &sem_value);
                        if (result < 0) {
                            err_sys("Read psem1");
                        }
                        printf("PROCESS 1(PSEM1): %d\n", sem_value);
                        fclose(toWrite);
                        sem_post(psem2);
                }
                } else  {
                //parent process
                int rep;
                char msg[255];
                FILE *toWrite = fopen(filename, "a+");
                while(1){
                        sem_wait(psem2);
                        sem_getvalue(psem1, &sem_value);
                        if (sem_value==1){
                                fclose(toWrite);
                                sem_destroy(psem1);
                                sem_destroy(psem2);
                                exit(0);
                        }

                        printf("P2: Enter the number of iterations (0-9): \n");
                        scanf("%d", &rep);
                        if(rep!=0){
                                printf("P2: Enter the word to be written: \n");
                                scanf("%s", msg);
                                strcat(msg, "\n");
                        } else {
                                fclose(toWrite);
                                sem_destroy(psem2);
                                sem_post(psem2);
                                exit(0);
                                }
                        while(i<rep){
                                fprintf(toWrite, msg);
                                i++;
                        }
                        fclose(toWrite);
                        sem_post(psem1);
                         result = sem_getvalue(psem1, &sem_value);
                         if (result < 0) {
                                 err_sys("Read psem1");
                         }
                                 printf("PROCESS 2(PSEM1): %d\n", sem_value);

                }
        }

    exit(0);
}
