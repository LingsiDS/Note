

## 装饰器

假设有如下程序，打印出`[1, 10000)`内的所有质数，并打印程序执行的时间

```python
# 判断num是不是一个质数
def is_prime(num):
	if num < 2:
		return False
	for i in range(2, num):
		if num % i == 0:
			return False
	return True

# 打印1到10000之间的所有质数
def prime_nums():
	t1 = time.time()
	for i in range(10000):
		if is_prime(i):
			print(i)
	t2 = time.time()
	print(t2 - t1, ' s')
```

这样`prime_nums()`里既有判断质数程序逻辑部分，又有程序计算时间的部分，这样程序可读性下降。计算程序执行时间可能是多个函数都有的需求，可以使用装饰器，将计算时间的功能抽离出来

```python
# 定义计算时间的decorator
def display_time(func):
	def wrapper():
		t1 = time.time()
		func()
		t2 = time.time()
		print("Total time: {:4} s".format(t2 - t1))
	return wrapper

@display_time
def prime_nums():
	for i in range(10000):
		if is_prime(i):
			print(i)
```

这段程序使用装饰器对第一段程序进行了重写，功能完全一样。装饰器本质是一个函数，其参数`func`是装饰器修饰的函数，`wrapper`函数对`func()`函数的执行流程做了一个封装，包裹了一个时间计算代码段，最后装饰器返回`wrapper`。

如果一个装饰器修饰的函数有**参数**和**返回值**呢？

```python
# 定义计算时间的decorator
def display_time(func):
	def wrapper(*args): #args接受func的参数
		t1 = time.time()
		count = func(*args) #将args传回给func，如果有返回值，需要接受返回值
		t2 = time.time()
		print("Total time {:.4} s".format(t2 - t1))
		return count #将func的返回值返回
	return wrapper

@display_time
def count_prime_nums(maxnum): #统计[0, maxnum)的质数的个数
	count = 0
	for i in range(maxnum):
		if is_prime(i):
			count += 1
	return count
```

