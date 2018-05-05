#include "x86.h"
#include "device.h"

static unsigned int row = 0;
static unsigned int col = 0;

void syscallHandle(struct TrapFrame *tf);

void TimerInterruptHandle(struct TrapFrame *tf);

void GProtectFaultHandle(struct TrapFrame *tf);

void print_vedio(char ch, int row, int col) {
	asm volatile("movl %0, %%eax":: "r"(KSEL(SEG_VIDEO)));
	asm volatile("movw %ax, %gs");
	asm volatile("movl %0, %%edi"::"r"((80*row+col)*2));//第row行第col列打印
	asm volatile("movb $0x0c, %ah");		   //黑底红字
	asm volatile("movb %0, %%al"::"r"(ch));		   //ch为ASCII码
	asm volatile("movw %ax, %gs:(%edi) ");             //写显存
}


void sys_write(struct TrapFrame *tf){
	char ch;
	
	if(current_pcb == &pcb[1]){
		//Log_hex("tf->ecx",tf->ecx);		
		ch = *(char *)tf->ecx;
	}
	else{
		//Log_hex("tf->ecx",tf->ecx);		
		ch = *(char *)(tf->ecx + PROCESS_SIZE);
	}
	putChar(ch);
	if(ch == '\n'||col == 80){
		row++;
		col = 0;
	}
	if(ch != '\n')	
		print_vedio(ch, row, col++);
}

void sys_fork(struct TrapFrame *tf){
	//Log_hex("new_pcb_addr",new_pcb_addr);
	unsigned int src= new_pcb_addr;
	unsigned int dst = new_pcb_addr + PROCESS_SIZE;

	//Log_hex("src",src);
	//Log_hex("dst",dst);

	for (int i = 0; i < PROCESS_SIZE; i++)
		*(uint8_t*)(dst++) = *(uint8_t*)(src++);
	for (int i = 0; i < MAX_STACK_SIZE; i++) 
        	pcb[2].stack[i] = pcb[1].stack[i];

	add_PCB();

	pcb[2].tf.eax = 0;
	pcb[1].tf.eax = pcb[2].pid;
	
	assert(current_pcb->tf.eip == tf -> eip);	
}

void sys_sleep(struct TrapFrame *tf){
	current_pcb->sleepTime = tf -> ecx;
	current_pcb->state = BLOCKED;
	schedule();
}

void sys_exit(struct TrapFrame *tf){
	putChar('E');
	current_pcb->state = DEAD;
	schedule();
}

void irqHandle(struct TrapFrame *tf) {
	/*
	 * 中断处理程序
	 */
	mov2kernel;

	switch(tf->irq) {
		case -1:
			break;
		case 0xd:
			GProtectFaultHandle(tf);
			break;
		case 0x20:
			TimerInterruptHandle(tf);
			break;
		case 0x80:
			syscallHandle(tf);
			break;
		default:Panic("irqHandle error!\n");assert(0);
	}
	
}

void syscallHandle(struct TrapFrame *tf) {
	/* 实现系统调用*/
	switch(tf->eax) {
		case 0: break;//maybe later
		case SYS_write: sys_write(tf); break;
		case SYS_fork: sys_fork(tf); break;
		case SYS_sleep: sys_sleep(tf); break;
		case SYS_exit: sys_exit(tf); break;
		default: Panic("syscall error!\n");assert(0);break;
	}
}

void TimerInterruptHandle(struct TrapFrame *tf){
	
	for(int i = 1; i <= 2; i++){
		if(pcb[i].state == BLOCKED){
			if(pcb[i].sleepTime > 0){
				pcb[i].sleepTime --;
				if(i == 1)
					putChar('Z');
				else
					putChar('z');
			}
			else 
				pcb[i].state = RUNABLE;

			//Log_hex("pcb[1].state",pcb[1].state);
			//Log_hex("pcb[2].state",pcb[2].state);
		}
		else if(pcb[i].state == RUNNING){
			if(pcb[i].timeCount > 0){	
				pcb[i].timeCount --;
				if(i == 1)
					current_pcb = &pcb[2]; //cheat
				else
					current_pcb = &pcb[1];
			}	
			else {
				pcb[i].timeCount = TIMESLICE;
			}
			pcb[i].state = RUNABLE;
		}
	}
	schedule();
}

void GProtectFaultHandle(struct TrapFrame *tf){
	assert(0);
	return;
}
