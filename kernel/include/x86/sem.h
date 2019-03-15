#ifndef __SEM_H__
#define __SEM_H__

#include "pcb.h"

#define MAX_SEM_NUM 10

struct Semaphore {
    int value;
    struct ProcessTable *list;
};

typedef struct Semaphore Semaphore;

Semaphore sem[MAX_SEM_NUM];

uint32_t sem_index;

void initSem();

void add2list(uint32_t sem_idx, struct ProcessTable *p);

struct ProcessTable *pop4list(uint32_t sem_idx);

void P(uint32_t sem_idx);

void V(uint32_t sem_idx);

void W(uint32_t sem_idx);

void R(uint32_t sem_idx);

#endif
