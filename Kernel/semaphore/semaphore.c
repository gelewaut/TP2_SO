#include "../include/semaphore.h"
#include "../include/lib.h"
#include "../include/scheduler.h"
#include <memoryManager.h>

Semaphore *sem_list;

static uint64_t pop_waiting_queue(Semaphore *sem);
static uint8_t add_sem(Semaphore *sem);
static uint8_t rem_sem(Semaphore *sem);
static Semaphore *find_sem(const char * _name);
static uint8_t add_to_waiting_list(Semaphore *sem, uint64_t pid);

void mutex_lock(uint32_t *lock) {
  while (sys_xchg(lock, 1) != 0);
}

void mutex_unlock(uint32_t *lock) {
  sys_xchg(lock, 0);
}

Semaphore *sem_open(const char * _name) {
  Semaphore *sem = find_sem(_name);
  sem->proccesses_attached++;
  return sem;
}

uint8_t sem_wait(Semaphore *sem) {
  mutex_lock(&(sem->lock));
  if (sem->value > 0) {
    sem->value--;
    mutex_unlock(&(sem->lock));
  } else {
    mutex_unlock(&(sem->lock));
    uint64_t pid = getPID();
    add_to_waiting_list(sem, pid);
    // changeProcessState(pid, BLOCKED);
    call_timerTick();
  }
  return 1;
}

uint8_t sem_signal(Semaphore *sem) {
  if (sem == NULL) {
    return -1;
  }
  mutex_lock(&(sem->lock));
  sem->value++;
  mutex_unlock(&(sem->lock));
  if (sem->waiting != NULL) {
    uint64_t pid = pop_waiting_queue(sem);
    changeProcessState(pid, READY);
  }
  return 0;
}

static uint64_t pop_waiting_queue(Semaphore *sem) {
  if (sem->waiting == NULL) {
    return 0;
  }
  uint64_t pid = sem->waiting->pid;
  waiting_list * aux = sem->waiting;
  sem->waiting = sem->waiting->next;
  my_free(aux);
  return pid;
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
    if(my_strcmp(c_sem->next->name, sem->name) == 0) {
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
    if (my_strcmp(c_sem->name, _name) == 0) {
      return c_sem;
    }
    c_sem = c_sem->next;
  }
  return NULL;
}

Semaphore *sem_create(const char * _name, uint64_t _value) {
  if (find_sem(_name) != NULL) {
    // ncPrintln("Semaphore already exists.");
    return NULL;
  }
  Semaphore *sem = my_malloc(sizeof(Semaphore));
  if (sem == NULL) {
    // ncPrintln("Error en malloc.");
    return NULL;
  }
  my_strcpy(sem->name, _name);
  sem->value = _value;
  sem->proccesses_attached = 0;
  sem->lock = 0;
  sem->waiting = NULL;
  sem->next = NULL;
  add_sem(sem);

  return sem;
}

static uint8_t add_to_waiting_list(Semaphore *sem, uint64_t pid) {
  process * auxProcess = findProcess(pid);
    if(auxProcess == NULL) {
        return 0;
    }


  waiting_list *waiting_pid = my_malloc(sizeof(waiting_list));
  if (waiting_pid == NULL) {
    return 0;
  } 
  waiting_pid->pid = pid;
  waiting_pid->next = NULL;
  
  if (sem->waiting == NULL) {
    sem->waiting = waiting_pid;
  } else {
    waiting_list *c_waiting = sem->waiting;
    while (c_waiting->next != NULL) {
      c_waiting = c_waiting->next;
    }
    c_waiting->next = waiting_pid;
    auxProcess->state = BLOCKED;
  }
  return 0;
}

void sem_info() {
  Semaphore * aux = sem_list;
  waiting_list * sc;
  ncPrint("\nSemaphores Name, Value, Blocked Processes\n");
  if (aux == NULL) {
    ncPrint("No active semaphores\n");
    return;
  }
  while (aux != NULL) {
    ncPrint(aux->name);
    ncPrint(", ");
    ncPrintDec(aux->value);
    ncPrint(", ");
    sc = aux->waiting;
    if (sc == NULL) {
      ncPrint("No Blocked Processes\n");
    } else {
      while (sc != NULL) {
        ncPrintDec(sc->pid);
        ncPrintChar(' ');
        sc = sc->next;
      }
      ncPrintChar('\n');
    }
    aux = aux->next;
  }
}