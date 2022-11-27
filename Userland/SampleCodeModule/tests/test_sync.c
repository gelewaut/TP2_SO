#include <test_util.h>
#include <syscalls.h> 
#include <userlibc.h>


#define TOTAL_PAIR_PROCESSES 2

#define NULL (void *) 0

static int64_t global;  //shared memory

void slowInc(int64_t *p, int64_t inc){
  uint64_t  aux= *p;
  sys_yield(); //This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

uint64_t my_process_inc(uint64_t argc, char *argv[]){
  uint64_t n;
  int8_t inc;
  Semaphore * sem;
  int8_t use_sem;

  if (argc != 4) {
    printf("Not ok");
    return -1;
  }

  if ((n = satoi(argv[0])) <= 0) return -1;
  if ((inc = satoi(argv[1])) == 0) return -1;
  if ((use_sem = satoi(argv[2])) < 0) return -1;

  if (use_sem) {
    if ( (sem = sys_semOpen(argv[3])) == NULL){
      printf("test_sync: ERROR opening semaphore\n");
      return -1;
    }
  }

  uint64_t i;
  for (i = 0; i < n; i++){
    if (use_sem){
      sys_semWait(sem);
    } 
    slowInc(&global, inc);
    if (use_sem) {
      sys_semSignal(sem);
    }
  }

  if (use_sem) {
    sys_semClose(sem);
  }
  
  return 0;
}

uint64_t test_sync(uint64_t argc, char *argv[]){ //{n, use_sem, semName}
  printf("In test_sync...\n");
  if (argc != 3) {
    printf("Expected 3 Arguments: n, use_sem, sem_name");
    return -1;
  }

  Semaphore * sem = sys_semCreate(argv[2], 1);
  
  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];
  int std_fd[2] = {0, 1};

  global = 0;

  uint64_t i;
  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    char * argv1[] = {"my_process_inc", argv[0], "-1", argv[1], argv[2]};
    pids[i] = sys_createProcess((void (*)(int, char **))&my_process_inc, 5, argv1, std_fd, 1);
    char * argv2[] = {"my_process_inc", argv[0], "1", argv[1], argv[2]};
    pids[i + TOTAL_PAIR_PROCESSES] = sys_createProcess((void (*)(int, char **))&my_process_inc, 5, argv2, std_fd, 1);
  }

  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    sys_wait(pids[i]);
    sys_wait(pids[i + TOTAL_PAIR_PROCESSES]);
  }

  sys_semClose(sem);
  char buf[10];
  numToStr(global, buf, 10);
  printf("Final value: %s",buf);

  return 0;
}