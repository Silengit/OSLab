#ifndef __PCB_H__
#define __PCB_H__

#include "common.h"
#include "memory.h"

//ProcessTable
#define MAX_STACK_SIZE   256  
#define MAX_PCB_NUM   10  

//ProcessState
#define RUNABLE 0
#define RUNNING 1
#define BLOCKED 2
#define DEAD 3

//TimeSlice
#define TIMESLICE 100

//PID
int cur_pid;

struct ProcessTable {
    uint32_t stack[MAX_STACK_SIZE]; // 内核堆栈
    struct TrapFrame tf;
    int state;
    int timeCount;
    int sleepTime;
    uint32_t pid;
};

struct ProcessTable pcb[MAX_PCB_NUM];

void initPCB();
void enterUserSpace(uint32_t entry);
int distribute_pid();

#endif
