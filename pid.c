#include <unistd.h>
#include <stdio.h>
int main(void)
{
	pid_t	pid;
	pid = getpid();
	printf("PID: %d\n", pid);

	pid_t	ppid;
	ppid = getppid();
	printf("PPID: %d\n", ppid);

	uid_t	uid;
	uid = getuid();
	printf("UID: %d\n", uid);

	uid_t	euid;
	euid = geteuid();
	printf("EUID: %d\n", euid);
	
	gid_t	gid;
	gid = getgid();
	printf("GID: %d\n", gid);

	gid_t	egid;
	egid = getegid();
	printf("EGID: %d\n", egid);
	return 0;
}
