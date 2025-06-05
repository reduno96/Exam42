#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/wait.h>

extern int ft_popen(const char *file, char *const argv[], char type);

int count_open_fds()
{
    DIR *dir = opendir("/proc/self/fd");
    if (!dir)
        return -1;

    int count = 0;
    struct dirent *entry;
    while ((entry = readdir(dir)))
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
            count++;
    }
    closedir(dir);
    return count;
}

void test_read_mode()
{
    int initial_fds = count_open_fds();

    int fd = ft_popen("echo", (char *const[]){"echo", "hello", NULL}, 'r');
    if (fd == -1)
    {
        perror("test_read_mode: ft_popen failed");
        exit(EXIT_FAILURE);
    }

    char buf[16];
    ssize_t bytes = read(fd, buf, sizeof(buf) - 1);
    close(fd);

    if (bytes != 6 || strcmp(buf, "hello\n") != 0)
    {
        printf("FAIL: test_read_mode - Expected 'hello\\n', got '%.*s'\n", (int)bytes, buf);
        exit(EXIT_FAILURE);
    }

    if (count_open_fds() != initial_fds)
    {
        printf("FAIL: test_read_mode - File descriptor leak detected!\n");
        exit(EXIT_FAILURE);
    }

    printf("PASS: test_read_mode\n");
}

// Test 2: Write mode (send input to 'cat' and capture output)
void test_write_mode()
{
    int initial_fds = count_open_fds();

    // Create a pipe to capture stdout
    int stdout_pipe[2];
    pipe(stdout_pipe);
    int saved_stdout = dup(STDOUT_FILENO);
    dup2(stdout_pipe[1], STDOUT_FILENO);
    close(stdout_pipe[1]);

    // Write to cat via ft_popen
    int fd = ft_popen("cat", (char *const[]){"cat", NULL}, 'w');
    if (fd == -1)
    {
        perror("test_write_mode: ft_popen failed");
        exit(EXIT_FAILURE);
    }

    write(fd, "test\n", 5);
    close(fd);

    // Restore stdout and read captured output
    fflush(stdout);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);

    char buf[16];
    ssize_t bytes = read(stdout_pipe[0], buf, sizeof(buf));
    close(stdout_pipe[0]);

    // Use memcmp to compare exact bytes
    if (bytes != 5 || memcmp(buf, "test\n", 5) != 0)
    {
        printf("FAIL: test_write_mode - Expected 'test\\n', got '%.*s'\n", (int)bytes, buf);
        exit(EXIT_FAILURE);
    }

    if (count_open_fds() != initial_fds)
    {
        printf("FAIL: test_write_mode - File descriptor leak detected!\n");
        exit(EXIT_FAILURE);
    }

    printf("PASS: test_write_mode\n");
}

// Test 3: Invalid parameters
void test_invalid_params()
{
    int fd;

    // NULL file
    fd = ft_popen(NULL, (char *const[]){"ls", NULL}, 'r');
    if (fd != -1)
    {
        printf("FAIL: test_invalid_params - NULL file did not return -1\n");
        exit(EXIT_FAILURE);
    }

    // NULL argv
    fd = ft_popen("ls", NULL, 'r');
    if (fd != -1)
    {
        printf("FAIL: test_invalid_params - NULL argv did not return -1\n");
        exit(EXIT_FAILURE);
    }

    // Invalid type
    fd = ft_popen("ls", (char *const[]){"ls", NULL}, 'x');
    if (fd != -1)
    {
        printf("FAIL: test_invalid_params - Invalid type did not return -1\n");
        exit(EXIT_FAILURE);
    }

    printf("PASS: test_invalid_params\n");
}

// Test 4: Command not found
void test_cmd_not_found()
{
    int initial_fds = count_open_fds();

    int fd = ft_popen("nonexistentcmd", (char *const[]){"nonexistentcmd", NULL}, 'r');
    if (fd == -1)
    {
        printf("FAIL: test_cmd_not_found - ft_popen failed unexpectedly\n");
        exit(EXIT_FAILURE);
    }

    char buf[16];
    ssize_t bytes = read(fd, buf, sizeof(buf));
    close(fd);

    if (bytes != 0)
    {
        printf("FAIL: test_cmd_not_found - Expected EOF, got %zd bytes\n", bytes);
        exit(EXIT_FAILURE);
    }

    if (count_open_fds() != initial_fds)
    {
        printf("FAIL: test_cmd_not_found - File descriptor leak detected!\n");
        exit(EXIT_FAILURE);
    }

    printf("PASS: test_cmd_not_found\n");
}

int main()
{
    test_read_mode();
    test_write_mode();
    test_invalid_params();
    test_cmd_not_found();
    return 0;
}