#include "x86/pcb.h"

void initPCB() {
	cur_pid = 100;

	for(int i=0;i<MAX_PCB_NUM;i++){
		pcb[i].state = DEAD;
		pcb[i].timeCount = TIMESLICE;
		pcb[i].sleepTime = 0;
		pcb[i].pid = i;
	}//pcb[0] is IDLE
}

void enterUserSpace(uint32_t entry){
	//from kernel to user1
	/*asm volatile("movl %0, %%eax":: "r"(KSEL(SEG_UDATA1)));//user1
	asm volatile("movw    %ax, %ds");
	asm volatile("movw    %ax, %es");
	asm volatile("movw    %ax, %ss");
	asm volatile("movw    %ax, %fs");*/

	//initial the father process pdb

	pcb[1].tf.ss = USEL(SEG_UDATA1);
	pcb[1].tf.esp = 0x300000;
	pcb[1].tf.cs = USEL(SEG_UCODE1);
	pcb[1].tf.eip = entry;
	pcb[1].pid = distribute_pid();
	pcb[1].state = RUNNING;

	asm volatile("pushl   %0"::"r"(USEL(SEG_UDATA1)));//push %ss
	asm volatile("pushl  $(128 << 20)");	 	 //push %esp
	asm volatile("pushfl");				 //push eflags
	asm volatile("pushl   %0"::"r"(USEL(SEG_UCODE1)));//push %cs
	asm volatile("pushl   %0"::"r"(entry));

	asm volatile("sti");
	asm volatile("iret");
}

int distribute_pid(){
	int new_pid = cur_pid;
	cur_pid++;
	return new_pid;
}
