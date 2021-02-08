# 标准IO

## perror

```c
#include <stdio.h>
void perror(const char *s);
```

当程序出错时，`errno`会根据错误原因设置为相应的值，可以借助`perror()`将`errno`对应的错误原因打印出来，参数`s`是自定义的错误描述，参数`s`将在打印`errno`错误原因之前打印

```c
FILE *fp = fopen("/not_create", "r");
if (fp == NULL) {
	perror("fopen() filed: ");
}
```

如上面的例子打开一个不存在的文件，输出：`fopen(): No such file or directory`

遵守标准：`perror(), errno: POSIX.1-2001, POSIX.1-2008, C89, C99, 4.3BSD.`



## fopen & fclose

### fopen

```c
FILE *fopen(const char *path, const char *mode);
```

fopen函数根据参数`path`打开文件，并且返回一个与之关联的**流**。mode指定打开文件的行为，其描述如下

| 打开模式 |      打开模式       |               文件指针位置                | 是否会截断文件 | 文件不存在是否会创建文件 |
| :------: | :-----------------: | :---------------------------------------: | :------------: | :----------------------: |
|    r     |       reading       |                 文件开始                  |       否       |            否            |
|    r+    |  reading & writing  |                 文件开始                  |       否       |            否            |
|    w     |       writing       |                 文件开始                  |       是       |            是            |
|    w+    |  reading & writing  |                 文件开始                  |       是       |            是            |
|    a     |      appending      |                 文件末尾                  |       否       |            是            |
|    a+    | reading & appending | 文件开始（reading）/文件末尾（appending） |       否       |            是            |

​		`mode`参数除了上面指定的情况外，还可以在上面所有打开模式之后或者两个字符之间加上字母`'b'`，表示以二进制方式打开一个文件，但是`‘b’`在任何准守`POSIX`标准的系统上被忽略，但是如果的确是打开一个二进制文件可以加上字母`'b'`，因为这样在非Unix类环境下仍然可以运行（可移植性好）

​		如果一个文件被创建，其访问权限为`S_IRUSR | S_IWUSR | S_IRGRP|  S_IWGRP  |  S_IROTH  | S_IWOTH   (0666)  & ~umask`，默认情况下`umask`为2，那么`fopen`创建的文件权限为`0664`（rwrwr)

​		`fopen`成功返回文件指针，失败返回`NULL`，并且设置好了`errno`



### fclose

​		`fopen`返回的是一个`FILE`类型的结构体的地址，并且这个结构体一定是在**堆**上动态申请的，所以我们要释放这个结构体所在的内存，这时就要调用`fclose`了

```c
int fclose(FILE *stream);
```

`fclose`使用`fflush(3)`刷新`stream`参数指定的流，并且关闭底层的文件描述符

打开一个文件后必须使用`fclose`的另一个原因是，每个进程打开文件的个数有一个上限，可以通过`ulimit -a`查看，可以看到，当前可以打开文件的上限是1024个文件，注意实际上用户能够打开的文件上限是1024-3=1021，因为每个进程都打开了`stdin`，`stdout`， `stderr`这三个流。

```shell
lingsi@ubuntu:~/workspace/linux_c$ ulimit -a
core file size          (blocks, -c) 0
data seg size           (kbytes, -d) unlimited
scheduling priority             (-e) 0
file size               (blocks, -f) unlimited
pending signals                 (-i) 7677
max locked memory       (kbytes, -l) 64
max memory size         (kbytes, -m) unlimited
open files                      (-n) 1024
pipe size            (512 bytes, -p) 8
POSIX message queues     (bytes, -q) 819200
real-time priority              (-r) 0
stack size              (kbytes, -s) 8192
cpu time               (seconds, -t) unlimited
max user processes              (-u) 7677
virtual memory          (kbytes, -v) unlimited
file locks                      (-x) unlimited
```



## fgetc() & fputc()

```c
#include <stdio.h>
int fgetc(FILE *stream);
```

​		`fgetc()`读取<u>stream</u>中的下一个字符，返回读取到的`unsigned char`转化为`int`的值，如果读取失败或者读取到了文件尾返回`EOF`。

```c
#include <stdio.h>
int fputc(int c, FILE *stream);
```

​		向<u>`stream`</u>指定的流中写入一个被转换为unsigned char类型的字符<u>`c`</u>，

`fgetc()`和`fputc()`的返回值：`return the character read as an unsigned char cast to an int or EOF on end of file or error.`



## fgets() & fputs

### fgets()

```c
#include <stdio.h>
char *fgets(char *s, int size, FILE *stream);
/*
fgets()  reads  in at most one less than size characters from stream and
       stores them into the buffer pointed to by s.  Reading stops after an EOF
       or  a  newline.   If a newline is read, it is stored into the buffer.  A
       terminating null byte ('\0') is stored after the last character  in  the
       buffer.
*/
```

`fgets()`，从<u>`stream`</u>中最多读取`size-1`个字符存储到<u>`s`</u>中。`fgets()`读取结束有3种情况

1. 读取到了`size - 1`个字符
2. 读取到了`EOF`
3. 读取到了换行符，并且换行符会被保留在<u>`s`</u>中

不过以何种方式结束，都会在<u>`s`</u>中存放的最后读取到的字符后面放置一个`'\0'`

```shell
lingsi@ubuntu:~/workspace/linux_c$ cat fgets.c
#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 1024

int main () {
	FILE *fp = fopen("./temp", "r");
	char buf[BUFSIZE];
	
	int cnt = 0;
	while (fgets(buf, 5, fp)) {
		cnt++;
		printf("cnt = %d, %s", cnt, buf);
	}
	return 0;
}
lingsi@ubuntu:~/workspace/linux_c$ cat temp 
12345

lingsi@ubuntu:~/workspace/linux_c$ ./fgets
cnt = 1, 1234cnt = 2, 5
cnt = 3, 
lingsi@ubuntu:~/workspace/linux_c$ 

```

### fputs()

```c
#include <stdio.h>
int fputs(const char *s, FILE *stream);
/*
fputs() writes the string s to stream, without its terminating null byte
       ('\0').
       
       fputs() return a nonnegative number on  success,  or  EOF  on
       error.
*/
```





## fread() & fwrite()

`fread()`和`fwrite()`为二进制读写IO操作

```c
#include <stdio.h>
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
```

`fread()`从`stream`中读取`nmemb`个数据对象，每个数据结构大小为`size`，将读取结果存到`ptr`指向的位置。

`fwrite()`向`stream`中写入从`ptr`开始的`nmemb`个数据对象，每个数据对象大小为`size`



返回值，`fread`和`fwrite`返回成功读/写的**数据对象的个数**，因此`fread()`无法判断是读取到了文件尾还是出错，需要使用`feof(3)`或者`ferror(3)`来确定是哪种情况。

如果使用`fread(ptr, 1, nmemb, stream)`，返回值就是读取的字节数



## printf() & scanf()函数族

### prinf

```c
#include <stdio.h>
int printf(const char *format, ...);
int fprintf(FILE *stream, const char *format, ...);
int sprintf(char *str, const char *format, ...);
int snprintf(char *str, size_t size, const char *format, ...);
```

这里主要关注`sprintf()`和`snprintf()`的区别，这两个函数都是将格式化字符串写入str中，但是，`sprintf()`没有指定缓冲区str的大小，所以可能会造成缓冲区溢出的问题，而`snprintf()`，最多向`str`写入`size`个字符（包括最后的`'\0'`），以上函数成功都返回成功输出的字符串的长度（不包括字符串结束符'\0'）。但是注意`snprintf()`如果要写入的字符数超过了缓冲区str的大小size，则返回值为要写入的字符串的长度（不包括字符串结束符）。



### scanf()

```c
#include <stdio.h>
int scanf(const char *format, ...);
int fscanf(FILE *stream, const char *format, ...);
int sscanf(const char *str, const char *format, ...);
```

`fscanf()`从流中读取，`sscanf`从字符串`str`中读取。所以可以用sscanf完成从字符串转换为整型数的功能。



## fseek() & ftell()

### fseek() & ftell()

```c
#include <stdio.h>
int fseek(FILE *stream, long offset, int whence);
long ftell(FILE *stream);
```

`fseek()`是移动文件读写指针，移动的位置根据参数`whence`为基准，`offset`为相对`whence`的偏移，最后成功返回当前的偏移，否则返回-1，并且设置好了`errno`。whence有这几个值，`SEEK_SET`，`SEEK_CUR`， `SEEK_END`，分别表示文件的开始位置，文件的当前位置，文件的末尾位置。

可以使用`fseek()`移动文件读写指针生成一个空洞文件（空洞文件为所有字符的ASCII为0）。



`ftell()`返回当前文件指针相对于文件开始的偏移量，注意返回值是long，如果在32为下，最大为2G-1。

利用`fseek()`和`ftell()`可以实现计算文件大小的程序。

```c
#include <stdlib.h>

int main() {
	FILE *fp = fopen("./temp", "r");
	if (fp == NULL) {
		perror("fopen()");
		exit(1);
	}
	
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	
	printf("size = %d\n", size);
	return 0;
}
```



### fseeko & ftelllo

```c
#include <stdio.h>
int fseeko(FILE *stream, off_t offset, int whence);
off_t ftello(FILE *stream);
```

`fseeko`和`ftello`的功能和`fseek` ，`ftell`一样，只不过`fseeko`参数`offset`和`ftello`的返回值由`long`变为`off_t`，如果使用long表示偏移，则最多只能操作2G-1大小的文件，将`long`改为`off_t`就是为了支持大文件，但是在一些架构上`off_t`和`long`都是32位，可以在编译时加上以下的定义将`off_t`转化为64位。

```makefile
gcc -D _FILE_OFFSET_BITS=64 ./big_file.c -o big_file
```



## fflush

```c
#include <stdio.h>
int fflush(FILE *stream);
/*
For  output streams, fflush() forces a write of all user-space buffered
       data for the given output or update stream via the stream's  underlying
       write function.
       
       If the stream argument  is  NULL,  fflush()  flushes  all  open  output
       streams.

*/
```

**标准IO下的缓冲**

缓冲区的作用：缓冲区大多数情况下是好事，可以合并系统调用。如将多个**标准IO（标准IO底层调用系统IO）**的要读写数据写入对应的缓冲区，当因为某种原因刷新缓冲区时，才**真正调用系统IO函数**完成实际的功能。

**全缓冲**：当缓冲区满了的时候，或者强制刷新时刷新。（默认一个流都是全缓冲，只要不是标准输出）

**行缓冲**：换行时候刷新，或者缓冲区满了的时候刷新，或者强制刷新。（标准输出时行缓冲）

**无缓冲**：如`stderr`，需要立即输出的内容



getline



tmpname

tmpfile



