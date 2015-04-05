/*
由一个进程创建三个子进程，三个子进程一个是生产者进程，两个是消费者进程
父子进程都使用附近城创建的共享存储区进行通信，由生产者讲一个数组中的十
个数值发送到由5个缓冲区组成的共享内存中，两个消费者进程轮番接收并输出这
10个数值，同事讲两个消费者进程独处的数值进行累加求和
*/

#define		CONSUMER	1
#define		PRODUCER	0

int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int PO(int semid, int semno)
{
	struct sembuf buf;
	memset(&buf, 0x00, sizeof(struct sembuf));
	buf.sem_num = semno;
	buf.sem_op = -1;
	buf.sem_flg = 0;
	return semop(semid, &buf, 1);
}

int VO(int semid, int semno)
{
	struct sembuf buf;
	memset(&buf, 0x00, sizeof(struct sembuf));
	buf.sem_num = semno;
	buf.sem_op = 1;
	buf.sem_flg = 0;
	return semop(semid, &buf, 1);
}


int main(int argc, char *argv[])
{
	
	pid_t producer;
	pid_t consumer;
	
	int semid;
	key_t key;
	if ((semkey = ftok("etc/profile", 1)) < )
	{
		perror("ftok error");
		exit(-1);
	}
	if ((semid = semget(key, 2, IPC_CREAT | 0666)) < 0)
	{
		perror("semget error");
		exit(-1);
	}
	short input_array[2];
	input_array[0] = 5;	//Producer
	input_array[1] = 0;	//Consumer
	semctl(semid, 2, SETALL, input_array);
	
	/*---------------------------------------------------------*/
	int shmid;
	int buffer[5];
	int bufsize = 0;
	if ((shmid = shmget(key, sizeof(buffer), IPC_CREAT | IPC_EXCL | 0666)) < 0)
	{
		perror("shmget error");
		exit(-1);
	}
	if ((buffer = shmat(shmid, NULL, 0)) == (void *)-1)
	{
		perror("shmat error");
		exit(-1);
	}
	
	
	if ((producer = fork()) < 0)
	{
		perror("producer fork error");
		exit(-1);
	}
	else if (producer == 0)
	{
		for(int i = 0; i < 10; ++i)
		{
			PO(semid, PRODUCER);
			buffer[bufsize++] = array[i];
			VO(semid, CONSUMER);
		}
	}

	if ((consumer0 = fork()) < 0)
	{
		perror("consumer0 fork error");
		exit(-1);
	}
	else if (consumer0 == 0)
	{
		while (bufsize > 0)
		{
			PO(semid, CONSUMER);
			printf("%d\n", buffer[bufsize--]);
			VO(semid, PRODUCER);
		}
	}
	
	if ((consumer1 = fork()) < 0)
	{
		perror("consumer1 fork error");
		exit(-1);
	}
	else if (consumer1 == 0)
	{
		while (bufsize > 0)
		{
			PO(semid, CONSUMER);
			printf("%d\n", buffer[bufsize--]);
			VO(semid, PRODUCER);
		}
	}
	return 0;
}
