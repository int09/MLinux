/*
struct sockaddr_in		//Internet地址族
{
	__SOCKADDR_COMMON(sin_);
	in_port_t sin_port;	//端口号
	struct in_addr sin_addr	//IP地址, uint32
	unsigned char sin_zero[sizeof(struct sockaddr) - 
				__SOCKADDR_COMMON_SIZE 
};


struct hostent
{
	char *h_name;		//主机名称
	char **h_aliases;	//主机别名列表
	int h_addrtype;		//主机地址类型，在Internet地址族下一般为AF——INET
	int h_length;		//地址字节长度
	char **h_addr_list;	//包含该主机的所有地址
#define h_addr h_addr_list[0]
};
*/
/*
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
int main()
{
	struct servent *s;
	if ((s = getservbyname("Telnet", "tcp")) == NULL) {
		fprintf(stderr, "getservbyname failed:%s\n", strerror(errno));
		exit(0);
	}
	fprintf(stderr, "Port:%d\n", ntohs(s->s_port));
	return 0;
}


*/
/*
#include <sys/socket.h>
int socket(int __domain, int __type, int __protocol);
//__domain协议族名称，在Internet协议族中使用宏定义PF_INET
//__type套接字类型:SOCK_STREAM/ SOCK_DGRAM/ SOCK_RAW
//__protocol协议类型
//return:非零为套接字描述符，-1表示出错
int soc;
soc = socket(PF_INET, SOCK_STREAM, 0);

int bind(int __fd, __CONST_SOCKADDR_ARG __addr, socklen_t __len);
//__fd套接字描述符，由socket调用后返回
//__addr指向通用套接字地址结构的指针
//__CONST_SOCKADDR_ARG  ->  const struct sockaddr *
//__len地址结构长度
*/
#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
//form 点分十进制 to 二进制
/*
inte_addr / inet_aton / inet_atop
建议使用	inet_aton
*/

//form 二进制 to 点分十进制
/*
inet_ntoa / inet_ntop
*/

//in_addr_t inet_aton(__const char *__cp, struct in_addr *__inp);

int main()
{
	int nSocket;
	struct sockaddr_in sin;
	if (nSocket = socket(PF_INET, SOCK_STREAM, 0) < 0) {
		fprintf(stderr, "socket error: %s", strerror(errno));
		exit(0);
	}//创建套接字
	bzero(&sin, sizeof(struct sockaddr_in));//清空sin结构
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(5500);
	if (bind(nSocket, (struct sockaddr *)&sin, sizeof(struct sockaddr_in)) < 0) {
		fprintf(stderr, "bind error :%s", strerror(errno));
		close(nSocket);
		exit(0);
	}
	printf("Bind Success\n");
	close(nSocket);
	return 0;
}






















