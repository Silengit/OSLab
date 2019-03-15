#include "x86.h"
#include "device.h"

static unsigned int row = 0;
static unsigned int col = 0;

void syscallHandle(struct TrapFrame *tf);

void TimerInterruptHandle(struct TrapFrame *tf);

void GProtectFaultHandle(struct TrapFrame *tf);

static int letter_code[] = {
	30, 48, 46, 32, 18, 33, 34, 35, 23, 36,
	37, 38, 50, 49, 24, 25, 16, 19, 31, 20,
	22, 47, 17, 45, 21, 44
};

void print_vedio(char ch, int row, int col) {
	asm volatile("movl %0, %%eax":: "r"(KSEL(SEG_VIDEO)));
	asm volatile("movw %ax, %gs");
	asm volatile("movl %0, %%edi"::"r"((80*row+col)*2));//第row行第col列打印
	asm volatile("movb $0x0c, %ah");		   //黑底红字
	asm volatile("movb %0, %%al"::"r"(ch));		   //ch为ASCII码
	asm volatile("movw %ax, %gs:(%edi) ");             //写显存
}

void write(char ch){
	putChar(ch);
	if(ch == '\n'||col == 80){
		row++;
		col = 0;
	}
	if(ch != '\n')	
		print_vedio(ch, row, col++);
}

void memcpy(uint32_t dst, uint32_t src, uint32_t size){
	for(int i=0;i<size;i++){
		*(uint8_t*)(dst) = *(uint8_t*)(src);
		dst++;
		src++;	
	}
}

uint32_t getKeyCode(struct TrapFrame *tf) {
    char code = inByte(0x60);
	char buf[100];
	int index = 0;
	do{
		if(inByte(0x60) != code){
			code = inByte(0x60);
			if(code > 0 && code != 0x1c){
			 	buf[index] = code;
				index ++;
				for (int i = 0; i < 26; i ++) {
					if (letter_code[i] == code) {
						write('a'+i);
					}
				}
			}
			else if(code == 0x1c){
				write('\n');
			 	int32_t val = inByte(0x61);
 	  	 		outByte(0x61, val | 0x80);
    			outByte(0x61, val);		
				if(tf -> ecx == 0)		
					return buf[0];
				else if(tf -> ecx == 1){
					for(int i = 0; i<index;i++){
						for (int j = 0; j < 26; j ++) {
							if (letter_code[j] == buf[i]) {
								buf[i] = 'a'+j;
							}
						}
					}
					buf[index] = '\0';
					memcpy(tf->edx, (uint32_t)&buf[0],index+1);	
					return 0;
				}
			}
		}
	}while(1);
}

void sys_read(struct TrapFrame *tf){
	if(tf->ecx == 0){
		int scan_code = getKeyCode(tf);
		int flag = 0;
		for (int i = 0; i < 26; i ++) {
			if (letter_code[i] == scan_code) {
				flag = 1;
				tf -> eax = 'a'+i;
			}
		}
		if(flag == 0)
			tf->eax = 'E';
	}
	else if(tf->ecx == 1){
		getKeyCode(tf);
		tf -> eax = tf -> edx;
	}
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
	write(ch);
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

void sys_init(struct TrapFrame *tf){
	tf->ecx += ((current_pcb-pcb)-1) * PROCESS_SIZE;
	uint32_t *sem_addr = (uint32_t *)tf->ecx;
	sem[sem_index].value = tf -> edx;
	*sem_addr = sem_index;
	//Log_hex("sem_index",sem_index);	

	sem_index ++;	//for next initialize

	assert(sem_index < 10);

	tf->eax = 0;
}

void sys_p(struct TrapFrame *tf){
	putChar('P');
	putChar('\n');
	tf->ecx += ((current_pcb-pcb)-1) * PROCESS_SIZE;
	uint32_t sem_idx = *(uint32_t *)tf->ecx;
	
	assert(sem_idx == 0);

	P(sem_idx);
	tf->eax = 0;
}

void sys_v(struct TrapFrame *tf){
	putChar('V');
	putChar('\n');
	tf->ecx += ((current_pcb-pcb)-1) * PROCESS_SIZE;
	uint32_t sem_idx = *(uint32_t *)tf->ecx;
	
	assert(sem_idx == 0);

	V(sem_idx);
	tf->eax = 0;
}

void sys_destroy(struct TrapFrame *tf){
	tf->ecx += ((current_pcb-pcb)-1) * PROCESS_SIZE;
	uint32_t sem_idx = *(uint32_t *)tf->ecx;
	sem[sem_idx].value = 0;
	sem[sem_idx].list = NULL;

	sem_index --;	//for next initialize
	//assert(sem_index >= 0);

	tf->eax = 0;
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
		case SYS_read: sys_read(tf); break;
		case SYS_write: sys_write(tf); break;
		case SYS_fork: sys_fork(tf); break;
		case SYS_sleep: sys_sleep(tf); break;
		case SYS_exit: sys_exit(tf); break;
		case SYS_pvinit: sys_init(tf); break;
		case SYS_P: sys_p(tf); break;
		case SYS_V: sys_v(tf); break;
		case SYS_pvdsty: sys_destroy(tf); break; 
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
