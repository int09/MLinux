#include<unistd.h> #include<stdio.h> #include<stdlib.h> #include<string.h> #include<sys/types.h> #include<linux/sem.h> int j=0; main() {    
    int array;  //共享内存array     int get;    //共享内存get 
    int set=0;  //生产者存入的下标位置 
    int customer_a,customer_b;  //消费者A，B进程     int producer;  //生产者进程     int *viraddr;  //虚地址 
    int mutex,empty,full;  //信号量     int i,m;     int getnum;     int sum_a=0;     int sum_b=0;     int aget;     int bget; 
    struct sembuf P,V; 
    union semun arg;  //共用体     
    get=shmget(IPC_PRIVATE,4*sizeof(int),0666|IPC_CREAT);    //创建共享存储区 get 
    viraddr=(int*)shmat(get,0,0); //挂起 
    for(i=0;i<3;i++)              //get的0下标用来存读取的下标 1下标来存a累加和 2下标存b累加和     {  
        viraddr[i]=0; 



  


    }     
    array=shmget(IPC_PRIVATE,5*sizeof(int),0666|IPC_CREAT); //创建共享存储区 array     
    empty=semget(IPC_PRIVATE,1,0666|IPC_CREAT);//创建信号量empty 初始化 
    arg.val=5; 
    if(semctl(empty,0,SETVAL,arg)==-1)         perror("semctl setval error 1");  
    full=semget(IPC_PRIVATE,1,0666|IPC_CREAT);//full     arg.val=0; 
    if(semctl(full,0,SETVAL,arg)==-1)         perror("semctl setval error 2"); 
    mutex=semget(IPC_PRIVATE,1,0666|IPC_CREAT);//mutex     arg.val=1; 
    if(semctl(mutex,0,SETVAL,arg)==-1)         perror("semctl setval error 3");  
    P.sem_num=0;     P.sem_op=-1; 
    P.sem_flg=SEM_UNDO; 
    V.sem_num=0;                                       //50     V.sem_op=1; 
    V.sem_flg=SEM_UNDO;      
    //********************************************************     while((producer=fork())==-1);//创建生产者子进程     if(producer==0)//如果生产者子进程返回    {   
        for(i=1;i<=10;i++)         {    
            semop(empty,&P,1); //P(empty);             semop(mutex,&P,1); //P(mutex); 
           printf("生产者写入数据%d\n",i);//写数据到共享存储区             viraddr=(int*)shmat(array,0,0);//挂起                      viraddr[set]=i;   //往set缓冲区存数  //           sleep(2);  
            set++;             if(set>4) 



  


            { 
                set=set-5;             } 
            semop(mutex,&V,1); //V(mutex);             semop(full,&V,1);  //V(full);         } 
        exit(0);     } 
    else//如果父进程返回     {    
        while((customer_a=fork())==-1);//创建消费者A子进程             if(customer_a==0)//如果消费者A 进程返回         { 
            for(i=0;i<5;i++)             { 
                semop(full,&P,1); //P(full); 
                semop(mutex,&P,1); //P(mutex);                       
                viraddr=(int*)shmat(get,0,0); 
                getnum=viraddr[0];           //getnum存要读取的位置的下标 
                viraddr[0]=viraddr[0]+1;     //下标加一 
                if(getnum>4)                 //下标大于4 循环                 { 
                    getnum=getnum-5;                 } 
                viraddr=(int*)shmat(array,0,0);//array挂起 
                aget=viraddr[getnum];   //A读的数存入aget中 
                printf("消费者A读出数据:");//从共享存储区中取数据                 printf("%d\n",viraddr[getnum]);                 viraddr=(int*)shmat(get,0,0); 
                viraddr[1]=viraddr[1]+aget;   //累加和  
                semop(mutex,&V,1); //V(mutex);                 semop(empty,&V,1); //V(empty);            } 
            printf(" end\n");             exit(0);  
        }                                                     //100 



  


        else//如果父进程返回         {                
            while((customer_b=fork())==-1);//创建消费者B子进程             if(customer_b==0)//如果消费者B进程返回             { 
                for(i=0;i<5;i++)                 { 
                    semop(full,&P,1); //P(full); 
                    semop(mutex,&P,1); //P(mutex);                      
                    viraddr=(int*)shmat(get,0,0);                     getnum=*viraddr;                     *viraddr=*viraddr+1;                     if(getnum>4)                     { 
                        getnum=getnum-5;                     } 
                    viraddr=(int*)shmat(array,0,0);//挂接array                     bget=viraddr[getnum]; 
                    printf("消费者B读出数据:");//从共享存储区中取数据 
                    printf("%d\n",viraddr[getnum]);                     viraddr=(int*)shmat(get,0,0); 
                    viraddr[2]=viraddr[2]+bget;  //累加和                     sum_b=sum_b+viraddr[getnum];                     semop(mutex,&V,1); //V(mutex);                     semop(empty,&V,1); //V(empty);                 } 
                printf(" end\n");                 exit(0);             }             else             { 
                wait(0);                 wait(0);                 wait(0); 
                printf("wait finish!\n"); 
                viraddr=(int*)shmat(get,0,0);  
                printf("suma=%d\n",viraddr[1]);                 printf("sumb=%d\n",viraddr[2]); 



  



