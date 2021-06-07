#include "kernel/types.h"
#include "user/user.h"

void slep(int t) {
	if (t < 0)
	{
		exit(1);
	}
	sleep(t);
	exit(0);
}

int main(int argc, char *argv[])
{
	if (argc <= 1) 
	{
		fprintf(2, "invalid input");
		exit(1);
	} 
	else 
	{
		int t = atoi(argv[1]);
		slep(t);
		write(0, "(nothing happens for a little while)\n", 37);
	}
	exit(0);
}
