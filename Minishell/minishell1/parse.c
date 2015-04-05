#include <unistd.h>
#include <stdio.h>
#include "parse.h"
#include <sys/wait.h>
#include <sys/types.h>
#include "externs.h"
#include "init.h"

void get_command(int i);
int check(const char *str);
void getname(char *name);
//shell main loop
void shell_loop(void)
{
	while(1) {
		printf("[minishell]$ ");
		fflush(stdout);
		init();
		if (read_command() == -1)
			break;
		parse_command();
		execute_command();
	}
	printf("\n exit \n");
}

//shell read command
//return 0 if OK else return -1
int read_command(void)
{
	if (fgets(cmdline, MAXLINE, stdin) == NULL)
		return -1;
	return 0;	
}


//parse command
//return the number of commands, if failed return -1
int parse_command(void)
{
/*	char *cp = cmdline;	
	char *avp = avline;
	int i = 0;
	while (*cp != '\0')
	{
		while (*cp == ' ' || *cp == '\t')
			cp++;
		if (*cp == '\0' || *cp == '\n')
			break;
		cmd.args[i] = avp;
		while (*cp != '\0' && *cp != ' ' && *cp != '\t'
						&& *cp != '\n')
		{
			*avp++ = *cp++;
		}
		*avp ++ = '\0';
		printf("[%s]\n", cmd.args[i]);
		i++;
	}
	return 0;*/
	get_command(0);
	
	if (check("<"))
	{
		getname(infile);
	}

	int i;
	for (i = 1; i < PIPELINE; ++i)
	{
		if (check("|"))
		{
			get_command(i);
		}
		else
			break;
	}

	if (check(">"))
		getname(outfile);
	
	if (check("&"))
		backgnd = 1;
	
	if (check("\n"))
	{
		cmd_count = i;
		return cmd_count;
	}
	else 
	{
		fprintf(stderr, "Command line syntax error\n");
		return -1;
	}
}


//execute commands
//return 0 if succeed, if failed return -1
int execute_command(void)
{
/*
	pid_t pid = fork();
	if (pid == 0)
		execvp(cmd.args[0], cmd.args);
	wait(NULL);*/
	return 0;

}



void get_command(int i)
{
	int j = 0;
	int inword;
	while (*lineptr != '\0')
	{
		while (*lineptr == ' ' || *lineptr == '\t')
		{
			*lineptr++;
		}
		cmd[i].args[j] = avptr;
		while(*lineptr != '\0' && *lineptr != ' '
				&& *lineptr != '\t' && *lineptr != '>'
				&& *lineptr != '<'  && *lineptr != '|'
				&& *lineptr != '&'  && *lineptr != '\n')
		{
			*avptr++ = *lineptr++;
			inword = 1;
		}
		*avptr++ = '\0';
		switch(*lineptr)
		{
		case ' ':
		case '\t':
			inword = 0;
			j++;
			break;
		case '<':
		case '>':
		case '|':
		case '&':
		case '\n':
			if (inword == 0)
				cmd[i].args[j] = NULL;
			return;
		default:			//for \0
			return;
		}
	}
}

int check(const char *str)
{
	return 0;
}

void getname(char *name)
{
	
}
