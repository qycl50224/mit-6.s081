#include "kernel/types.h"
#include "user/user.h"


void pingpong()
{
	int p_c[2];
	int c_p[2];
	char buf_p[5];
	char buf_c[5];

	pipe(p_c);
	pipe(c_p);
	int pid = fork();
	if (pid == 0)
	{
		close(c_p[0]);
		close(p_c[1]);
		if (read(p_c[0], buf_c, 1) != 1)
		{
			fprintf(2, "read error\n");
			exit(1);
		}
		close(p_c[0]);
		printf("%d: received ping\n" , getpid()); 
		if (write(c_p[1], "\n", 1) != 1)
		{
			fprintf(2, "write error\n");
			exit(1);
		}
		close(c_p[1]);
		exit(0);
	}
	else 
	{
		close(c_p[1]);
		close(p_c[0]);
		write(p_c[1], "\n", 1);
		close(p_c[1]);
		if (read(c_p[0], buf_p, 1) != 1)
		{
			fprintf(2, "read error\n");
			exit(1);
		}
		close(c_p[0]);
		printf("%d: received pong\n" , getpid());
		exit(0);
	}
}
