#include "x86.h"
#include "device.h"

SegDesc gdt[NR_SEGMENTS];
TSS tss;

#define SECTSIZE 512
#define PT_LOAD 1

void waitDisk(void) {
	while((inByte(0x1F7) & 0xC0) != 0x40); 
}

void readSect(void *dst, int offset) {
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

void initSeg() {
	gdt[SEG_KCODE] = SEG(STA_X | STA_R, 0,       0xffffffff, DPL_KERN);
	gdt[SEG_KDATA] = SEG(STA_W,         0,       0xffffffff, DPL_KERN);
	gdt[SEG_UCODE1] = SEG(STA_X | STA_R, 0,       0xffffffff, DPL_USER);
	gdt[SEG_UDATA1] = SEG(STA_W,         0,       0xffffffff, DPL_USER);
	gdt[SEG_UCODE2] = SEG(STA_X | STA_R, 200000,       0xffffffff, DPL_USER);
	gdt[SEG_UDATA2] = SEG(STA_W,         200000,       0xffffffff, DPL_USER);
	gdt[SEG_TSS] = SEG16(STS_T32A,      &tss, sizeof(TSS)-1, DPL_KERN);
	gdt[SEG_TSS].s = 0;//s=0 means system
	gdt[SEG_VIDEO] = SEG(STA_W,    0xb8000,       0xffffffff, DPL_KERN);
	setGdt(gdt, sizeof(gdt));

	/*
	 * 初始化TSS
	 */
	tss.esp0 = 0x200000; 
	tss.ss0 = KSEL(SEG_KDATA);
	asm volatile("ltr %%ax":: "a" (KSEL(SEG_TSS)));

	/*设置正确的段寄存器*/
	asm volatile("movl %0, %%eax":: "r"(KSEL(SEG_KDATA)));//kernel
	asm volatile("movw    %ax, %ds");
	asm volatile("movw    %ax, %es");
	asm volatile("movw    %ax, %ss");
	asm volatile("movw    %ax, %fs");
	lLdt(0);
	
}

void loadUMain(void) {

	/*加载用户程序至内存*/
	struct ELFHeader *elf;
	struct ProgramHeader *ph, *eph;
	uint8_t *buf=(uint8_t *)0x6000000;

	for(int i = 1; i <= 200; i++){
		readSect((void *)(buf + 512 * (i - 1)), i+200);
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
	enterUserSpace(elf->entry);
	
}
