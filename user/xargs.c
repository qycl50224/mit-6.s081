#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

// 1.get input 
// 2.return the end position of new_argv
int read_line(int endbit, char **new_argv)
{
	char *buf = malloc(256);
	char *p = buf;
	while (read(0, p, 1) != 0)
	{
		if (*p == '\n')
		{
			*p = 0;
			break;	
		}
		p++;
	}
	// no input
	if (p == buf) return 0;
	char *s = buf;
	// handle space ' ' and insert new arg into new_argv
	while (s != p)
	{
		new_argv[endbit++] = s;
		while (*s != ' ' && s != p)
		{
			s++;
		}
		while (*s == ' ' && s != p)
		{
			*s = 0;
			s++;
		}
	}
	free(buf);
	return endbit;
}


int main(int argc, char *argv[])
{
	if (argc <= 1)
	{
		fprintf(2, "invalid input");
		exit(1);
	}
	
	char *cmd = malloc(strlen(argv[1]+1));
	char *new_argv[MAXARG];
	// get cmd
	cmd = argv[1];
	for (int i = 1; i < argc; i++)
	{
		new_argv[i-1] = malloc(strlen(argv[i]+1));
		new_argv[i-1] = argv[i];
	}

	int end_bit; // end new_argv cuz we set it's size as MAXARG 
	while ((end_bit = read_line(argc-1, new_argv)) != 0)
	{
		new_argv[end_bit] = 0;
		if (fork() == 0)
		{
			exec(cmd, new_argv);
			exit(0);
		}
		else 
		{
			wait(0);
		}
	}
	exit(0);


}
