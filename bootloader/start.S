.code16

.global start
start:
	cli                             #关闭中断
        inb $0x92, %al                  #启动A20总线
        orb $0x02, %al
        outb %al, $0x92
        data32 addr32 lgdt gdtDesc      #加载GDTR
        movl %cr0, %eax                 #启动保护模式
        orb $0x01, %al
        movl %eax, %cr0
        data32 ljmp $0x08, $start32     #长跳转切换至保护模式

.code32
start32:
	movw    $0x10, %ax
	movw    %ax, %ds          # %DS = %AX
	movw    %ax, %es          # %ES = %AX
	movw    %ax, %ss          # %SS = %AX
	movw    %ax, %fs          # %FS = %AX
	movl $0, %ebp
	movl $(128 << 20), %esp	  #init %esp
	sub $16, %esp
	jmp bootMain                    #跳转至bootMain函数 定义于boot.c

.p2align 2
gdt: # 8 bytes for each table entry, at least 1 entry
	.word 0,0 # empty entry
	.byte 0,0,0,0

	.word 0xffff,0                  #代码段描述符
        .byte 0,0x9a,0xcf,0
        
        .word 0xffff,0                  #数据段描述符
        .byte 0,0x92,0xcf,0
        

gdtDesc: # 6 bytes in total
	.word (gdtDesc - gdt -1)
	.long gdt
