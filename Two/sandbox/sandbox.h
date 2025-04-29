#ifndef SANDBOX_H
#define SANDBOX_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
int sandbox(void(*f)(void), unsigned int timeout, bool verbose);
#endif
