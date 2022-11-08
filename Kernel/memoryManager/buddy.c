#ifdef BUDDY
// LINK DE APOYO: https://github.com/evanw/buddy-malloc/blob/master/buddy-malloc.c

#include <memoryManager.h>
#include <stddef.h>

#define HEADER_SIZE 8
#define MIN_ALLOC_LOG2 4 // The minimum allocation size is 16 bytes.
#define MIN_ALLOC ((size_t)1 << MIN_ALLOC_LOG2)
#define MAX_ALLOC_LOG2 31 // The maximum allocation size is currently set to 2gb.
#define MAX_ALLOC ((size_t)1 << MAX_ALLOC_LOG2)
#define BUCKET_COUNT (MAX_ALLOC_LOG2 - MIN_ALLOC_LOG2 + 1) //Allocations are done in powers of two starting from MIN_ALLOC and ending at MAX_ALLOC inclusive.

typedef struct lists_t {
      unsigned int bucket;
      unsigned int free;
      struct lists_t *prev, *next;
} lists_t;

static lists_t buckets[BUCKET_COUNT];
static uint64_t bucket_max;
static lists_t *base;
static unsigned int buckets_count;

static void list_init(lists_t *list) {
  list->prev = list;
  list->next = list;
}

static void list_push(lists_t *list, lists_t *entry) {
  lists_t *prev = list->prev;
  entry->prev = prev;
  entry->next = list;
  prev->next = entry;
  list->prev = entry;
}

static void list_remove(lists_t *entry) {
  lists_t *prev = entry->prev;
  lists_t *next = entry->next;
  prev->next = next;
  next->prev = prev;
}

static lists_t *list_pop(lists_t *list) {
  lists_t *back = list->prev;
  if (back == list) 
      return NULL;
  list_remove(back);
  return back;
}

int log2(uint32_t n);

int is_empty(lists_t *list)
{
    return list->prev == list;
}

static void addNodeToBucket(lists_t *bucketList, lists_t *node, uint8_t bucketLevel) {
  node->bucket = bucketLevel;
  node->free = 1;
  list_push(bucketList, node);
}

static size_t getIdealBucket(size_t request) {
  size_t requestLog2 = log2(request);

  if (requestLog2 < MIN_ALLOC_LOG2) {
    return 0;
  }

  requestLog2 -= MIN_ALLOC_LOG2;

  if (request && !(request & (request - 1))) {
    return requestLog2;
  }

  return requestLog2 + 1;
}

static int getAvailableBucket(uint8_t minBucketRequired) {
  uint8_t availableBucket;

  for (availableBucket = minBucketRequired; availableBucket < buckets_count && is_empty(&buckets[availableBucket]); availableBucket++)
    ;

  if (availableBucket > buckets_count) {
    return -1;
  }

  return availableBucket;
}

static lists_t *getBuddy(lists_t *node) {
  uint8_t bucket = node->bucket;
  uintptr_t currentOffset = (uintptr_t)node - (uintptr_t)base;
  uintptr_t newOffset = currentOffset ^ (1 << (MIN_ALLOC_LOG2 + bucket));

  return (lists_t *)((uintptr_t)base + newOffset);
}

static lists_t *getAddress(lists_t *node) {
  uint8_t bucket = node->bucket;
  uintptr_t mask = (1 << (MIN_ALLOC_LOG2 + bucket));
  mask = ~mask;

  uintptr_t currentOffset = (uintptr_t)node - (uintptr_t)base;
  uintptr_t newOffset = currentOffset & mask;

  return (lists_t *)(newOffset + (uintptr_t)base);
}

void mem_init(char *memBase, unsigned long memSize) {
  if (memBase == NULL) {
    return;
  }

  base = (lists_t *)memBase;
  bucket_max = memSize;

  buckets_count = (int)log2(memSize) - MIN_ALLOC_LOG2 + 1;

  if (buckets_count > BUCKET_COUNT) {
    buckets_count = BUCKET_COUNT;
  }

  for (int i = 0; i < buckets_count; i++) {
    list_init(&buckets[i]);
    buckets[i].free = 0;
    buckets[i].bucket = i;
  }

  addNodeToBucket(&buckets[buckets_count - 1], base, buckets_count - 1);
}

void *my_malloc(unsigned long nbytes) {
  size_t bytesNeeded = nbytes + sizeof(lists_t);

  if (nbytes == 0 || bytesNeeded > bucket_max + 1) {
    return NULL;
  }

  uint8_t idealBucket = getIdealBucket(bytesNeeded);
  int availableBucket = getAvailableBucket(idealBucket);

  if (availableBucket == -1) {
    return NULL;
  }

  lists_t *resultNode = list_pop(&buckets[availableBucket]);

  for (; idealBucket < availableBucket; availableBucket--) {
    resultNode->bucket--;
    addNodeToBucket(&buckets[availableBucket - 1], getBuddy(resultNode), availableBucket - 1);
  }

  resultNode->free = 0;

  return (void *)(resultNode + 1);
}

void my_free(void *block) {
  if (block == NULL) {
    return;
  }

  lists_t *freeNode = (lists_t *)block - 1;

  freeNode->free = 1;

  lists_t *freeBuddy = getBuddy(freeNode);

  while (freeNode->bucket != buckets_count - 1 && freeBuddy->bucket == freeNode->bucket && freeBuddy->free) {
    list_remove(freeBuddy);
    freeNode = getAddress(freeNode);
    freeNode->bucket++;
    freeBuddy = getBuddy(freeNode);
  }
  
  list_push(&buckets[freeNode->bucket], freeNode);
}

void print_memSet()
{
    lists_t *list, *aux;
    uint32_t index = 0;
    uint32_t availableSpace = 0;
    ncPrint("\nLevels with free blocks:\n");
    for (int i = buckets_count - 1; i >= 0; i--)
    {
        list = &buckets[i];
        if (!is_empty(list))
        {
          ncPrint("Bucket: ");
          ncPrintDec(i + MIN_ALLOC_LOG2);
          ncPrint(", ");
          ncPrint("Free blocks of size: 2^");
          ncPrintDec(i + MIN_ALLOC_LOG2);
          ncPrint(",  ");
          for (aux = list->next, index = 1; aux != list; index++, aux = aux->next)
          {
              if (aux->free)
              {
                ncPrint("Bucket number: ");
                ncPrintDec(index);
                ncPrint(", State: free");
                availableSpace += index * (1 << (MIN_ALLOC_LOG2 + i));
              }
          }
          ncPrintChar('\n');
        }
    }
    ncPrint("Available Space: ");
    ncPrintDec(availableSpace);
    ncPrintChar('\n');
}

int log2(uint32_t n)
{
      if (n == 0)
            return -1;
      int ans = -1;
      while (n)
      {
            ans++;
            n >>= 1;
      }
      return ans;
}

#endif