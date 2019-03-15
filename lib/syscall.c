#include "lib.h"
#include "types.h"

/*
 * io lib here
 * 库函数写在这
 */

static int32_t syscall(uint32_t eax, uint32_t ecx, uint32_t edx)
{
	int32_t ret = 0;

	//内嵌汇编 保存 eax, ecx, edx 至通用寄存器

	asm volatile("int $0x80": "=a"(ret):"a"(eax),"c"(ecx),"d"(edx));
		
	return ret;//what will ret be like?
}


int fork(){
	return syscall(SYS_fork, 0, 0);
}

int sleep(uint32_t time){
	return syscall(SYS_sleep, time, 0);
}

int exit(){
	return syscall(SYS_exit, 0, 0);
}

int sem_init(sem_t *sem, uint32_t value){
	
	//printf("sem = %d\n",*sem);
	int ret = syscall(SYS_pvinit, (uint32_t)sem, value);
	//printf("sem = %d\n",*sem);
	return ret;
}

int sem_post(sem_t *sem){
	return syscall(SYS_V, (uint32_t)sem, 0);
}

int sem_wait(sem_t *sem){
	return syscall(SYS_P, (uint32_t)sem, 0);
}

int sem_destroy(sem_t *sem){
	return syscall(SYS_pvdsty, (uint32_t)sem, 0);	
}
