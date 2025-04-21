#ifndef SANDBOX_H
#define SANDBOX_H
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdbool.h>
int sandbox(void (*f)(void), unsigned int timeout, bool verbose);
// Sandbox prototypes --------------------------------------------------------
int sandbox_code1(void (*f)(void), unsigned int timeout, bool verbose);
int sandbox_code2(void (*f)(void), unsigned int timeout, bool verbose);
int sandbox_revised(void (*f)(void), unsigned int timeout, bool verbose);
void do_nothing(int sig);
#endif