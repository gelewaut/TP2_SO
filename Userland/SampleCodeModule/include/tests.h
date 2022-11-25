#ifndef _TESTS_H_
#define _TESTS_H_

#include <stdint.h>

uint64_t test_mm(uint64_t args_cant, char *args[]);
void test_prio();
int64_t test_processes(uint64_t args_cant, char *args[]);
uint64_t test_sync(uint64_t args_cant, char *args[]);

#endif