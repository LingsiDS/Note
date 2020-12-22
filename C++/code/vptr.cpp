#include <iostream>
#include <cstdio>
using namespace std;

class A {
public:
	virtual void vfunc1(){
		cout << "A::vfunc1()" << endl;
	}
	virtual void vfunc2() {
		cout << "A::vfunc2()" << endl;
	}
	void func1();
	void func2();
private:
	int m_data1, m_data2;
};


class B: public A{
public:
	virtual void vfunc1() {
		cout << "B::vfunc1()" << endl;
	}
	void func2();
private:
	int m_data3;
};


class C: public B {
public:
	virtual void vfunc1() {
		cout << "C::vfunc1()" << endl;
	}
	void func2();
private:
	int m_data1, m_data4;
};


int main () {
	// A *a = new A();
	// A *b = new B();
	// A *c = new C();

	// a->vfunc1();
	// a->vfunc2();
	// b->vfunc1();
	// c->vfunc1();

	
	// printf("%x\n", a);
	// printf("%x\n", b);
	// printf("%x\n", c);


	A a1;
	B b1;
	C c1;
	printf("sizeof(A) = %d\n", sizeof(a1));
	printf("sizeof(B) = %d\n", sizeof(b1));
	printf("sizeof(C) = %d\n", sizeof(c1));
	printf("%x\n", a1);
	printf("%x\n", b1);
	printf("%x\n", c1);


	// void *vptra = NULL, *vptrb = NULL, *vptrc = NULL;
	// vptra = (void *)&a1;	//虚指针的内容
	// printf("void = %x\n", *(int*)vptra);
	// printf("int = %x\n", *(int *)&a1);

	
	// printf("%x\n", *(void*)vptra);	//第一个虚函数指针的地址
	// printf("%x\n", (*(void*)(vptra + 1)) ); //第二个虚函数指针的地址

	typedef void(*Fun)(void);//函数指针

	Fun f1 = (Fun)(*(int*)*(int*)&a1); // A::vfunc1
	Fun f2 = (Fun)(*( ((int*)(*(int*)(&a1))) + 1 ));

	f1();
	f2();

	// int tt = 1;
	// printf("111 %x\n", &tt);
	// void *pp  = &tt;
	// printf("222 %x\n", pp);

	return 0;
}