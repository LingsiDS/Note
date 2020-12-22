//成员初始值列表测试
#include <iostream>
using namespace std;

class NoDefault {
public:
	NoDefault(int i):data(i) {

	}
private:
	int data;
};

class ConstRef {
public:
	//error: uninitialized const member in 'const int"
	//error: uninitialized reference member in 'int& '
	ConstRef(int ii):df(NoDefault(ii)), i(ii), ci(ii), ri(ii) {
		i = ii;
		//ci = ii;//error: assignment of read-only member 'ConstRef: :ci'
		ri = i;
	}
private:
	NoDefault df;
	int i;
	const int ci;
	int &ri;
};

int main () {

	return 0;
}
