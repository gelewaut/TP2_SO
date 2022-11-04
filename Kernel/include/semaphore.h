#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <stdint.h>
#include <blockedProcessList.h>
#include <naiveConsole.h>

typedef struct waiting_list {
  uint64_t pid;
  waiting_list *next;
} waiting_list;

typedef struct Semaphore {
  const char * name;
  uint64_t value;
  uint16_t listening;
  uint32_t *lock;
  struct waiting_list *waiting;
  uint32_t proccesses_attached;
  struct Semaphore *next;
} Semaphore;

Semaphore *sem_create(const char * _name, uint64_t _value);
Semaphore *sem_open(const char * _name);
uint8_t sem_close(Semaphore *sem);
uint8_t sem_signal(Semaphore *sem);
uint8_t sem_wait(Semaphore *sem);
void sem_info();

#endif