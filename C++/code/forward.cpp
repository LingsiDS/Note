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

template <typename T>
void bar(T &&x) {
	process(std::forward<T>(x));
}

int main () {
	int x = 1;
	foo(x);	
	foo(std::move(x));

	bar(x);
	bar(std::move(x));
	return 0;
}