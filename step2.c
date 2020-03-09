#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<semaphore.h>
#include<unistd.h>
#include<errno.h>
#include<pthread.h>
sem_t *psem1;
sem_t *psem2;
void err_sys(const char* x)
{
    perror(x);
    exit(1);
}

const char *name = "semafor";

/* void startSem() {
        // Open psem1

        sem_t *psem1 = (sem_t*) sem_open(name, O_CREAT);
        if (psem1 == SEM_FAILED) {
                err_sys("Open psem1");
        }

} */
int task (char filename[], char msg[]) {
        FILE *toWrite;
        toWrite = fopen(filename, "a");
        if(toWrite == NULL) {
                perror("Error opening file");
                return(-1);
        }
        fputs(msg, toWrite);
        fclose(toWrite);
        return(0);
}

int main(int argc, char *argv[]) {
	psem1 = sem_open("/sem1", O_CREAT, 1);
        if (psem1 == SEM_FAILED) {
                err_sys("Open psem1");
        }
        psem2 = sem_open("/sem2", O_CREAT, 1);
        if (psem2 == SEM_FAILED) {
                err_sys("Open psem2");
        }
	int result;
        int sem_value;
        result = sem_getvalue(psem2, &sem_value);
        if (result < 0) {
        err_sys("Read psem2");
        }

	while (sem_value > 0) {
		sem_wait(psem2);
		sem_value--;
	}

	int i = 0;
	int rep = 1;
	char msg[255];

	while (1) {
		sem_wait(psem2);
		printf("P2: Enter the number of iterations (0-9): \n");
		scanf("%d", &rep);
		if(rep!=0){
			printf("P2: Enter the word to be written: \n");
			scanf("%s", msg);
			strcat(msg, "\n");
		} else {
			sem_destroy(psem1);
        		sem_destroy(psem2);
		        exit(0);
		}
		while(i<rep){
			task("log.txt", msg);
			printf("done\n");
			i++;
		}
		 // Read and print semaphore value
                        result = sem_getvalue(psem2, &sem_value);
                        if (result < 0) {
                                err_sys("Read psem2");
                        }
                        printf("PROCESS 2(PSEM2): %d\n", sem_value);

		//provar sem_post aqui
		i = 0;
		sem_post(psem1);
		//sem_wait(psem2);
	}
	sem_destroy(psem1);
	sem_destroy(psem2);
	exit(0);

        // Read and print semaphore value
        /*result = sem_getvalue(psem1, &sem_value);
        if (result < 0) {
        err_sys("Read psem1");
        }
        printf("PROCESS 1(PSEM1): %d\n", sem_value);
	return(0);
	*/
}
