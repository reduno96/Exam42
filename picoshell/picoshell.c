/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picoshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rel-mora						            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:02:39 by rel-mora          #+#    #+#             */
/*   Updated: 2025/04/12 16:03:49 by motivred         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "picoshell.h"

int picoshell(char **cmds[])
{
	int count = 0;
	int pre_fd[2] = {-1, -1};
	pid_t pid;
	int status;
	int res = 0;
	if (!cmds)
		return 1;
	while (cmds[count])
		count++;
	for (int i = 0; i < count; i++)
	{
		int cur_fd[2] = {-1, -1};
		if (i < count - 1)
		{
			if (pipe(cur_fd) == -1)
				return (1);
		}
		pid = fork();
		if (pid == -1)
		{
			if (i > 0)
			{
				close(pre_fd[0]);
				close(pre_fd[1]);
			}
			if (i < count - 1)
			{
				close(cur_fd[0]);
				close(cur_fd[1]);
			}
			return (1);
		}
		if (pid == 0)
		{
			if (i > 0)
			{
				if (dup2(pre_fd[0], STDIN_FILENO) == -1)
					exit(1);
				if (close(pre_fd[0]) == -1 || close(pre_fd[1]) == -1)
					exit(1);
			}
			if (i < count - 1)
			{
				if (dup2(cur_fd[1], STDOUT_FILENO) == -1)
					exit(1);
				if (close(cur_fd[0]) == -1 || close(cur_fd[1]) == -1)
					exit(1);
			}

			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}
		if (pre_fd[0] != -1)
		{
			if (close(pre_fd[0]) == -1 || close(pre_fd[1]) == -1)
				return (1);
		}
		pre_fd[0] = cur_fd[0];
		pre_fd[1] = cur_fd[1];
	}
	while (wait(&status) > 0)
	{
		if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
			res = 1;
	}
	return res;
}

#include "picoshell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#include <string.h>

// Function to count open file descriptors for current process
int count_open_fds()
{
	DIR *dir = opendir("/proc/self/fd");
	if (!dir)
		return -1;

	int count = 0;
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL)
	{
		if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
		{
			count++;
		}
	}
	closedir(dir);
	return count;
}

// Function to check for zombie processes
int count_zombie_processes()
{
	int zombies = 0;
	pid_t pid;
	int status;

	while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
	{
		if (WIFEXITED(status) || WIFSIGNALED(status))
		{
			zombies++;
		}
	}
	return zombies;
}

void run_leak_test(const char *description, char ***pipeline, int expect_error)
{
	int initial_fds = count_open_fds();
	int initial_zombies = count_zombie_processes();

	printf("=== %s ===\n", description);
	int ret = picoshell(pipeline);

	// Check return value
	if ((expect_error && ret == 0) || (!expect_error && ret != 0))
	{
		printf("FAIL: Incorrect return value\n");
		return;
	}

	// Check file descriptors
	int final_fds = count_open_fds();
	if (final_fds != initial_fds)
	{
		printf("FAIL: File descriptor leak detected (%d -> %d)\n", initial_fds, final_fds);
		return;
	}

	// Check zombie processes
	int final_zombies = count_zombie_processes();
	if (final_zombies != initial_zombies)
	{
		printf("FAIL: Zombie processes detected (%d -> %d)\n", initial_zombies, final_zombies);
		return;
	}

	printf("PASS: No leaks and %s\n\n", expect_error ? "error detected as expected" : "successful execution");
}
int main(void)
{
	// Normal tests (8 tests)
	printf("=== NORMAL TESTS (WITH LEAK CHECKS) ===\n");

	// Test 1: Single command
	char *cmd1[] = {"/bin/echo", "hello world", NULL};
	char **pipeline1[] = {cmd1, NULL};
	run_leak_test("Test 1: Single command (echo)", pipeline1, 0);

	// Test 2: Simple pipeline
	char *cmd2[] = {"/bin/ls", NULL};
	char *cmd3[] = {"/usr/bin/grep", "picoshell", NULL};
	char **pipeline2[] = {cmd2, cmd3, NULL};
	run_leak_test("Test 2: /bin/ls | grep picoshell", pipeline2, 0);

	// Test 3: Three-command pipeline
	char *cmd4[] = {"/bin/echo", "squalala", NULL};
	char *cmd5[] = {"/bin/cat", NULL};
	char *cmd6[] = {"/bin/sed", "s/a/b/g", NULL};
	char **pipeline3[] = {cmd4, cmd5, cmd6, NULL};
	run_leak_test("Test 3: echo squalala | cat | sed", pipeline3, 0);

	// Test 4: Empty pipeline (should do nothing)
	char **pipeline4[] = {NULL};
	run_leak_test("Test 4: Empty pipeline", pipeline4, 0);

	// Test 5: Simple grep test
	char *cmd7[] = {"/bin/echo", "Makefile", NULL};
	char *cmd8[] = {"/usr/bin/grep", "Makefile", NULL};
	char **pipeline5[] = {cmd7, cmd8, NULL};
	run_leak_test("Test 5: /bin/echo \"Makefile\" | grep Makefile", pipeline5, 0);

	// Test 6: Multi-stage transformation
	char *cmd9[] = {"/bin/echo", "a b c", NULL};
	char *cmd10[] = {"/usr/bin/tr", " ", "\n", NULL};
	char *cmd11[] = {"/usr/bin/grep", "b", NULL};
	char *cmd12[] = {"/bin/sed", "s/b/B/g", NULL};
	char **pipeline6[] = {cmd9, cmd10, cmd11, cmd12, NULL};
	run_leak_test("Test 6: Multi-stage transformation", pipeline6, 0);

	// Test 7: High-volume data test
	char *cmd13[] = {"/usr/bin/yes", "test", NULL};
	char *cmd14[] = {"/usr/bin/head", "-n", "1000", NULL};
	char *cmd15[] = {"/usr/bin/wc", "-l", NULL};
	char **pipeline7[] = {cmd13, cmd14, cmd15, NULL};
	run_leak_test("Test 7: High-volume data test", pipeline7, 0);

	// Test 8: Command with arguments containing spaces
	char *cmd16[] = {"/bin/echo", "hello  world", NULL};
	char *cmd17[] = {"/bin/grep", "hello", NULL};
	char **pipeline8[] = {cmd16, cmd17, NULL};
	run_leak_test("Test 8: Command with spaced arguments", pipeline8, 0);

	// Error detection tests (8 tests)
	printf("=== ERROR DETECTION TESTS (WITH LEAK CHECKS) ===\n");

	// Test 9: NULL pipeline
	run_leak_test("Test 9: NULL pipeline", NULL, 1);

	// Test 10: Invalid command
	char *cmd18[] = {"/invalid/command", NULL};
	char **pipeline9[] = {cmd18, NULL};
	run_leak_test("Test 10: Invalid command: /invalid/command", pipeline9, 1);

	// Test 11: Pipeline with an invalid middle command
	char *cmd19[] = {"/bin/echo", "hello", NULL};
	char *cmd20[] = {"/invalid/command", NULL};
	char *cmd21[] = {"/bin/cat", NULL};
	char **pipeline10[] = {cmd19, cmd20, cmd21, NULL};
	run_leak_test("Test 11: Pipeline with an invalid middle command", pipeline10, 1);

	// Test 12: Command with NULL as executable
	char *cmd22[] = {NULL, NULL};
	char **pipeline11[] = {cmd22, NULL};
	run_leak_test("Test 12: Command with NULL executable", pipeline11, 1);

	// Test 13: Empty command array
	char *cmd23[] = {NULL};
	char **pipeline12[] = {cmd23, NULL};
	run_leak_test("Test 13: Empty command array", pipeline12, 1);

	// Test 14: Non-existent command in the middle of arguments
	char *cmd24[] = {"/bin/ls", "/nonexistent/file", NULL};
	char **pipeline13[] = {cmd24, NULL};
	run_leak_test("Test 14: Command with non-existent argument", pipeline13, 1);

	// Test 15: Command that fails (false command)
	char *cmd25[] = {"/bin/false", NULL};
	char **pipeline14[] = {cmd25, NULL};
	run_leak_test("Test 15: Command that returns non-zero (false)", pipeline14, 1);

	// Test 16: Pipeline with permission denied command
	char *cmd26[] = {"/etc/shadow", NULL};
	char **pipeline15[] = {cmd26, NULL};
	run_leak_test("Test 16: Permission denied command", pipeline15, 1);

	// Test 17: Invalid pipeline with typo in command
	char *cmd27[] = {"/bin/ls", "-a", NULL};
	char *cmd28[] = {"/usr/bin/grep", "file", NULL}; // Intentional typo in 'grep'
	char **pipeline16[] = {cmd27, cmd28, NULL};
	run_leak_test("Test 17: ls -a | grep file (not found, expected 1)", pipeline16, 1);

	return 0;
}

