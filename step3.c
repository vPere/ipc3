#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

sem_t sem;
char filename[64] = "log2.txt";
int b = 1;
int tn;

void* thread(void* arg)
{
	//wait
	sem_wait(&sem);
	printf("\nEntered..\n");

	//critical section
	FILE *f;
	f = fopen(filename, "a");
	char word[64];
	int rep;
        printf("%i %s", tn, "Write a number bigger than 0 (or 0 to exit): ");
       	scanf("%i", &rep);
       	if(rep==0){
       		fclose(f);
	        printf("%s", "Finnishing execution...\n");
		b = 0;
		sem_post(&sem);
		return(0);
	}
        printf("%i %s", tn, "Write a word (max 64 char): ");
        scanf("%s", &word);
	while(rep>0){
		fputs(strcat(word,"\n"), f);
		rep--;
	}
	printf("%s %i %s", "\nJust Exiting: ", tn, "\n");
        sem_post(&sem);
}


int main()
{
	FILE *toWrite = fopen(filename, "w");
	fclose(toWrite);
	sem_init(&sem, 0, 1);
	pthread_t t1,t2;
	while(b == 1){
		tn = 1;
		pthread_create(&t1,NULL,thread,NULL);
		if (b==0) break;
		tn = 2;
		pthread_create(&t2,NULL,thread,NULL);
		if (b==0) break;
	}
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	sem_destroy(&sem);
	return 0;
}

