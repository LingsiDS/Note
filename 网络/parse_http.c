#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#define BUFFER_SIZE 40960

typedef enum {
	CHECK_STATE_REQUESTLINE = 0, //正在分析请求行
	CHECK_STATE_HEADER			 //正在分析头部字段
}CHECK_STATE;

typedef enum {
	LINE_OK = 0,	//读取到一个完整的行
	LINE_BAD,		//行出错
	LINE_OPEN		//行数据尚不完整
}LINE_STATUS;

typedef enum {
	NO_REQUEST,			//请求不完整，继续读取客户数据
	GET_REQUEST,		//获得了一个完整的客户请求
	BAD_REQUEST,		//客户请求有语法错误
	FORBIDDEN_REQUEST,	//客户对资源没有足够的访问权限
	INTERNAL_ERROR,		//服务器内部错误
	CLOSED_CONNECTION	//客户端已经关闭连接
}HTTP_CODE;

static const char* szret[] = {"I get a correct result\n", "Something wrong\n"};


//从状态机，用与解析出一行内容
LINE_STATUS parse_line(char* buffer, int& checked_index, int& read_index) {
	char temp;

	/*	checked_index是buffer中正在分析的字节，read_index为客户数据的尾部的下一字节
	buffer[0 , checked_index)字节已经分析完毕，下面逐个分析buffer[checked_index, read_index - 1]字节*/
	for (; checked_index < read_index; checked_index++) {
		temp = buffer[checked_index];
		//读到一个'\r'说明可能读取到一个完整的行
		if (temp == '\r') {
			//没有读取到一个完整的行
			if ((checked_index + 1) == read_index) {
				return LINE_OPEN;
			}
			//读取到一个完整的行
			else if (buffer[checked_index + 1] == '\n') {
				buffer[checked_index++] = '\0';
				buffer[checked_index++] = '\0';
				return LINE_OK;
			}
			//客户发送的HTTP请求存在语法问题
			return LINE_BAD;	
		}
		//
		else if (temp == '\n') {
			if (checked_index > 1 && buffer[checked_index - 1] == '\r') {
				buffer[checked_index - 1] = '\0';
				buffer[checked_index++] = '\0';
				return LINE_OK;
			}
			return LINE_BAD;
		}
		
	}
	//所有内容分析玩也没有'\r'字符，表示需要继续读取客户数据
	return LINE_OPEN;
}

//分析请求行
HTTP_CODE parse_requestline(char *temp, CHECK_STATE& checkstate) {
	//char *strpbrk(const char *str1, const char *str2)
	//依次检验字符串 str1 中的字符，当被检验字符在字符串 str2 中也包含时，则停止检验，并返回该字符位置。
	char *url = strpbrk(temp, " \t");//url定位到第一个空格的位置
	
	//请求行中没有空白字符或'\t'，则http请求必有问题
	if (!url) {
		return BAD_REQUEST;
	}

	*url++ = '\0'; //得到请求方法
	char* method = temp;
	if (strcasecmp(method, "GET") == 0) {//strncasecmp()用来比较参数s1和s2字符串前n个字符，比较时会自动忽略大小写的差异。
		printf("The request method is GET\n");
	}
	else {
		return BAD_REQUEST;
	}

	// size_t strspn(const char *str1, const char *str2) 检索字符串 str1 中第一个不在字符串 str2 中出现的字符下标。
	url += strspn(url, " \t");//去掉前导空格
	char *version = strpbrk(url, " \t");
	if (!version) {
		return BAD_REQUEST;
	}
	*version++ = '\0';
	version += strspn(version, " \t");//去掉前导空格

	//仅支持HTTP/1.1
	if (strcasecmp(version, "HTTP/1.1") != 0) { 
		return BAD_REQUEST;
	}

	//检查URL是否合法
	if (strncasecmp(url, "http://", 7) == 0) {
		url += 7;
		url = strchr(url, '/');
	}

	if (!url || url[0] != '/') {
		return BAD_REQUEST;
	}
	printf("The request URL is: %s\n", url);

	//HTTP请求行处理完毕，状态转移到头部字段的分析
	checkstate = CHECK_STATE_HEADER;
	return NO_REQUEST;
}

//分析头部字段
HTTP_CODE parse_headers(char *temp) {
	//遇到空行，说明得到一个正确的HTTP请求
	if (temp[0] == '\0') {
		return GET_REQUEST;
	}
	//处理Host头部字段
	else if (strncasecmp(temp, "Host:", 5) == 0) {
		temp += 5;
		temp += strspn(temp, " \t");
		printf("The request host is: %s\n", temp);
	}
	//其他头部字段都不处理
	else {
		printf("I can not handle this header\n");
	}
	return NO_REQUEST;
}

HTTP_CODE parse_content(char* buffer, int& checked_index, CHECK_STATE&
					checkstate, int& read_index, int& start_line) {
	LINE_STATUS linestatus = LINE_OK;	//当前行的读取状态
	HTTP_CODE retcode = NO_REQUEST;		//记录HTTP请求的处理结果

	//主状态机，用于从buffer中取出所有完整的行
	while ((linestatus = parse_line(buffer, checked_index, read_index)) == LINE_OK) {
		char *temp = buffer + start_line; //start_line是行在buffer中的起始位置
		start_line = checked_index;	  //下一行的起始位置

		switch (checkstate) {
			case CHECK_STATE_REQUESTLINE: {//请求行
				retcode = parse_requestline(temp, checkstate);
				if (retcode == BAD_REQUEST) 
					return BAD_REQUEST;
				break;
			}
			case CHECK_STATE_HEADER: {
				retcode = parse_headers(temp);
				if (retcode == BAD_REQUEST) 
					return BAD_REQUEST;
				else if (retcode == GET_REQUEST) 
					return GET_REQUEST;
				break;
			}
			default: {
				return INTERNAL_ERROR;
			}
		}
	}

	//没有读取到一个完整的行，继续读取客户数据
	if (linestatus == LINE_OPEN)
		return NO_REQUEST;
	else 
		return BAD_REQUEST;
}

int main (int argc, char *argv[]) {
	if (argc <= 1) {
		printf("Usage: %s port_number\n", argv[0]);
		return 1;
	}

	// const char* ip = argv[1];
	int port = atoi(argv[1]);

	struct sockaddr_in address;
	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;
	inet_pton(AF_INET, "0.0.0.0", &address.sin_addr);
	address.sin_port = htons(port);

	int listenfd = socket(PF_INET, SOCK_STREAM, 0);
	assert(listenfd >= 0);

	int ret = bind(listenfd, (struct sockaddr*)&address, sizeof(address));
	assert(ret != -1);

	ret = listen(listenfd, 5);
	assert(ret != -1);

	struct sockaddr_in client_address;
	socklen_t client_addrlength = sizeof(client_address);
	int fd = accept(listenfd, (struct sockaddr*)&client_address, &client_addrlength);
	if (fd < 0) {
		printf("errno is: %d\n", errno);
	}
	else {
		char buffer[BUFFER_SIZE];  	//读缓冲区
		memset(buffer, '\0', BUFFER_SIZE);
		int data_read = 0;			
		int read_index = 0;			//当前已经读取了多少字节的客户数据
		int checked_index = 0;		//当前已经分析完了多少字节的客户数据
		int start_line = 0;			//行在buffer中的起始位置

		CHECK_STATE checkstate = CHECK_STATE_REQUESTLINE;
		
		//循环读取数据并分析
		while (1) {
			data_read = recv(fd, buffer + read_index, BUFFER_SIZE - read_index, 0);
			if (data_read == -1) {
				printf("reading failed\n");
				break;
			}
			else if (data_read == 0) {
				printf("remote client has closed the connection\n");
				break;
			}
			read_index += data_read;

			//分析目前已经获得的所有客户数据
			HTTP_CODE result = parse_content(buffer, checked_index, checkstate, 
											read_index, start_line);
			//尚未读到一个完整的HTTP请求
			if (result == NO_REQUEST)
				continue;
			else if (result == GET_REQUEST) {
				send(fd, szret[0], strlen(szret[0]), 0);
				break;
			}
			else {
				send(fd, szret[1], strlen(szret[1]), 0);
				break;
			}
		}
		close(fd);
	}
	close(listenfd);
	return 0;
}
