#include <iostream>

using namespace std;


class A {
public:
	virtual ~A() {
		cout << "~A()" << endl;
	}
};

class B: public A {
public:
	~B() {
		cout << "~B()" << endl;
	}
};

int main(int argc, char const *argv[])
{
	A* p = new B;
	delete p;
	return 0;
}