import time

# 判断num是不是一个质数
def is_prime(num):
	if num < 2:
		return False
	for i in range(2, num):
		if num % i == 0:
			return False
	return True

# # 打印1到10000之间的所有质数
# def prime_nums():
# 	t1 = time.time()
# 	for i in range(10000):
# 		if is_prime(i):
# 			print(i)
# 	t2 = time.time()
# 	print(t2 - t1, ' s')


# # 定义计算时间的decorator
# def display_time(func):
# 	def wrapper():
# 		t1 = time.time()
# 		func()
# 		t2 = time.time()
# 		print("Total time: {:4} s".format(t2 - t1))
# 	return wrapper

# @display_time
# def prime_nums():
# 	for i in range(10000):
# 		if is_prime(i):
# 			print(i)

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
def count_prime_nums(maxnum):
	count = 0
	for i in range(maxnum):
		if is_prime(i):
			count += 1
	return count


count = count_prime_nums(10000)
print(count)
