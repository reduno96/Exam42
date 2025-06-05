
#include "picoshell.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int ret;

	// // Normal tests (8 tests)
	// printf("=== NORMAL TESTS ===\n");

	// // Test 1: Single command
	// char *cmd1[] = {"/bin/echo", "hello world", NULL};
	// char **pipeline1[] = {cmd1, NULL};
	// printf("Test 1: Single command (echo)\n");
	// ret = picoshell(pipeline1);
	// printf(ret == 0 ? "Test 1 passed\n\n" : "Test 1 failed\n\n");

	// // Test 2: Simple pipeline
	// char *cmd2[] = {"/bin/ls", NULL};
	// char *cmd3[] = {"/usr/bin/grep", "picoshell", NULL};
	// char **pipeline2[] = {cmd2, cmd3, NULL};
	// printf("Test 2: /bin/ls | grep picoshell\n");
	// ret = picoshell(pipeline2);
	// printf(ret == 0 ? "Test 2 passed\n\n" : "Test 2 failed\n\n");

	// // Test 3: Three-command pipeline
	// char *cmd4[] = {"/bin/echo", "squalala", NULL};
	// char *cmd5[] = {"/bin/cat", NULL};
	// char *cmd6[] = {"/bin/sed", "s/a/b/g", NULL};
	// char **pipeline3[] = {cmd4, cmd5, cmd6, NULL};
	// printf("Test 3: echo squalala | cat | sed\n");
	// ret = picoshell(pipeline3);
	// printf(ret == 0 ? "Test 3 passed\n\n" : "Test 3 failed\n\n");

	// // Test 4: Empty pipeline (should do nothing)
	// char **pipeline4[] = {NULL};
	// printf("Test 4: Empty pipeline\n");
	// ret = picoshell(pipeline4);
	// printf(ret == 0 ? "Test 4 passed\n\n" : "Test 4 failed\n\n");

	// // Test 5: Simple grep test
	// char *cmd7[] = {"/bin/echo", "Makefile", NULL};
	// char *cmd8[] = {"/usr/bin/grep", "Makefile", NULL};
	// char **pipeline5[] = {cmd7, cmd8, NULL};
	// printf("Test 5: /bin/echo \"Makefile\" | grep Makefile\n");
	// ret = picoshell(pipeline5);
	// printf(ret == 0 ? "Test 5 passed\n\n" : "Test 5 failed\n\n");

	// // Test 6: Multi-stage transformation
	// char *cmd9[] = {"/bin/echo", "a b c", NULL};
	// char *cmd10[] = {"/usr/bin/tr", " ", "\n", NULL};
	// char *cmd11[] = {"/usr/bin/grep", "b", NULL};
	// char *cmd12[] = {"/bin/sed", "s/b/B/g", NULL};
	// char **pipeline6[] = {cmd9, cmd10, cmd11, cmd12, NULL};
	// printf("Test 6: Multi-stage transformation\n");
	// ret = picoshell(pipeline6);
	// printf(ret == 0 ? "Test 6 passed\n\n" : "Test 6 failed\n\n");

	// // Test 7: High-volume data test
	// char *cmd13[] = {"/usr/bin/yes", "test", NULL};
	// char *cmd14[] = {"/usr/bin/head", "-n", "1000", NULL};
	// char *cmd15[] = {"/usr/bin/wc", "-l", NULL};
	// char **pipeline7[] = {cmd13, cmd14, cmd15, NULL};
	// printf("Test 7: High-volume data test\n");
	// ret = picoshell(pipeline7);
	// printf(ret == 0 ? "Test 7 passed\n\n" : "Test 7 failed\n\n");

	// // Test 8: Command with arguments containing spaces
	// char *cmd16[] = {"/bin/echo", "hello  world", NULL};
	// char *cmd17[] = {"/bin/grep", "hello", NULL};
	// char **pipeline8[] = {cmd16, cmd17, NULL};
	// printf("Test 8: Command with spaced arguments\n");
	// ret = picoshell(pipeline8);
	// printf(ret == 0 ? "Test 8 passed\n\n" : "Test 8 failed\n\n");

	// // Error detection tests (8 tests)
	// printf("=== ERROR DETECTION TESTS ===\n");

	
	// // Test 10: Invalid command
	// char *cmd18[] = {"ls", NULL};
	// char **pipeline9[] = {cmd18, NULL};
	// printf("Test 10: Invalid command: /invalid/command\n");
	// ret = picoshell(pipeline9);
	// printf(ret != 0 ? "Test 10 passed (error detected as expected)\n\n" : "Test 10 failed\n\n");

	// // Test 11: Pipeline with an invalid middle command
	// char *cmd19[] = {"/bin/echo", "hello", NULL};
	// char *cmd20[] = {"/invalid/command", NULL};
	// char *cmd21[] = {"/bin/cat", NULL};
	// char **pipeline10[] = {cmd19, cmd20, cmd21, NULL};
	// printf("Test 11: Pipeline with an invalid middle command\n");
	// ret = picoshell(pipeline10);
	// printf(ret != 0 ? "Test 11 passed (error detected as expected)\n\n" : "Test 11 failed\n\n");

	// // Test 12: Command with NULL as executable
	// char *cmd22[] = {NULL, NULL};
	// char **pipeline11[] = {cmd22, NULL};
	// printf("Test 12: Command with NULL executable\n");
	// ret = picoshell(pipeline11);
	// printf(ret != 0 ? "Test 12 passed (error detected as expected)\n\n" : "Test 12 failed\n\n");

	// // Test 13: Empty command array
	// char *cmd23[] = {NULL};
	// char **pipeline12[] = {cmd23, NULL};
	// printf("Test 13: Empty command array\n");
	// ret = picoshell(pipeline12);
	// printf(ret != 0 ? "Test 13 passed (error detected as expected)\n\n" : "Test 13 failed\n\n");

	// // Test 14: Non-existent command in the middle of arguments
	// char *cmd24[] = {"/bin/ls", "/nonexistent/file", NULL};
	// char **pipeline13[] = {cmd24, NULL};
	// printf("Test 14: Command with non-existent argument\n");
	// ret = picoshell(pipeline13);
	// printf(ret != 0 ? "Test 14 passed (error detected as expected)\n\n" : "Test 14 failed\n\n");

	// // Test 15: Command that fails (false command)
	// char *cmd25[] = {"/bin/false", NULL};
	// char **pipeline14[] = {cmd25, NULL};
	// printf("Test 15: Command that returns non-zero (false)\n");
	// ret = picoshell(pipeline14);
	// printf(ret != 0 ? "Test 15 passed (error detected as expected)\n\n" : "Test 15 failed\n\n");

	// Test 16: Pipeline with permission denied command
	char *cmd25[] = {"lsa", "-al", NULL};
	char *cmd27[] = {"cat","-e" ,NULL};
	char *cmd26[] = {"lsa", "-al", NULL};
	char **pipeline15[] = {cmd26, cmd27, NULL};
	
	ret = picoshell(pipeline15);
	printf("_____%d\n", ret);
	
	return (0);
}

