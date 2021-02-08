#include <iostream>
using namespace std;

int fun1(int &x) {
	x = 1;
	return x;
}

int fun2(int *x) {
	*x = 1;
	return *x;
}

int main () {
	int i = 1;
	int &ri = i;
	ri = 2;
	int * const pi = &i;
	*pi = 3;

	fun1(ri);
	fun2(pi);
	return 0;
}