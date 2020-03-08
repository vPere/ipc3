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
	psem1 = sem_open(name, O_CREAT, 1, 1);
        if (psem1 == SEM_FAILED) {
                err_sys("Open psem1");
        }
	int result;
        int sem_value;
        result = sem_getvalue(psem1, &sem_value);
        if (result < 0) {
        err_sys("Read psem1");
        }

	/*while (sem_value > 0) {
		sem_wait(psem1);
		sem_value--;
	}*/
	int i = 0;
	int rep = 1;
	char msg[255];

	while (1) {
		sem_wait(psem1);
		printf("P1: Enter the number of iterations (0-9): \n");
		scanf("%d", &rep);
		if(rep!=0){
			printf("P1: Enter the word to be written: \n");
			scanf("%s", msg);
			strcat(msg, "\n");
		} else {
			break;
		}
		while(i<rep){
			task("log.txt", msg);
			printf("done\n");
			 // Read and print semaphore value
        		result = sem_getvalue(psem1, &sem_value);
        		if (result < 0) {
        			err_sys("Read psem1");
        		}
        		printf("PROCESS 1(PSEM1): %d\n", sem_value);
			i++;
		}
		//provar sem_post aqui
		i = 0;
		sem_post(psem1);
	}


        // Read and print semaphore value
        result = sem_getvalue(psem1, &sem_value);
        if (result < 0) {
        err_sys("Read psem1");
        }
        printf("PROCESS 1(PSEM1): %d\n", sem_value);
	return(0);
}
