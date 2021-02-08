#include <iostream>


using namespace std;

int main () {
	const int a = 10;
	cout << a << endl;

	int *p = (int *)&a;
	*p = 20;

	cout << a << endl;	//不能改变
	cout << *p << endl;
}