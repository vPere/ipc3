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
    sem_t*  psem1;
    int  sem_value;
    char filename[64] = "log2.txt";
    int result;

    /* Parse program arguments */
    /*if (argc != 2) {
        fprintf(stderr, "Usage: %s <iterations>\n", argv[0]);
        exit(1);
    }*/

    /* Open psem1 */
    psem1 = (sem_t*) sem_open("/sem1", O_CREAT, 0644, 0);
    if (psem1 == SEM_FAILED) {
        err_sys("Open psem1");
    }
    sem_getvalue(psem1, &sem_value);
    while (sem_value > 0) {
        sem_wait(psem1);
        sem_value--;
    }
    /* Create a new process and print */
   	returnedpid = fork();
	if(fork()==0){
		//child process
		
		int rep;
		char msg[255];
		FILE *toWrite = fopen(filename, "a+");
		sem_wait(psem1);
                printf("P1: Enter the number of iterations (0-9): \n");
                scanf("%d", &rep);
                if(rep!=0){
                        printf("P1: Enter the word to be written: \n");
                        scanf("%s", msg);
                        strcat(msg, "\n");
                } else {
			fclose(toWrite);
                        sem_destroy(psem1);
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
		sem_post(psem1);
	} else  {
		//parent process
                int rep;
                char msg[255];
                FILE *toWrite = fopen(filename, "a+");
                sem_wait(psem1);
                printf("P2: Enter the number of iterations (0-9): \n");
                scanf("%d", &rep);
                if(rep!=0){
                        printf("P2: Enter the word to be written: \n");
                        scanf("%s", msg);
                        strcat(msg, "\n");
                } else {
			fclose(toWrite);
                        sem_destroy(psem1);
                        exit(0);
                }
                while(i<rep){
                         fprintf(toWrite, msg);
                         // Read and print semaphore value
                        result = sem_getvalue(psem1, &sem_value);
                        if (result < 0) {
                                err_sys("Read psem1");
                        }
                        printf("PROCESS 2(PSEM1): %d\n", sem_value);
                        i++;
                }
		fclose(toWrite);
                sem_post(psem1);

	}
    /*
    top =atoi(argv[1]);
    for (i=0; i< top;i++) {
     fprintf(stdout, "value: %d\t",i);
    }
    fprintf(stdout, "\n");
    */
    exit(0);
}
