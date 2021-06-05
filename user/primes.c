#include "kernel/types.h"
#include "user/user.h"

void primes(int fd)
{
	int first, second;
	if (read(fd, &first, 4) == 4)
	{
		// get first readed number t	
		printf("prime %d\n", first);
		// if count of read >= 2, should have a new process	
		if (read(fd, &second, 4) != 0)
		{
			int p2[2];
			pipe(p2);
			if (fork() == 0) // child process
			{
				close(p2[1]);
				primes(p2[0]);
				close(p2[0]);
			}
			else // parent process
			{
				close(p2[0]);
				int k; // store the third... read number
				if (second % first != 0) 
				{
					write(p2[1], &second, 4);
				}
				while (read(fd, &k, 4) != 0)
				{
					//printf("k is %d\n", k);
					if (k % first != 0)
					{
						write(p2[1], &k, 4);
					}
				}
				close(p2[1]);
				wait(0);
			}
		}
		exit(0);
	}
	else 
	{
		fprintf(2, "invalid input");
		exit(1);
	}
	
}


int main(int argc, char *argv)
{
	if (argc > 1) 
	{
		fprintf(2, "too much argument");
		exit(1);
	}
	int p[2];
	pipe(p);
	if (fork() != 0)
	{
		close(p[0]);
		for (int i = 2; i <= 35; i++)
		{
			write(p[1], &i, 4);
		}
		close(p[1]);
		wait(0);
	}
	else 
	{
		close(p[1]);
		primes(p[0]);
		close(p[0]);
	}
	exit(0);
}
