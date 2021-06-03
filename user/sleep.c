#include "kernel/types.h"
#include "user/user.h"

void slep(int t) {
	if (t < 0)
	{
		//write(0, "error: no argument\n", 19);
		fprintf(2, "error: no argument\n");
		exit(1);
	}
	sleep(t*10);
	write(0, "(nothing happens for a little while)\n", 37);
	exit(0);
}

int main(int argc, char *argv[])
{
	if (argc <= 1) 
	{
		slep(-1);
		exit(0);
	} 
	else 
	{
		int t = atoi(argv[1]);
		slep(t);
	}
	exit(0);
}
