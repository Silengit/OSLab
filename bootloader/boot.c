#include "boot.h"

#define SECTSIZE 512
#define PT_LOAD 1

void bootMain(void) {
	/* 加载内核至内存，并跳转执行 */
	struct ELFHeader *elf;
	struct ProgramHeader *ph, *eph;
	uint8_t *buf=(uint8_t *)0x6000000;

	for(int i = 1; i <= 200; i++){
		readSect((void *)(buf + 512 * (i - 1)), i); 	
	}
	elf = (void *)buf;

	ph = (void *)elf + elf->phoff;
	eph = ph + elf->phnum;
	for(; ph < eph; ph ++) {
		if(ph->type == PT_LOAD) {
			unsigned int dst = ph->vaddr;
			unsigned int src = ph->off;
			unsigned int filesize = ph->filesz;
			for(unsigned int i=0;i<filesize;i++){
				*(uint8_t*)(dst) = *(uint8_t*)(buf+src);
				dst++;
				src++;	
			}
			for(unsigned int i=filesize;i<ph->memsz;i++){
				*(uint8_t*)(dst) = 0;	
				dst++;
			}	
		}
	}

	void (*entry)(void);
	entry = (void *)(elf->entry);
	entry();
}

/*void memcpy(void *dst, void *src, unsigned int data_size){
	for(unsigned int i=0;i<data_size;i++)
		*(uint8_t*)(dst++) = *(uint8_t*)(src++);
}
void memset(void *dst, uint8_t data, unsigned int data_size){
	for(unsigned int i=0;i<data_size;i++)
		*(uint8_t*)(dst++) = data;
}*/

void waitDisk(void) { // waiting for disk
	while((inByte(0x1F7) & 0xC0) != 0x40);
}

void readSect(void *dst, int offset) { // reading a sector of disk
	int i;
	waitDisk();
	outByte(0x1F2, 1);
	outByte(0x1F3, offset);
	outByte(0x1F4, offset >> 8);
	outByte(0x1F5, offset >> 16);
	outByte(0x1F6, (offset >> 24) | 0xE0);
	outByte(0x1F7, 0x20);

	waitDisk();
	for (i = 0; i < SECTSIZE / 4; i ++) {
		((int *)dst)[i] = inLong(0x1F0);
	}
}
