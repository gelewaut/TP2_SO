/* sampleCodeModule.c */
#include <shell.h>
#include <syscalls.h>

int main() {
	init_shell();
	sys_killProcess(sys_getPID());
	return 0;
}