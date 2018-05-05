#include "x86/pcb.h"

extern SegDesc gdt[NR_SEGMENTS];
extern TSS tss;

void schedule(){
	int Flag_Run = 0;
	if(pcb[2].state != DEAD){		//after fork pcb[2], emulate queue
		if(current_pcb == &pcb[1]){
			for(int i = 2; i >= 1; i--)
				if(pcb[i].state == RUNABLE){
					current_pcb =  &pcb[i];
					pcb[i].state = RUNNING;
					Flag_Run = 1;
					break;		
				}
		}
		else{
			for(int i = 1; i <= 2; i++)
				if(pcb[i].state == RUNABLE){
					current_pcb =  &pcb[i];
					pcb[i].state = RUNNING;
					Flag_Run = 1;
					break;		
				}
		}
	}
	else{
		if(pcb[1].state == RUNABLE){
			current_pcb = &pcb[1];
			pcb[1].state = RUNNING;
			Flag_Run = 1;
		}
	}
	
	if(Flag_Run == 0){
		current_pcb = NULL;
		IDLE();		
	}
	else{
		//p1 kernel stack 2 p2 kernel stack
		tss.esp0 = (uint32_t)&current_pcb->top_stack; 
		tss.ss0 = KSEL(SEG_KDATA);

        gdt[SEG_UDATA] = SEG(STA_W,  ((current_pcb-pcb)-1) * PROCESS_SIZE, 0xffffffff, DPL_USER);
	
		asm volatile("movl %0, %%esp" ::"r"(&current_pcb->tf));

		asm volatile("popl %gs");
       	asm volatile("popl %fs");
       	asm volatile("popl %es");
        asm volatile("popl %ds");
        asm volatile("popal");  
        asm volatile("addl $4, %esp");
        asm volatile("addl $4, %esp");

       	asm volatile("iret");
			
	}
}

void initPCB() {
	current_pcb = NULL;
	cur_pid = 100;
	new_pcb_addr = 0x200000;
	for(int i=0;i<MAX_PCB_NUM;i++){
		pcb[i].state = DEAD;
		pcb[i].timeCount = TIMESLICE;
		pcb[i].sleepTime = 0;
		pcb[i].pid = i;
	}//pcb[0] is IDLE
}

void enterUserSpace(uint32_t entry){
	//from kernel to user1
	mov2user;

	//initial the father process pdb
	pcb[1].tf.ss = USEL(SEG_UDATA);
	pcb[1].tf.esp = new_pcb_addr + PROCESS_SIZE;
	asm volatile("pushfl");
	asm volatile("movl (%%esp), %0" : "=r"(pcb[1].tf.eflags) :);
	pcb[1].tf.cs = USEL(SEG_UCODE1);
	pcb[1].tf.eip = entry;

	pcb[1].pid = distribute_pid();
	pcb[1].state = RUNNING;
	
	current_pcb = &pcb[1];

	asm volatile("movl %0, %%esp" ::"r"(&current_pcb->tf.eip));
	asm volatile("iret");
}

void add_PCB(){
	assert(pcb[2].state == DEAD);
	if(pcb[2].state == DEAD){
		new_pcb_addr += PROCESS_SIZE;
		pcb[2].tf.cs = USEL(SEG_UCODE2);
		pcb[2].pid = distribute_pid();
		pcb[2].state = RUNABLE;
	}
	else	return;
}


int distribute_pid(){
	int new_pid = cur_pid;
	cur_pid++;
	return new_pid;
}

void IDLE(){
	asm volatile("movl %0, %%esp;" ::"i"(IDLE_STACK));
	enableInterrupt();
	while(1)
    	waitForInterrupt();
}
