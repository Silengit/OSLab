#include "x86.h"
#include "device.h"

static unsigned int row = 0;
static unsigned int col = 0;

void syscallHandle(struct TrapFrame *tf);

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
	char ch = *(char *)tf->ecx;
	putChar(ch);
	if(ch == '\n'||col == 80){
		row++;
		col = 0;
	}
	if(ch != '\n')	
		print_vedio(ch, row, col++);
}

void sys_fork(struct TrapFrame *tf){
	//tf->eax = 0;
	return;
}

void sys_sleep(struct TrapFrame *tf){
	return;
}

void sys_exit(struct TrapFrame *tf){
	return;
}

void irqHandle(struct TrapFrame *tf) {
	/*
	 * 中断处理程序
	 */
	/* Reassign segment register */
	switch(tf->irq) {
		case -1:
			break;
		case 0xd:
			GProtectFaultHandle(tf);
			break;
		case 0x80:
			syscallHandle(tf);
			break;
		default:assert(0);
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
		default: break;
	}
}

void GProtectFaultHandle(struct TrapFrame *tf){
	assert(0);
	return;
}
