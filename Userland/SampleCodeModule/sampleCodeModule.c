/* sampleCodeModule.c */
#include <shell.h>
#include <syscalls.h>
#include <stddef.h>

static void write1(int argc, char ** argv) {
	Semaphore * sem = sys_semOpen("my_sem");
	if (sem == NULL)
		return;
	// sys_printProcesses();
	while (1) {
		// for(int i=0; i<5; i++) {
		// }
		sys_write(1, "1", 1);
		sys_semWait(sem);
		sys_semSignal(sem);
	}
}
static void write2(int argc, char ** argv) {
	Semaphore * sem = sys_semOpen("my_sem");
	if (sem == NULL)
		return;
	// sys_printProcesses();
	while (1) {
		// for(int i=0; i<5; i++) {
		// }
		sys_write(1, "2", 1);
		sys_semWait(sem);
		sys_semSignal(sem);
	}
}
static void write3(int argc, char ** argv) {
	Semaphore * sem = sys_semOpen("my_sem");
	if (sem == NULL)
		return;
	// sys_printProcesses();
	while (1) {
		// for(int i=0; i<5; i++) {
		// }
		sys_write(1, "3", 1);
		sys_semWait(sem);
		sys_semSignal(sem);
	}
}

static void read (int argc, char ** argv) {
	Semaphore * sem = sys_semOpen("my_sem");
	if (sem == NULL)
		return;
	// sys_printProcesses();
	while (1) {
		// for (int i=0; i<20; i++) {
		// }
		// sys_write(1, "ok", 2);
		// sys_blockProcess(4);
		// sys_blockProcess(5);
		// sys_blockProcess(6);
		// sys_yield();
		sys_semSignal(sem);
		sys_yield();
	}
}



int main() {
	// int fd[2];
	// fd[0] = in;
	// fd[1] = 1;
	// Semaphore * sem = sys_semCreate("my_sem", 0);

	// fd[0] = 0;
	// fd[1] = 1;
	// sys_createProcess(&write1, 0, NULL, fd, 1);
	// sys_createProcess(&write2, 0, NULL, fd, 1);
	// sys_createProcess(&write3, 0, NULL, fd, 1);
	// sys_createProcess(&read, 0, NULL, fd, 1);
	// sys_printProcesses();
	// sys_semSignal(sem);
	// while(1);
	init_shell();
	return 0;
}