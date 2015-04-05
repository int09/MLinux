#include <iostream>

using namespace std;
void forEach(int a[], const int &len) {
	for (int i = 0; i < len; ++i) {
		a[i] += 5;
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
	forEach(a, 6);
	print(a, 6);
	return 0;
}
