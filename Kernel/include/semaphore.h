#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <stdint.h>
#include <naiveConsole.h>

#define MAX_WAITING_PROCESSES 32

typedef struct Semaphore {
  const char * name;
  uint64_t value;
  uint16_t waiting;
  uint16_t listening;
  struct Semaphore *next;
  uint32_t *lock;
  uint32_t waiting[MAX_WAITING_PROCESSES];
  uint8_t waiting_idx;
  uint32_t proccesses_attached;
} Semaphore;

Semaphore *sem_open(const char * _name);
uint8_t sem_close(Semaphore *sem);
uint8_t sem_signal(Semaphore *sem);
uint8_t sem_wait(Semaphore *sem);
void sem_info();

#endif