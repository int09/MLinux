#include <stdio.h>
int main()

{
	int a = 10, b = 8;
	float x = 10.2, y = 8.1;
	printf("int a+b=%d\n", add_int(a, b));
	prnitf("int a-b=%d\n", sub_int(a, b));
	printf("float a+b=%f\n", add_float(x, y));
	printf("float a-b=%f\n", sub_float(x, y));
	return 0;
}
