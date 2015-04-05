#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>

typedef struct				//Share Memory struct
{
	double random;
} ShmStru;

int p(int semid, int semno)	//P Operation
{
	struct sembuf buf;
	memset(&buf, 0x00, sizeof(struct sembuf));
	buf.sem_num = semno;
	buf.sem_op = -1;
	buf.sem_flg = 0;
	return semop(semid, &buf, 1);
}

int v(int semid, int semno)	//V Operation
{
	struct sembuf buf;
	memset(&buf, 0x00, sizeof(struct sembuf));
	buf.sem_num = semno;
	buf.sem_op = 1;
	buf.sem_flg = 0;
	return semop(semid, &buf, 1);
}

double randfunc()			//Make random number
{
	srand(time(NULL));
	return (double)(rand() * 0.01);
}

int main(int argc, char *argv[])
{
	int n, pid;
	key_t key;				//Key that the function ftok gets
	int semid, shmid;		//Share memory ID and Semaphore ID
	int semarray[2];		//Set semaphore values
	ShmStru *pstru;			//Share memory struct
	//Get key value by ftok
	if((key = ftok("/etc/profile", 0)) < 0)
	{
		perror("ftok");
		exit(1);
	}
	//Get sem ID by semget
	if((semid = semget(key, 2, IPC_CREAT | IPC_EXCL | 0666)) < 0)
	{
		perror("semget");
		exit(2);
	}
	//Get share memory id by shmget
	if((shmid = shmget(key, sizeof(ShmStru), IPC_CREAT | IPC_EXCL | 0666)) < 0)
	{
		perror("shmget");
		exit(3);
	}
	if((pstru = shmat(shmid, NULL, 0)) == (void *)-1)
	{
		perror("shmat");
		exit(4);
	}
	semarray[0] = 1;
	semarray[1] = 0;
	if(semctl(semid, 2, SETALL, semarray) < 0)
	{
		perror("semctl");
		exit(5);
	}
	pstru->random = 0.00;
	if((pid = fork()) == 0)
	{
		for(; ;)
		{
			p(semid, 1);
			printf("%.2f\n", pstru->random);
			v(semid, 0);
		}
	}
	else
	{
		for(n = 0; n < 10; n++)
		{
			p(semid, 0);
			pstru->random = randfunc();
			v(semid, 1);
			sleep(1);
		}
		kill(pid, SIGKILL);
	}
	if(shmdt(pstru) == -1)
	{
		perror("shmdt");
		exit(4);
	}
	shmctl(shmid, IPC_RMID, 0);
	semctl(shmid, IPC_RMID, 0);
	return 0;
}
