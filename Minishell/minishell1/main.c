#include "parse.h"
#include "init.h"
#include "def.h"


char cmdline[MAXLINE + 1];
COMMAND cmd[PIPELINE];
char avline[MAXLINE + 1];

char *lineptr;
char *avptr;
char infile[MAXNAME + 1];
char outfile[MAXNAME + 1];
int cmd_count = 0;
int backgnd;


int main()
{
	setup();
	shell_loop();
	return 0;
}
