#include "../include/semaphore.h"
#include "../include/lib.h"
#include "../include/scheduler.h"
#include "../include/blockedProcessList.h"

Semaphore *sem_list;

static Semaphore *sem_create(const char * _name, uint64_t value);
static uint8_t add_sem(Semaphore *sem);
static uint8_t rem_sem(Semaphore *sem);
static Semaphore *sem_find(const char * _name);
static uint8_t unblock_sem(Semaphore *sem);

void mutex_lock(uint32_t *lock) {
  while (sys_xchg(lock, 1) != 0);
}

void mutex_unlock(uint32_t *lock) {
  sys_xchg(lock, 0);
}

Semaphore *sem_open(const char * _name) {
  Semaphore *sem = sem_find(_name);
  if (sem->proccesses_attached >= MAX_WAITING_PROCESSES) {
    return NULL;
  }

  sem->proccesses_attached++;
  return sem;
}

uint8_t sem_wait(Semaphore *sem) {
  mutex_lock(sem->lock);
  if (sem->value > 0) {
    sem->value--;
    mutex_unlock(sem->lock);
  } else {
    uint64_t pid = getPID();
    sem->waiting[sem->waiting_idx++] = pid;
    changeProcessState(pid, BLOCKED);
    release(sem->lock);
  }
  return 1;
}

uint8_t sem_signal(Semaphore *sem) {
  mutex_lock(sem->lock);
  sem->value++;
  if (sem->waiting > 0) {
    uint64_t pid = sem->waiting[0];
    pop_waiting_queue(sem);
    changeProcessState(pid, READY);
  }
  mutex_unlock(sem->lock);
  return 0;
}

uint8_t sem_close(Semaphore * sem) {
  if ((sem->listening--) == 0) {
    rem_sem(sem);
  }
  return 0;
}

static uint8_t add_sem(Semaphore *sem) {
  if (sem_list == NULL) {
    sem_list = sem;
    return 0;
  }
  Semaphore *aux = sem_list;
  while (aux->next != NULL) {
    aux = aux->next;
  }
  aux->next = sem;
  return 0; 
}

static uint8_t rem_sem(Semaphore *sem) {
  Semaphore *c_sem = sem_list;
  while (c_sem->next != NULL) {
    if(strcmp(c_sem->next->name, sem->name) == 0) {
      c_sem->next = sem->next;
      my_free(sem);
      return 0;
    }
  }
  // Semaphore wasn't found in the list.
  return -1;
}

static Semaphore *find_sem(const char * _name) {
  Semaphore *c_sem = sem_list;
  while (c_sem != NULL) {
    if (strcmp(c_sem->name, _name) == 0) {
      return c_sem;
    }
  }
  return NULL;
}

static Semaphore *sem_create(const char * _name, uint64_t _value) {
  Semaphore *sem = my_malloc(sizeof(Semaphore));
  if (sem == NULL) {
    ncPrintln("Error en malloc.");
    return NULL;
  }
  if (sem_find(_name) != NULL) {
    ncPrintln("Semaphore already exists.");
    return NULL;
  }
  sem->name = _name;
  sem->value = _value;
  sem->proccesses_attached = 0;
  sem->lock = 0;
  sem->next = NULL;
  add_sem(sem);

  return sem;
}