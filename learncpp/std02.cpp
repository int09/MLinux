#include <iostream>

using namespace std;

void add5(int &src) {
	src += 5;
}
void subtract(int &src) {
	src -= 5;
}
void mul5(int &src) {
	src *= 5;
}

typedef void (*action) (int &src);


void forEach(int a[], const int &len, action func) {
	for (int i = 0; i < len; ++i) {
		func(a[i]);
	}
	cout << endl;
}

void print(int a[], const int &len) {
	
	for (int i = 0; i < len; ++i) {
		cout << a[i] << ' ';
	}
	cout << endl;
}
int main() {
	int a[] = {1, 2, 4, 5, 6, 7};
	forEach(a, 6, add5);
	print(a, 6);
	forEach(a, 6, mul5);
	print(a, 6);
	return 0;
}
