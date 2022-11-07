#include <phylosophers.h>

//LINK DE APOYO: https://github.com/pchapin/tutorialpthread/blob/master/philosophers.c

#define MAX_PHYLOS 8
#define MIN_PHYLOS 4
#define BASE_SEM_ID 1000

typedef enum
{
    THINKING = 1,
    EATING = 2,
    HUNGRY = 3,
} PHYLO_STATE;

typedef struct Phylosopher
{
    int pid;
    Semaphore * sem;
    PHYLO_STATE state;
} Phylosopher;

Phylosopher * phylos[MAX_PHYLOS];
static int actualPhylosopherCount = 0;
static Semaphore * semMutex;
static int problemRunning;

#define RIGHT(i) ((i) + 1) % (actualPhylosopherCount)                         /* number of i right neighbor */
#define LEFT(i) ((i) + actualPhylosopherCount - 1) % (actualPhylosopherCount) /* number of i left neighbor */

void phylo(int argc, char *argv[]);
void takeForks(int i);
void placeForks(int i);
void test(int i);
int addPhylosopher();
int removePhylosopher();
void printTable();

void phylo(int argc, char *args[])
{
    int idx = strToNum(args[0]);
    while (problemRunning)
    {
        takeForks(idx);
        sleep(1);
        placeForks(idx);
        sleep(1);
    }
}

void takeForks(int i)
{
    sys_semWait(semMutex);
    phylos[i]->state = HUNGRY;
    test(i);
    sys_semSignal(semMutex);
    sys_semWait(phylos[i]->sem);
}

void placeForks(int i)
{
    sys_semWait(semMutex);
    phylos[i]->state = THINKING;
    test(LEFT(i));
    test(RIGHT(i));
    sys_semSignal(semMutex);
}

void test(int i)
{
    if (phylos[i]->state == HUNGRY && phylos[LEFT(i)]->state != EATING && phylos[RIGHT(i)]->state != EATING)
    {
        phylos[i]->state = EATING;
        sys_semSignal(phylos[i]->sem);
    }
}

void run_phylosophers()
{
    problemRunning = 1;
    semMutex = sys_semCreate("Mutex", 1);
    printf("Bienvenido al problema de los filosofos\n");
    printf("El problema tiene un minimo de 4 filosofos y un maximo de 8 filosofos\n");
    printf("Puedes agregar un filosofo presionando la tecla \'a\', sacar uno presionando la tecla \'d\' y salir con la tecla \'q\'.\n");
    printf("El estado de cada uno se muestra como E (Eating) o . (Hungry)\n\n");

    printf("Esperando a los filosofos...\n\n");

    sleep(1);

    for (int i = 0; i < MIN_PHYLOS; i++){

        addPhylosopher();

    }

    char *args[] = {"PrintTable"};
    int fd[2];
    fd[0] = 0;
    fd[1] = 1;
    int printTablePid = sys_createProcess(&printTable, 1, args, fd, 1);
    while (problemRunning)
    {

        char key = getChar();
        switch (key)
        {
        case 'a':
        if (addPhylosopher() == -1)
            printf("No se puede agregar otro filosofo. Hay un maximo de 8 filosofos.\n");
        else
            printf("Se unio un nuevo filosofo\n");
        break;
        case 'd':
        if (removePhylosopher() == -1)
            printf("No se puede sacar otro filosofo. Hay un minimo de 4 filosofos.\n");
        else
            printf("Se saco un filosofo\n");
        break;
        case 'q':
        printf("Programa terminado\n");
        problemRunning = 0;
        break;
        default:
        break;
        }
    }

    for (int i = 0; i < actualPhylosopherCount; i++)
    {
        sys_semClose(phylos[i]->sem);
        sys_killProcess(phylos[i]->pid);
        sys_free(phylos[i]);
    }
    actualPhylosopherCount = 0;
    sys_killProcess(printTablePid);
    sys_semClose(semMutex);
}

int addPhylosopher()
{
    
    if (actualPhylosopherCount == MAX_PHYLOS)
        return -1;

    sys_semWait(semMutex);
    Phylosopher *auxPhylo = sys_malloc(sizeof(Phylosopher));
    if (auxPhylo == NULL)
        return -1;
    auxPhylo->state = THINKING;
    auxPhylo->sem = sys_semOpen("Mutex");
    char buffer[3];
    char *name[] = {"phylosopher", (char*)numToStr(actualPhylosopherCount, buffer, 10)};
    auxPhylo->pid = sys_createProcess(&phylo, 2, name, 0, 1);
    phylos[actualPhylosopherCount++] = auxPhylo;
    sys_semSignal(semMutex);
    return 0;
}

int removePhylosopher()
{
    if (actualPhylosopherCount == MIN_PHYLOS)
    {
        return -1;
    }

    actualPhylosopherCount--;
    Phylosopher *chosenPhylo = phylos[actualPhylosopherCount];
    sys_semClose(chosenPhylo->sem);
    sys_killProcess(chosenPhylo->pid);
    sys_free(chosenPhylo);
    sys_semSignal(semMutex);

    return 0;
}

void printTable(int argc, char *args[])
{
    while (problemRunning)
    {
        sys_semWait(semMutex);
        for (int i = 0; i < actualPhylosopherCount; i++)
        {
        phylos[i]->state == EATING ? putChar('E') : putChar('.');
        putChar(' ');
        }
        putChar('\n');
        sys_semSignal(semMutex);
        sys_yield();
    }
}

