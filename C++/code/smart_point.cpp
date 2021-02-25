#include <iostream>

using namespace std;

template <typename T>
class smart_ptr {
public:
	explicit smart_ptr(T* ptr = nullptr): ptr_(ptr) {

	}
	~smart_ptr() {
		delete ptr_;
	}
	T* get() const {
		return ptr_;
	}

	//运算符重载，使其看起来更像一个指针
	T& operator*() const { return *ptr_; }
	T* operator->() const { return ptr_; }
	operator bool() const { return ptr_; }

	//拷贝构造和拷贝赋值，第一种方案，禁止拷贝
	smart_ptr(const smart_ptr&) = delete;
	smart_ptr& operator=(const smart_ptr&) = delete;
private:
	T* ptr_;
};



template <typename T>
class auto_ptr {
public:
	explicit auto_ptr(T* ptr = nullptr): ptr_(ptr) {

	}
	~auto_ptr() {
		delete ptr_;
	}
	T* get() const {
		return ptr_;
	}

	//运算符重载，使其看起来更像一个指针
	T& operator*() const { return *ptr_; }
	T* operator->() const { return ptr_; }
	operator bool() const { return ptr_; }



	//拷贝构造和拷贝赋值，第二种方案，拷贝时转移指针的所有权
	auto_ptr(auto_ptr& other) {
		ptr_ = other.release();
	}
	auto_ptr& operator=(auto_ptr& rhs) {
		auto_ptr(rhs).swap(*this);
		return *this;
	}

	T* release() {		//让出指针的指向
		T* ptr = ptr_;
		ptr_ = nullptr;
		return ptr;
	}
	void swap(auto_ptr& rhs) {//交换两个指针的指向
		using std::swap;
		swap(ptr_, rhs.ptr_);
	}

private:
	T* ptr_;
};



template <typename T>
class unique_ptr {
public:
	explicit unique_ptr(T* ptr = nullptr): ptr_(ptr) {

	}
	~unique_ptr() {
		cout << "~unique_ptr()" << endl;
		delete ptr_;
	}
	T* get() const {
		return ptr_;
	}

	//运算符重载，使其看起来更像一个指针
	T& operator*() const { return *ptr_; }
	T* operator->() const { return ptr_; }
	operator bool() const { return ptr_; }



	//拷贝构造和拷贝赋值，第三种方案，指针的移动语义
	template <typename U>	//实现子类指针向基类指针的转换
	unique_ptr(unique_ptr<U>&& other) {	//定义移动构造函数，拷贝构造函数被禁用
		cout << "unique_ptr(unique_ptr<U>&& other): " << *other << endl;
		ptr_ = other.release();
	}
	unique_ptr(const unique_ptr &other) = delete;

	//赋值函数的行为是移动还是拷贝，完全依赖于构造函数时走的是移动构造还是拷贝构造
	unique_ptr& operator=(unique_ptr rhs) {
		rhs.swap(*this);
		return *this;
	}


	T* release() {		//让出指针
		T* ptr = ptr_;
		ptr_ = nullptr;
		return ptr;
	}
	void swap(unique_ptr& rhs) {//交换两个指针的指向
		using std::swap;
		swap(ptr_, rhs.ptr_);
	}

private:
	T* ptr_;
};

template <typename T>
class unique_ptr <T[]>{
public:
	explicit unique_ptr(T* ptr = nullptr): ptr_(ptr) {

	}
	~unique_ptr() {
		cout << "~unique_ptr()" << endl;
		delete[] ptr_;
	}
	T* get() const {
		return ptr_;
	}

	//运算符重载，使其看起来更像一个指针
	T& operator*() const { return *ptr_; }
	T* operator->() const { return ptr_; }
	operator bool() const { return ptr_; }



	//拷贝构造和拷贝赋值，第三种方案，指针的移动语义
	template <typename U>	//实现子类指针向基类指针的转换
	unique_ptr(unique_ptr<U>&& other) {	//定义移动构造函数，拷贝构造函数被禁用
		cout << "unique_ptr(unique_ptr<U>&& other): " << *other << endl;
		ptr_ = other.release();
	}
	unique_ptr(const unique_ptr &other) = delete;

	//赋值函数的行为是移动还是拷贝，完全依赖于构造函数时走的是移动构造还是拷贝构造
	unique_ptr& operator=(unique_ptr rhs) {
		rhs.swap(*this);
		return *this;
	}


	T* release() {		//让出指针
		T* ptr = ptr_;
		ptr_ = nullptr;
		return ptr;
	}
	void swap(unique_ptr& rhs) {//交换两个指针的指向
		using std::swap;
		swap(ptr_, rhs.ptr_);
	}

private:
	T* ptr_;
};



class shared_count {
public:
	shared_count():count_(1) {}
	void add_count() {
		++count_;
	}
	long reduce_count() {
		return --count_;
	}
	long get_count() {
		return count_;
	}
private:
	long count_;
};

template <typename T>
class shared_ptr {
public:
	template <typename U>
	friend class shared_ptr;

	shared_ptr(T* ptr = nullptr): ptr_(ptr) {
		if (ptr) {
			ptr_ = ptr;
			shared_count_ = new shared_count();
		}
	}

	template <typename U>
	shared_ptr(const shared_ptr<U>& other, T* ptr) {
		ptr_ = ptr;
		if (ptr_) {
			other.shared_count_->add_count();
			shared_count_ = other.shared_count_;
		}
	}

	

	~shared_ptr() {
		if (ptr_ && !shared_count_->reduce_count()) {
			delete ptr_;
			delete shared_count_;
		}
	}

	void swap(shared_ptr& rhs) {
		using std::swap;
		swap(ptr_, rhs.ptr_);
		swap(shared_count_, rhs.shared_count_);
	}

	template<typename U>
	shared_ptr(const shared_ptr<U>& other) {
		ptr_ = other.ptr_;//两个指针指向同一块内存，增加引用计数
		if (ptr_) {
			other.shared_count_->add_count();
			shared_count_ = other.shared_count_;
		}
	}
	template<typename U>
	shared_ptr(shared_ptr<U>&& other) {
		ptr_ = other.ptr_;
		if (ptr_) {//用右值构造了新值，右值消亡，引用计数不变
			shared_count_ = other.shared_count_;
			other.ptr_ = nullptr;
		}
	}

	shared_ptr& operator=(shared_ptr rhs) noexcept{
		rhs.swap(*this);
		return *this;
	}

	long use_count() const {
		if (ptr_) 
			return shared_count_->get_count();
		else
			return 0;
	}

	T* get() const {
		return ptr_;
	}
private:
	T* ptr_;
	shared_count * shared_count_;
};



class shape {
public:
	virtual ~shape() {}
};

class circle: public shape {
public:
	~circle() {puts("~circle()");}
};


template <typename T, typename U>
shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U>& other) {
	T* ptr = dynamic_cast<T*>(other.get());
	return shared_ptr<T>(other, ptr);
}

int main() {
	// shared_ptr<circle> ptr1(new circle());
	// printf("use count of ptr1 is %ld\n", ptr1.use_count());

	// shared_ptr<shape> ptr2;
	// printf("use count of ptr2 is %ld\n", ptr2.use_count());

	// ptr2 = ptr1;
	// printf("use count of ptr1 is %ld\n", ptr1.use_count());
	// printf("use count of ptr2 is %ld\n", ptr2.use_count());


	// shared_ptr<circle> ptr3 = dynamic_pointer_cast<circle>(ptr2);
	// printf("use count of ptr3 is %ld\n", ptr3.use_count());


	unique_ptr<int> up1(new int(1));
	// cout << *up1 << endl;
	unique_ptr<int> up2(new int(2));
	// unique_ptr<int> up3(new int(3));
	up1 = std::move(up2);
	return 0;
}