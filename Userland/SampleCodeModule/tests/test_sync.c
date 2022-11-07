#include <test_util.h>
#include <syscalls.h> 
#include <userlibc.h>


#define TOTAL_PAIR_PROCESSES 2

#define NULL (void *) 0

int64_t global;  //shared memory
char * SEM_ID = "sem";

void slowInc(int64_t *p, int64_t inc){
  uint64_t aux = *p;
  sys_yield(); //This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

uint64_t my_process_inc(uint64_t argc, char *argv[]){
  uint64_t n;
  int8_t inc;
  Semaphore * sem;
  int8_t use_sem;

  if (argc != 3) return -1;

  if ((n = satoi(argv[0])) <= 0) return -1;
  if ((inc = satoi(argv[1])) == 0) return -1;
  if ((use_sem = satoi(argv[2])) < 0) return -1;

  if (use_sem)
    if ( (sem = sys_semOpen(SEM_ID)) == NULL){
      printf("test_sync: ERROR opening semaphore\n");
      return -1;
    }

  uint64_t i;
  for (i = 0; i < n; i++){
    if (use_sem) sys_semWait(sem);
    slowInc(&global, inc);
    if (use_sem) sys_semSignal(SEM_ID);
  }

  if (use_sem) sys_semClose(sem);
  
  return 0;
}

uint64_t test_sync(uint64_t argc, char *argv[]){ //{n, use_sem, 0}
  printf("In test_sync...\n");
  Semaphore * sem = sys_semCreate(SEM_ID, 1);
  
  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];
  int std_fd[2] = {0, 1};

  if (argc != 2) return -1;

  char * argvDec[] = {argv[0], "-1", argv[1], NULL};
  char * argvInc[] = {argv[0], "1", argv[1], NULL};

  global = 0;

  uint64_t i;
  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    pids[i] = sys_createProcess(&my_process_inc, 3, argvDec, std_fd, 1);
    pids[i + TOTAL_PAIR_PROCESSES] = sys_createProcess(&my_process_inc, 3, argvInc, std_fd, 1);

  }

  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    sys_wait(pids[i]);
    sys_wait(pids[i + TOTAL_PAIR_PROCESSES]);
  }

  printf("Final value: %d\n", global);

  return 0;
}