#include "x86/sem.h"

void initSem(){
	sem_index = 0;
	for(int i=0;i<MAX_SEM_NUM;i++){
		sem[i].value = 0;
		sem[i].list = NULL;
	}
}

void add2list(uint32_t sem_idx, struct ProcessTable *p){
	//assert(p != NULL);
	//Log_int("p->pid",p->pid);
	if(sem[sem_idx].list == NULL){
		sem[sem_idx].list = p;
		p->next = NULL;	
	}
	else{
		struct ProcessTable *tail = NULL;
		struct ProcessTable *temp= sem[sem_idx].list;
		while(temp != NULL){
			tail = temp;
			temp = temp->next;
		}
		tail -> next = p;
		p -> next = NULL;
		tail = p;
	}
}

struct ProcessTable *pop4list(uint32_t sem_idx){
	//Log_int("list->pid",list->pid);
	assert(sem[sem_idx].list != NULL);
	struct ProcessTable *p = sem[sem_idx].list;
	sem[sem_idx].list = sem[sem_idx].list -> next;
	p->next = NULL;
	return p;
}

void P(uint32_t sem_idx) {
    sem[sem_idx].value --;
    if (sem[sem_idx].value < 0){   
		W(sem_idx);
	}
}

void V(uint32_t sem_idx) {
   	sem[sem_idx].value ++;
    if (sem[sem_idx].value <= 0)
        R(sem_idx);
}

void W(uint32_t sem_idx) {
	current_pcb -> state = BLOCKED;
	current_pcb -> sleepTime = MAX_SLEEP_TIME;
	add2list(sem_idx,current_pcb);
	schedule();
}

void R(uint32_t sem_idx) {
	struct ProcessTable *p =pop4list(sem_idx);
	p -> state = RUNABLE;
	p -> timeCount = TIMESLICE;
	p -> sleepTime = 0;
	//schedule();
}

