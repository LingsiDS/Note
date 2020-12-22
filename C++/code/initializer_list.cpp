#include <iostream>
#include <initializer_list>
using namespace std;


struct Point{
	int x, y;
	Point(int xx = 1, int yy = 2):x(xx), y(yy) {}
};

struct Foo{
	int a;
	double b;
	Point p;

};
ostream& operator<< (ostream& os, Foo &f) {
	os << f.a << " " << f.b << " " << f.p.x << " " << f.p.y;
	return os;
} 
int main () {
	Foo f = {1};
	cout << f << endl;
	return 0;
}