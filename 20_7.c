
#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
int main()
{
	int n, nSocket;
	char buf[21];
	struct sockaddr_in sin;
	if ((nSocket = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		fprintf(stderr, "socket error:%s\n", strerror(errno));
		exit(0);
	}
	bzero(&sin, sizeof(struct sockaddr_in));
	if (inet_pton(AF_INET, "127.0.0.1", &(sin.sin_addr)) != 1)
	{
		fprintf(stderr, "inet_pton error:%s", strerror(errno));
		exit(0);
	}
	sin.sin_family = AF_INET;
	sin.sin_port = htons(13);
	if (connect(nSocket, (struct sockaddr *)&sin, sizeof(struct sockaddr_in)) < 0)
	{
		fprintf(stderr, "connect error:%s", strerror(errno));
		exit(0);
	}
	memset(buf, 0x00, sizeof(buf));
	n = recv(nSocket, buf, 20, 0);
	printf("%s\n", buf);
	close(nSocket);
	return 0;
}
