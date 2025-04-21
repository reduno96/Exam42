#include "sandbox.h"

// ---- NICE FUNCTIONS ----

// Simple function that returns normally
void nice_function(void)
{
	printf("This is a nice function that returns normally\n");
}

// Function that does some work but finishes in time
void nice_function_with_work(void)
{
	printf("Working...\n");
	for (int i = 0; i < 3; i++)
	{
		printf("Step %d completed\n", i + 1);
		sleep(1);
	}
	printf("Work completed successfully\n");
}

// ---- BAD FUNCTIONS ----

// Function that segfaults
void bad_function_segfault(void)
{
	int *ptr;

	printf("About to cause a segmentation fault...\n");
	ptr = NULL;
	*ptr = 42; // This will cause a segmentation fault
}

// Function that aborts
void bad_function_abort(void)
{
	printf("About to abort...\n");
	abort();
}

// Function that exits with non-zero status
void bad_function_exit_code(void)
{
	printf("About to exit with code 42...\n");
	exit(42);
}

// Function that times out
void bad_function_timeout(void)
{
	printf("About to enter an infinite loop...\n");
	while (1)
	{
		sleep(1);
		printf("Still running...\n");
	}
}

// Function that raises a signal
void bad_function_signal(void)
{
	printf("About to raise SIGILL...\n");
	raise(SIGILL); // Illegal instruction
}

// Function that does nothing and returns
void nice_function_empty(void)
{
	// no-op
}

// Function that prints a lot quickly
void nice_function_fast_output(void)
{
	for (int i = 0; i < 1000; i++)
		printf("Output line %d\n", i);
}

// Function that sleeps less than timeout
void nice_function_sleep_short(void)
{
	sleep(2); // Should succeed if timeout > 2
}
// Function that causes division by zero
void bad_function_divzero(void)
{
	int x;

	x = 1 / 0;
	(void)x;
}

// Function that raises SIGFPE (floating point exception)
void bad_function_fpe(void)
{
	raise(SIGFPE);
}

// Function that causes stack overflow (infinite recursion)
void bad_function_stack_overflow(void)
{
	bad_function_stack_overflow();
}

// Main function to demonstrate sandbox
int main(int argc, char *argv[])
{
	int result;
	bool verbose;

	verbose = true;
	(void)argc;
	(void)argv;
	printf("\n=== Testing nice functions ===\n");
	printf("\n1. Testing nice_function:\n");
	result = sandbox(nice_function, 5, verbose);
	printf("Result: %d (expected: 1)\n", result);
	printf("\n2. Testing nice_function_with_work:\n");
	result = sandbox(nice_function_with_work, 5, verbose);
	printf("Result: %d (expected: 1)\n", result);
	printf("\n=== Testing bad functions ===\n");
	printf("\n3. Testing bad_function_segfault:\n");
	result = sandbox(bad_function_segfault, 5, verbose);
	printf("Result: %d (expected: 0)\n", result);
	printf("\n4. Testing bad_function_abort:\n");
	result = sandbox(bad_function_abort, 5, verbose);
	printf("Result: %d (expected: 0)\n", result);
	printf("\n5. Testing bad_function_exit_code:\n");
	result = sandbox(bad_function_exit_code, 5, verbose);
	printf("Result: %d (expected: 0)\n", result);
	printf("\n6. Testing bad_function_timeout:\n");
	result = sandbox(bad_function_timeout, 3, verbose);
	printf("Result: %d (expected: 0)\n", result);
	printf("\n7. Testing bad_function_signal:\n");
	result = sandbox(bad_function_signal, 5, verbose);
	printf("Result: %d (expected: 0)\n", result);
	printf("\n8. Testing nice_function_empty:\n");
	result = sandbox(nice_function_empty, 5, verbose);
	printf("Result: %d (expected: 1)\n", result);
	printf("\n9. Testing nice_function_fast_output:\n");
	result = sandbox(nice_function_fast_output, 5, verbose);
	printf("Result: %d (expected: 1)\n", result);
	printf("\n10. Testing nice_function_sleep_short:\n");
	result = sandbox(nice_function_sleep_short, 3, verbose);
	printf("Result: %d (expected: 1)\n", result);
	printf("\n11. Testing bad_function_divzero:\n");
	result = sandbox(bad_function_divzero, 5, verbose);
	printf("Result: %d (expected: 0)\n", result);
	printf("\n12. Testing bad_function_fpe:\n");
	result = sandbox(bad_function_fpe, 5, verbose);
	printf("Result: %d (expected: 0)\n", result);
	printf("\n13. Testing bad_function_stack_overflow:\n");
	result = sandbox(bad_function_stack_overflow, 5, verbose);
	printf("Result: %d (expected: 0)\n", result);
	printf("\n14. NULL NULL:\n");
	result = sandbox(NULL, 5, verbose);
	printf("Result: %d (expected: 0)\n", result);
	return (0);
}


/*


// Test functions
void nice_func(void) { printf("Nice function executed.\n"); } // Should return 1
void exit_42(void) { exit(42); }							  // Should return 0
void segfault(void) { *(volatile int *)0 = 0; }				  // Should return 0 (SIGSEGV)
void infinite_loop(void)
{
	while (1)
		usleep(1000);
} // Should timeout

int main()
{
	printf("Testing sandbox function:\n");

	// Test cases
	struct
	{
		const char *name;
		void (*func)(void);
		int expected;
		unsigned int timeout;
	} tests[] = {
		{"Nice function", nice_func, 1, 3},
		{"Exit code 42", exit_42, 0, 3},
		{"Segmentation fault", segfault, 0, 3},
		{"Infinite loop", infinite_loop, 0, 3},
	};

	for (size_t i = 0; i < sizeof(tests) / sizeof(tests[0]); i++)
	{
		printf("\n\033[1m[%s]\033[0m\n", tests[i].name);
		int result = sandbox(tests[i].func, tests[i].timeout, true);
		printf("Expected: %d | Actual: %d → %s\n",
			   tests[i].expected, result,
			   result == tests[i].expected ? "\033[32mPASS\033[0m" : "\033[31mFAIL\033[0m");
	}

	return 0;
}
*/
