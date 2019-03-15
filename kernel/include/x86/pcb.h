#ifndef __PCB_H__
#define __PCB_H__

#include "common.h"
#include "memory.h"
#include "cpu.h"

//ProcessTable
#define MAX_STACK_SIZE   (8<<10) 
#define MAX_PCB_NUM   10  
#define PROCESS_SIZE   0x100000 

//ProcessState
#define RUNABLE 0
#define RUNNING 1
#define BLOCKED 2
#define DEAD 3

//TimeSlice
#define TIMESLICE 100

//Idle
#define IDLE_STACK 0x500000


//Sleeptime
#define MAX_SLEEP_TIME 0x100000

//PID
int cur_pid;
int new_pcb_addr;
struct ProcessTable *current_pcb;

struct ProcessTable {
    union {
        uint8_t stack[MAX_STACK_SIZE];  // kernel stack + tf
        struct {
            uint8_t buf[MAX_STACK_SIZE - sizeof(struct TrapFrame)];
            struct TrapFrame tf;
        };	
    };
	uint32_t top_stack;
    int state;
    int timeCount;
    int sleepTime;
    uint32_t pid;
	struct ProcessTable *next;
};

struct ProcessTable pcb[MAX_PCB_NUM];

void schedule();
void initPCB();
void enterUserSpace(uint32_t entry);
void add_PCB();
int distribute_pid();
void IDLE();

#endif
