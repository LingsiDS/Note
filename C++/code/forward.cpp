#include <iostream>
using namespace std;

void process(int &x) {
	cout << "process(int &x)" << endl;
}

void process(int &&x) {
	cout << "process(int &&x)" << endl;
}

void foo(int &&x) {
	process(x);//process(std::forward<int>(x));
}

void foo(int &x) {
	process(x);
}

int main () {
	int x = 1;
	foo(x);	
	foo(std::move(x));
	return 0;
}