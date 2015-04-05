#include <stdio.h>
int main(void)
{
	extern char **environ;
	printf("%s", *environ);
	return 0;
}
