#include <iostream>
#include <string>
#include <complex>
#include <vector>
using namespace std;

void f() {
	cout << "f()" << endl;
}

void f1() {
	cout << "f1()" << endl;
}



vector<int> foo() {
	return vector<int>(10);
}

int &&bar() {
	int x = 1;
	return x;
}


int main () {
	string s1("Hello");
	string s2("world");
	s1 + s2 = s2;			//s1 + s2是一个右值，却可以出现在赋值号左侧？
	cout << s1 << endl;
	cout << s2 << endl;

	string() = s2;		//string()是一个右值，却可以出现在赋值号左侧？
	
	complex<int> c1(1, 2), c2(3, 4);
	c1 + c2 = complex<int>(5, 6);
	complex<int>() = c1;

	// auto p1 = &(c1 + c2);			//error: taking address of temporary
	// auto p2 = &complex<int>(1, 1);	//error: taking address of temporary

	auto p3 = &(vector<int>(10)[0]);
	// auto p4 = &vector<int>(10);

	// vector<int>&& rv = foo();
	// &(foo()[0]);

	// auto p5 = &(foo());//error: taking address of temporary

	// &(true);//error: lvalue required as unary '&' operand
	
	// &std::move(s1);//error: taking address of xvalue(rvalue reference)

	&(s1 + s2);
	&s1.substr(1, 1);

	&(s1 < s2);

	&bar();

	&"hello world";
	return 0;
}