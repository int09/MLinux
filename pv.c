
semaphore	bufnum	=	5;
semaphore	pronum	=	0;
semaphore	mutex	=	1;

int	array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int cursor = 0;

void pproducer(void)
{
	for (int i = 0; i < 10; ++i)
	{
		P(bufnum);
		P(mutex);
		buffer[cursor++] = array[i];
		V(mutex);
		V(pronum);
	}
}

void pconsumer0(void)
{
	int number, sum = 0;
	while (TRUE)
	{
		P(pronum);
		P(mutex);
		number = buffer[cursor--];
		printf("%d\n", number);
		sum += number;
		V(mutex);
		P(bufnum);
	}
	printf("累加和为 %d\n", sum);
}

void pconsumer1(void)
{
	int number, sum = 0;
	while (TRUE)
	{
		P(pronum);
		P(mutex);
		number = buffer[cursor--];
		printf("%d\n", number);
		sum += number;
		V(mutex);
		V(bufnum);
	}
	printf("累加和为 %d\n", sum);
}
