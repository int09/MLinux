/*
由一个进程创建三个子进程，三个子进程一个是生产者进程，两个是消费者进程
父子进程都使用附近城创建的共享存储区进行通信，由生产者讲一个数组中的十
个数值发送到由5个缓冲区组成的共享内存中，两个消费者进程轮番接收并输出这
10个数值，同事讲两个消费者进程独处的数值进行累加求和
*/

#include <stdio.h>		//包含函数库，以使用共享内存/信号量等
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <memory.h>


int	array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int in = 0, out = 0;
#define		CONSUMER	1	//定义消费者使用的信号量编号
#define		PRODUCER	0	//定义生产者使用的信号量编号

int PO(int semid, int semno);
int VO(int semid, int semno);
int writed = 0;
typedef struct{
	int buf[5];
} shm_struct;

int main(int argc, char *argv[])
{
	
	pid_t producer;					//定义生产者PID
	pid_t consumer0, consumer1;		//定义两个消费者的PID
	
	int semid;						//信号量的ID
	int semcc;						//消费者互斥使用的ID信号量
	key_t key;						//用以作为信号量和共享内存获取的参数
	if ((key = ftok("/dev/zero", 1)) < 0)	//产生key
	{
		perror("ftok error");		//错误处理
		exit(-1);
	}
	if ((semid = semget(key, 2, IPC_CREAT | 0666)) < 0)	//获取信号量
	{
		perror("semget error");		//错误处理
		exit(-1);
	}
	if ((semcc = semget(key, 1, IPC_CREAT | 0666)) < 0)
	{
		perror("semget error");
		exit(-1);
	}
	short input_array[2];			//作为semctl函数的SETALL选项的参数
	input_array[0] = 5;				//bufnum
	input_array[1] = 0;				//pronum
	semctl(semid, 2, SETALL, input_array);	//将两个信号量分别设置为5和0
	union semun {
		int		val;
		struct semid_ds *buf;
		unsigned short *array;
	} su;
	su.val = 1;
	semctl(semcc, 0, SETVAL, su);
	int shmid;						//定义共享内存ID
	shm_struct *buffer;					//5个缓冲区
	int bufsize = 0;				//标示缓冲区最末的数据
	//共享内存获取，第一个参数是之前获取的key， 第三个参数说明创建的方式
	if ((shmid = shmget(key, 1024, 0600)) < 0)
	{
		perror("shmget error");		//错误处理
		exit(-1);
	}
	buffer = (shm_struct *)shmat(shmid, 0, 0);	//映射到共享内存区
	
	
	if ((producer = fork()) < 0)	//生产者的进程，由主进程派生出的子进程
	{	
		perror("producer fork error");	//错误处理
		exit(-1);
	}
	if ((consumer0 = fork()) < 0)	//消费者0的进程，由主进程派生出的子进程
	{
		perror("consumer0 fork error");	//错误处理
		exit(-1);
	}
	if ((consumer1 = fork()) < 0)	//消费者1的进程，由主进程派生出的子进程
	{
		perror("consumer1 fork error");	//错误处理
		exit(-1);
	}

	if (producer == 0)			//如果是子进程
	{
		for (int i = 0; i < 10; ++i)
		{
			PO(semid,0);
			PO(semcc, 0);
			buffer->buf[in] = array[i];
			in = (in + 1) % 5;
			writed++;
			VO(semcc, 0);
			VO(semid, 1);
		}
	}
	else if (consumer0 == 0)		//如果是子进程
	{
		int number, sum = 0;
		while (writed < 10)
		{
			PO(semid, CONSUMER);
			PO(semcc, 0);
			number = buffer->buf[out];
			out = (out + 1) % 5;
			printf("%d\n", number);
			sum += number;
			VO(semcc, 0);
			PO(semid, PRODUCER);
		}
		printf("Consumer0的累加和为 %d\n", sum);
	}
	else if (consumer1 == 0)		//如果是子进程
	{
		int number, sum = 0;
		while (writed < 10)
		{
			PO(semid, CONSUMER);
			PO(semcc, 0);
			number = buffer->buf[out];
			out = (out + 1) % 5;
			printf("%d\n", number);
			sum += number;
			VO(semcc, 0);
			VO(semid, PRODUCER);
		}
		printf("Consumer1的累加和为 %d\n", sum);
	}
	shmdt(buffer);
	return 0;
}
int PO(int semid, int semno)					//P操作
{
	struct sembuf buf;							//定义sembuf结构体变量buf，用以控制信号量
	memset(&buf, 0x00, sizeof(struct sembuf));	//对buf结构体清零
	buf.sem_num = semno;						//信号量组的第semno个信号量
	buf.sem_op = -1;							//对该信号量的操作
	buf.sem_flg = 0;							//信号量操作的标志
	return semop(semid, &buf, 1);				//返回semop操作函数，完成对信号量的操作
}

int VO(int semid, int semno)					//V操作
{
	struct sembuf buf;							//操作同上PO
	memset(&buf, 0x00, sizeof(struct sembuf));
	buf.sem_num = semno;
	buf.sem_op = 1;			
	buf.sem_flg = 0;
	return semop(semid, &buf, 1);
}
