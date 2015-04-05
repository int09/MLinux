#include <sys/types.h>	//key_t
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#define	SEMMUTEXNUM		0
#define SEMNUMNUM		2
#define SEMORDERNUM		2

int P(int semid, int semno);
int V(int semid, int semno);

int main(int argc, char *argv[])
{
	typedef struct shm_struct {
		int	buf[5];
		int in;
		int out;
		int array[10];
		int writed;
	} shm_struct;
	
	typedef union semun {
		int					 val;
		struct	semid_ds	*buf;
		unsigned	short	*array;
	} semun;
	key_t	key;
	if ((key = ftok(argv[0], 1)) == -1)
	{
		perror("ftok error");
		exit(-1);
	}
	printf("%d\n", key);
/*Share Memory Struct that I used it to storage the buffer*/
	int shmid;
	if ((shmid = shmget(IPC_PRIVATE, 1024 * 1024, 0666 | IPC_CREAT)) < 0)
	{
		printf("%d\n", shmid);
		perror("shmget shmid error");
		exit(-1);
	}
	struct shm_struct *pshmbuf;
	pshmbuf->in = 0;
	pshmbuf->out = 0;
	int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int index;
	for(index = 0; index < 10; index++)
		pshmbuf->array[index] = arr[index];
	pshmbuf->writed = 0;
	if ((pshmbuf = (shm_struct *)shmat(shmid, 0, 0)) == (void *)-1)
	{
		perror("shmat pshmbuf error");
		exit(-1);
	}


	int semmutex, semnum, semorder;
	if ((semmutex = semget(key, 1, IPC_CREAT | 0666)) < 0)
	{
		perror("semget semmutex error");
		exit(-1);
	}
	if ((semnum = semget(key, 2, IPC_CREAT | 0666)) < 0)
	{
		perror("semget semnum error");
		exit(-1);
	}
	if ((semorder = semget(key, 2, IPC_CREAT | 0666)) < 0)
	{
		perror("semget semorder error");
		exit(-1);
	}
	semun	sumutex;
	sumutex.val = 1;
	semctl(semmutex, SEMMUTEXNUM, SETVAL, sumutex);

	unsigned short semnumarr[] = {0, 5};
	semun	sunum;
	sunum.array = semnumarr;
	semctl(semnum, SEMNUMNUM, SETALL, sunum);

	unsigned short semorderarr[] = {1, 0};
	union semun	suorder;
	suorder.array = semorderarr;
	semctl(semorder, SEMORDERNUM, SETALL, suorder);
	pid_t	producer;
	pid_t	consumer0, consumer1;
	if ((producer = fork()) < 0)
	{
		perror("fork producer error");
		exit(-1);
	}
	if ((consumer0 = fork()) < 0)
	{
		perror("fork consumer0 error");
		exit(-1);
	}
	if ((consumer1 = fork()) < 0)
	{
		perror("fork consumer1 error");
		exit(-1);
	}

	if (producer == 0)
	{
		int i;
		for (i = 0; i < 10; ++i)
		{
			P(semnum, 0);
			P(semmutex, 0);
			//Put the number into the buffer
			pshmbuf->buf[pshmbuf->in] = pshmbuf->array[i];
			printf("put %d in buffer", pshmbuf->buf[pshmbuf->in]);
			pshmbuf->in = (pshmbuf->in + 1) % 5;
			pshmbuf->writed++;
			V(semmutex, 0);
			V(semnum, 1);
		}
	}
	else if (consumer0 == 0)
	{
		int sum = 0, curnum;
		while (pshmbuf->in == pshmbuf->out && pshmbuf->writed == 10)
		{
			P(semorder, 0);
			P(semnum, 0);
			P(semmutex, 0);
			//Get the number from the buffer
			curnum = pshmbuf->buf[pshmbuf->out];
			pshmbuf->out = (pshmbuf->out + 1) % 5;
			printf("now consumer0 get %d\n", curnum);
			sum += curnum;
			V(semmutex, 0);
			V(semnum, 1);
			V(semorder, 1);
		}
		printf("Consumer0 sum is %d\n", sum);
	}
	else if (consumer1 == 0)
	{
		int sum = 0, curnum;
		while (pshmbuf->in == pshmbuf->out && pshmbuf->writed == 10)
		{
			P(semorder, 1);
			P(semnum, 0);
			P(semmutex, 0);
			//Get the number form the buffer
			curnum = pshmbuf->buf[pshmbuf->out];
			pshmbuf->out = (pshmbuf->out + 1) % 5;
			printf("now consumer1 get %d\n", curnum);
			sum += curnum;
			V(semmutex, 0);
			V(semnum, 1);
			V(semorder, 0);
		}
		printf("Consumer1 sum is %d\n", sum);
	}
	shmdt(pshmbuf);
	return 0;
}

int P(int semid, int semno)
{
	struct	sembuf	buf;
	memset(&buf, 0x00, sizeof(struct sembuf));
	buf.sem_num = semno;
	buf.sem_op = -1;
	buf.sem_flg = 0;
	return semop(semid, &buf, 1);
}

int V(int semid, int semno)
{
	struct	sembuf	buf;
	memset(&buf, 0x00, sizeof(struct sembuf));
	buf.sem_num = semno;
	buf.sem_op = 1;
	buf.sem_flg = 0;
	return semop(semid, &buf, 1);
}












