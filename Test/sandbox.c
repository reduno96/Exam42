#include "sandbox.h"

void time_handler(int sig)
{

	(void)sig;
}
int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
	pid_t pid;
	int ex_status;
	int sig;

	pid = fork();
	if (pid == -1)
		return -1;
	if (pid == 0)
	{
		alarm(timeout);
		f();
		exit(0);
	}

	struct sigaction sa;
	sa.sa_handler = time_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGALRM, &sa, NULL);

	alarm(timeout);
	int st;
	int res_wait = waitpid(pid, &st, 0);
	if (res_wait == -1)
	{
		if (errno == EINTR)
		{
			kill(pid, SIGKILL);
			waitpid(pid, NULL, 0);
			if (verbose)
				printf("Bad function: timed out after %u seconds\n", timeout);
			return 0;
		}
		return -1;
	}
	if (WIFEXITED(st))
	{
		ex_status = WEXITSTATUS(st);
		if (ex_status == 0)
		{
			if (verbose)
				printf("Nice function!\n");
			return 1;
		}
		if (verbose)
			printf("Bad function: exited with code %d\n", ex_status);
		return 0;
	}else if (WIFSIGNALED(st))
	{
		sig = WTERMSIG(st);
		if (sig == SIGALRM)
		{
			if (verbose)
				printf("Bad function: timed out after %u seconds\n", timeout);
			return 0;
		}
		if (verbose)
			printf("Bad function: %s\n", strsignal(sig));
		return 0;
	}
	return -1;
}

