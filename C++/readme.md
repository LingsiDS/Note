# C++

## 默认初始化与值初始化

何时执行值初始化？参考[这里](https://en.cppreference.com/w/cpp/language/value_initialization)，简单来说如果定义变量时，使用了一对小括号或者大括号，可以认为该变量被值初始化；如果定义变量时未指定任何初始化方式，则该变量执行默认初始化。

- 对于对象来说，默认初始化和值初始化都是调用对象的**默认构造函数**

- 对于内置类型来说，值初始化意味着内置类型被初始化为零，而默认初始化依据内置类型所在的位置不同其行为也有所不同，如果内置类型是static或者在全局作用域，则被初始化为0，否则内置类型将不被初始化，其值是未定义的。



## 默认构造函数

如果创建一个对象时没有提供初始值，则该对象执行了**默认初始化**，对象的默认初始化是通过默认构造函数进行初始化的。默认的构造函数按照如下规则初始化类的数据成员：

- 如果存在类内的初始值，用它来初始化成员
- 否则，**默认初始化该成员**



## 构造函数初始值列表（p258)

- 如果没有在构造函数的初始值列表中显式地初始化成员，则该成员将在**构造函数体之前执行默认初始化**。

- 如果成员是**const**、**引用**，或者属于某种**未提供默认构造函数的类类型**，必须通过构造函数初始值列表为这些成员提供初始值。
- 构造函数初始值列表只说明用于初始化成员的值，而**不限定初始化的具体执行顺序**。成员的初始化顺序与它们在类中**定义中出现的顺序**一致。

```c++
//成员初始值列表测试
#include <iostream>
using namespace std;

class NoDefault {
public:
	NoDefault(int i):data(i) {//没有默认构造函数
	}
private:
	int data;
};

class ConstRef {
public:
    /*如果不在初始值列表中初始化引用和const成员会得到得到如下错误
    error: uninitialized const member in 'const int"
	error: uninitialized reference member in 'int& '*/
	ConstRef(int ii):df(NoDefault(ii)), i(ii), ci(ii), ri(ii) {}
private:
	NoDefault df;
	int i;
	const int ci;
	int &ri;
};
```



## 虚指针以及虚函数表







## 虚析构函数

如果父类的析构函数是虚函数，当子类定义了析构函数时（相当于`overwrite`了父类的虚析构函数），如`Base *p = new Derived`; 当`delete p`时，不再是静态绑定，而是动态绑定，会根据动态绑定调用子类的析构函数，执行完子类析构函数的函数体后，在子类析构函数结束之前调用父类的析构函数（这是编译器保证）完成父类对象的析构。

```c++
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
	delete p;//~B() ~A()
	return 0;
}
```



## 虚基类

当派生类从多个基类派生，而这些基类又有共同基类，则在访问此共同基类中的成员时，将产生冗余，并且可能因冗余带来不一致性。

虚基类主要用来解决多继承时可能发生的对同一基类继承多次而产生的二义性问题。虚基类为最远的派生类提供唯一的基类成员，而不重复产生多次复制。

![image-20201222212404784](assets/image-20201222212404784.png)

在整个继承结构中，直接或间接继承虚基类的**所有派生类**，都必须在构造函数的成员初始化表中调用虚基类的构造函数初始化虚基类的成员，如果未显示调用虚基类的构造函数，则表示调用该虚基类的默认构造函数，在建立对象时，只有最远派生类的构造函数调用虚基类的构造函数，其他类对虚基类构造函数的调用被忽略。

```c++
class Base0{
public:
    Base0(int var);
    int var0;
};
class Base1: public virtual Base0 {
public:
    Base1(int var):Base0(var) {}
    int var1;
};
class Base2: public virtual Base0 {
public:
    Base2(int var):Base0(var) {}
    int var2;
};
class Derived: public Base1, public Base2 {
public:
    Derived(int var): Base0(var), Base1(var), Base2(var) {}
    int var;
};
```



## 模板偏特化





































