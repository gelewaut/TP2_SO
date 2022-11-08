#ifndef _PHYLOSOPHERS_H_
#define _PHYLOSOPHERS_H_
#include <stdint.h>
#include <userlibc.h>
#include <stddef.h>
#include <syscalls.h>

void run_phylosophers();
void printTable();
int removePhylosopher();
int addPhylosopher();
void test(int i);
void placeForks(int i);
void takeForks(int i);
void phylo(int argc, char *args[]);

#endif