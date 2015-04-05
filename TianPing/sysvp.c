    /*使用信号量实现进程同步与互斥*/   
    /*3个子进程中一个是生产者进程，两个是消费者进程*/   
    /*生产者进程发送0～9到由5个缓冲区组成的共享内存中*/   
    /*两个消费者进程轮流接收并输出这10个信息，同时两个消费者进程对读出的所有数据进行累加*/   
    /*由父进程输出两个消费者进程累加的和*/   
    #include <sys/types.h>   
    #include <linux/sem.h>   
    #include <linux/shm.h>   
    #include <unistd.h>   
    #include <stdio.h>   
    #include <errno.h>   
    #include <time.h>   
    #include <stdlib.h>   
    #define MAXSEM 5    /*定义缓冲区数组的下标变量个数*/   
       
    /******定义3个信号量的内部标识******/   
    int fullid;   
    int emptyid;   
    int mutexid;   
       
    int main()   
    {   
        /******定义信号量所用的数据结构******/   
        struct sembuf P,V;     
        union semun arg;   
       
        /******定义4个共享内存的ID******/   
        int arrayid;       
        int setid;   
        int getid;   
        int sumid;  /*存放数据和*/   
        /******定义共享内存虚拟地址******/   
        int *array;    
        int *set;   
        int *get;   
        int *sum;   
        /******创建共享内存******/   
        arrayid=shmget(IPC_PRIVATE,sizeof( int )*MAXSEM,IPC_CREAT|0666);       
        setid=shmget(IPC_PRIVATE,sizeof( int ),IPC_CREAT|0666);   
        getid=shmget(IPC_PRIVATE,sizeof( int ),IPC_CREAT|0666);   
        sumid=shmget(IPC_PRIVATE,sizeof( int ),IPC_CREAT|0666);   
        /******初始化共享内存******/   
        array= (int *) shmat(arrayid,0,0);     
        get= (int * ) shmat(getid,0,0);   
        *get=0;   
        set= (int * ) shmat(setid,0,0);   
        *set=0;    
        sum= (int * ) shmat(sumid,0,0);   
        *sum=0;   
       
        /******创建信号量******/   
        fullid= semget(IPC_PRIVATE,1,IPC_CREAT|0666);     
        emptyid=semget(IPC_PRIVATE,1,IPC_CREAT|0666);   
        mutexid=semget(IPC_PRIVATE,1,IPC_CREAT|0666);   
        /******初始化信号量******/   
        arg.val = 0;                /*初始时缓冲区中无数据*/   
        if(semctl(fullid , 0 , SETVAL , arg) == -1) perror("semctl setval error");   
        arg.val = MAXSEM;       /*初始时缓冲区中有5个空闲的数组元素*/   
        if(semctl(emptyid , 0 ,SETVAL , arg) == -1) perror("semctl setval error");   
        arg.val = 1;                /*初始时互斥信号为1，允许一个进程进入*/   
        if(semctl(mutexid , 0 ,SETVAL , arg) == -1) perror("setctl setval error");   
        /******初始化P,V操作******/   
        P.sem_num=0;   
        P.sem_op =-1;           /*P操作需要-1*/   
        P.sem_flg=SEM_UNDO;   
        V.sem_num=0;   
        V.sem_op =1;            /*V操作需要+1*/   
        V.sem_flg=SEM_UNDO;    
       
           
        /******消费者A进程******/   
        if(fork()==0)    
        {   
            while(1)   
            {   
                if(*get == 10)   
                    break;                     
                semop(fullid , &P , 1);         /*执行P(full)*/   
                semop(mutexid , &P , 1);        /*执行P(mutex)*/   
                printf("The ComsumerB get number from NO. %d\n",(*get)%MAXSEM);    
                (*sum)=(*sum)+array[(*get)%MAXSEM];     /*将取出的数据累加到SUM中*/   
                (*get)++;                      
                semop(mutexid , &V , 1);        /*执行V(mutex)*/   
                semop(emptyid , &V ,1 );        /*执行V(empty)*/   
                sleep(1);   
            }   
            printf("ConsumerB is over\n");   
            exit(0);   
        }   
        else    
        {   
            /******消费者B进程******/   
            if(fork()==0)    
            {   
                while(1)   
                {   
                    if(*get == 10)    
                        break;                 
                    semop(fullid , &P , 1);         /*执行P(full)*/   
                    semop(mutexid , &P , 1);        /*执行P(mutex)*/   
                    printf("The ComsumerA get number from NO. %d\n", (*get)%MAXSEM );   
                    (*sum)=(*sum)+array[(*get)%MAXSEM]; /*将取出的数据累加到SUM中*/   
                    (*get)++;                  
                    semop(mutexid , &V , 1);        /*执行V(mute)*/   
                    semop(emptyid , &V ,1 );        /*执行V(empty)*/   
                    sleep(1);   
                }   
                printf("ConsumerA is over\n");   
                exit(0);   
            }   
            else    
            {   
                /******生产者进程******/   
                if(fork() == 0 )     
                {    
                    int i = 0;   
                    while( i < 10)   
                    {   
                    semop(emptyid , &P ,1 );        /*执行P(empty)*/   
                    semop(mutexid , &P , 1);        /*执行P(mutex)*/   
                    array[*(set)%MAXSEM] = i + 1;   
                    printf("Producer put number %d to NO. %d\n", array[(*set)%MAXSEM],(*set)%MAXSEM);   
                    (*set)++;   
                    semop(mutexid , &V , 1);        /*执行V(mutex)*/   
                    semop(fullid , &V , 1);         /*执行v(full)*/   
                    i++;     
                    }   
                sleep(5);                   /*睡眠5秒，等待消费者进程执行完毕*/   
                printf("Producer is over\n");   
                exit(0);   
                }   
            }              
        }   
        /******父进程回收子进程******/   
        wait(0);           
        wait(0);   
        wait(0);   
        /*****父进程输出信息总和sum*****/   
        printf("The ComsumerA&B got numbers from shared buffer,the total is: %d\n",(*sum));    
        /******断开并撤消共享内存******/   
        shmdt(array);   
        shmctl(arrayid,IPC_RMID,0);    
        shmdt(get);   
        shmctl(getid,IPC_RMID,0);   
        shmdt(set);   
        shmctl(setid,IPC_RMID,0);   
        /******撤消信号量******/   
        semctl(emptyid,IPC_RMID,0);    
        semctl(fullid,IPC_RMID,0);   
        semctl(mutexid,IPC_RMID,0);   
        exit(0);   
    }   
