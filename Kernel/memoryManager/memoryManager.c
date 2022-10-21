#ifdef FREEMM

//Codigo extraido o basado en el libro C Programming de Kernigan y Ritchie

#include <memoryManager.h>
#define NULL 0
#define NALLOC 1024 /* minimo # de unidades por requerir */

typedef long Align; // Para alinear al limite mayor

union header
{
    struct 
    {
        union header *ptr;
        unsigned size;
    }s;
    Align x;
};

typedef union header Header;

static Header *base;
static Header *freep = NULL;

unsigned long total_units;


void mem_init(char *mem_base, unsigned long mem_size){
    total_units = (mem_size + sizeof(Header) - 1) / sizeof(Header) + 1;
    freep = base = (Header *)mem_base;
    freep->s.size = total_units;
    freep->s.ptr = freep;
}

void * my_malloc(unsigned long nbytes){
    if (nbytes == 0)
    {
        return NULL;
    }
    

    Header *p, *prevp;
    prevp = freep;
    unsigned long nunits;

    nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
    if ((prevp = freep) == NULL) { /* no hay lista libre aún */ 
        base->s.ptr = freep = prevp = &base; 
        base->s.size = 0;
    }


    for (p = prevp->s.ptr;; prevp = p, p = p->s.ptr)
    {
        if (p->s.size >= nunits)
        {                                 /* suficientemente grande */
                if (p->s.size == nunits) /* exacto */
                    prevp->s.ptr = p->s.ptr;
                else                 /* asigna la parte final */
                { 
                    p->s.size -= nunits;
                    p += p->s.size;
                    p->s.size = nunits;
                }
                freep = prevp;
                return (void *)(p + 1);
        }
        if (p == freep)    /* dio la vuelta a la lista libre */
            return NULL; /* nada libre */
                        
    }
}

void my_free(void *ap ){
    if (ap == NULL || (((long)ap - (long)base) % sizeof(Header)) != 0)
            return;

      Header *bp, *p;
      p=freep;
      bp = (Header *)ap - 1; /* apunta al encabezador de un bloque */
      
      if (bp < base || bp >= (base + total_units * sizeof(Header)))
            return;
    
      for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr){
            if (p >= p->s.ptr && (bp > p || bp < p->s.ptr)){
                  break;            /* libera bloque al inicio o al final */
            }
                  
      }
      if (bp + bp->s.size == p->s.ptr) /* une al nbr superior */
      { 
            bp->s.size += p->s.ptr->s.size;
            bp->s.ptr = p->s.ptr->s.ptr;
      }
      else{
            bp->s.ptr = p-> s.ptr;
      }
      if (p + p->s.size == bp)     /* une al nbr inferior */
      { 
            p->s.size += bp->s.size;
            p->s.ptr = bp->s.ptr;
      }else{
            p->s.ptr = bp;
      }
      freep = p;
}

void print_memSet(){
    long long idx = 1;
    Header *original, *current;
    original = current = freep;
    int flag = 1;

    print("%s %x %s", "Total memory:        ", total_units * sizeof(Header), "\n\n");
    if (freep == NULL)
        print("%s", "    No free memory\n");
    while (current != original || flag)
    {
        flag = 0;
        print("%s", "-------------------------------\n");
        print("%s %x %s", "        Base:        ", (uint64_t)current, "\n");
        print("%s %x %s", "        Free memory: ", (current->s.size), "\n");
        print("%s", "-------------------------------\n");
        current = current->s.ptr;
        idx++;
    }

    print("%s", "\n\n");

}

#endif