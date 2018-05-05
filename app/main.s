
main.o:     file format elf32-i386


Disassembly of section .group:

00000000 <.group>:
   0:	01 00                	add    %eax,(%eax)
   2:	00 00                	add    %al,(%eax)
   4:	07                   	pop    %es
   5:	00 00                	add    %al,(%eax)
	...

Disassembly of section .text:

00000000 <uEntry>:
   0:	55                   	push   %ebp
   1:	89 e5                	mov    %esp,%ebp
   3:	57                   	push   %edi
   4:	56                   	push   %esi
   5:	53                   	push   %ebx
   6:	83 ec 0c             	sub    $0xc,%esp
   9:	e8 fc ff ff ff       	call   a <uEntry+0xa>
   e:	81 c3 02 00 00 00    	add    $0x2,%ebx
  14:	e8 fc ff ff ff       	call   15 <uEntry+0x15>
  19:	85 c0                	test   %eax,%eax
  1b:	75 53                	jne    70 <uEntry+0x70>
  1d:	c7 83 00 00 00 00 02 	movl   $0x2,0x0(%ebx)
  24:	00 00 00 
  27:	b8 02 00 00 00       	mov    $0x2,%eax
  2c:	bf 08 00 00 00       	mov    $0x8,%edi
  31:	8d b3 00 00 00 00    	lea    0x0(%ebx),%esi
  37:	eb 09                	jmp    42 <uEntry+0x42>
  39:	8d 76 00             	lea    0x0(%esi),%esi
  3c:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
  42:	4f                   	dec    %edi
  43:	51                   	push   %ecx
  44:	57                   	push   %edi
  45:	50                   	push   %eax
  46:	56                   	push   %esi
  47:	e8 fc ff ff ff       	call   48 <uEntry+0x48>
  4c:	c7 04 24 80 00 00 00 	movl   $0x80,(%esp)
  53:	e8 fc ff ff ff       	call   54 <uEntry+0x54>
  58:	83 c4 10             	add    $0x10,%esp
  5b:	85 ff                	test   %edi,%edi
  5d:	75 dd                	jne    3c <uEntry+0x3c>
  5f:	e8 fc ff ff ff       	call   60 <uEntry+0x60>
  64:	31 c0                	xor    %eax,%eax
  66:	8d 65 f4             	lea    -0xc(%ebp),%esp
  69:	5b                   	pop    %ebx
  6a:	5e                   	pop    %esi
  6b:	5f                   	pop    %edi
  6c:	5d                   	pop    %ebp
  6d:	c3                   	ret    
  6e:	66 90                	xchg   %ax,%ax
  70:	40                   	inc    %eax
  71:	74 f1                	je     64 <uEntry+0x64>
  73:	c7 83 00 00 00 00 01 	movl   $0x1,0x0(%ebx)
  7a:	00 00 00 
  7d:	b8 01 00 00 00       	mov    $0x1,%eax
  82:	bf 08 00 00 00       	mov    $0x8,%edi
  87:	8d b3 00 00 00 00    	lea    0x0(%ebx),%esi
  8d:	eb 07                	jmp    96 <uEntry+0x96>
  8f:	90                   	nop
  90:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
  96:	4f                   	dec    %edi
  97:	52                   	push   %edx
  98:	57                   	push   %edi
  99:	50                   	push   %eax
  9a:	56                   	push   %esi
  9b:	e8 fc ff ff ff       	call   9c <uEntry+0x9c>
  a0:	c7 04 24 80 00 00 00 	movl   $0x80,(%esp)
  a7:	e8 fc ff ff ff       	call   a8 <uEntry+0xa8>
  ac:	83 c4 10             	add    $0x10,%esp
  af:	85 ff                	test   %edi,%edi
  b1:	75 dd                	jne    90 <uEntry+0x90>
  b3:	eb aa                	jmp    5f <uEntry+0x5f>

Disassembly of section .bss:

00000000 <data>:
   0:	00 00                	add    %al,(%eax)
	...

Disassembly of section .rodata.str1.1:

00000000 <.LC0>:
   0:	43                   	inc    %ebx
   1:	68 69 6c 64 20       	push   $0x20646c69
   6:	50                   	push   %eax
   7:	72 6f                	jb     78 <.LC1+0x5b>
   9:	63 65 73             	arpl   %sp,0x73(%ebp)
   c:	73 3a                	jae    48 <.LC1+0x2b>
   e:	20 50 6f             	and    %dl,0x6f(%eax)
  11:	6e                   	outsb  %ds:(%esi),(%dx)
  12:	67 20 25             	and    %ah,(%di)
  15:	64 2c 20             	fs sub $0x20,%al
  18:	25 64 3b 0a 00       	and    $0xa3b64,%eax

0000001d <.LC1>:
  1d:	46                   	inc    %esi
  1e:	61                   	popa   
  1f:	74 68                	je     89 <.LC1+0x6c>
  21:	65 72 20             	gs jb  44 <.LC1+0x27>
  24:	50                   	push   %eax
  25:	72 6f                	jb     96 <.LC1+0x79>
  27:	63 65 73             	arpl   %sp,0x73(%ebp)
  2a:	73 3a                	jae    66 <.LC1+0x49>
  2c:	20 50 69             	and    %dl,0x69(%eax)
  2f:	6e                   	outsb  %ds:(%esi),(%dx)
  30:	67 20 25             	and    %ah,(%di)
  33:	64 2c 20             	fs sub $0x20,%al
  36:	25 64 3b 0a 00       	and    $0xa3b64,%eax

Disassembly of section .text.__x86.get_pc_thunk.bx:

00000000 <__x86.get_pc_thunk.bx>:
   0:	8b 1c 24             	mov    (%esp),%ebx
   3:	c3                   	ret    

Disassembly of section .comment:

00000000 <.comment>:
   0:	00 47 43             	add    %al,0x43(%edi)
   3:	43                   	inc    %ebx
   4:	3a 20                	cmp    (%eax),%ah
   6:	28 44 65 62          	sub    %al,0x62(%ebp,%eiz,2)
   a:	69 61 6e 20 36 2e 33 	imul   $0x332e3620,0x6e(%ecx),%esp
  11:	2e 30 2d 31 38 2b 64 	xor    %ch,%cs:0x642b3831
  18:	65 62 39             	bound  %edi,%gs:(%ecx)
  1b:	75 31                	jne    4e <.LC1+0x31>
  1d:	29 20                	sub    %esp,(%eax)
  1f:	36 2e 33 2e          	ss xor %cs:(%esi),%ebp
  23:	30 20                	xor    %ah,(%eax)
  25:	32 30                	xor    (%eax),%dh
  27:	31 37                	xor    %esi,(%edi)
  29:	30                   	.byte 0x30
  2a:	35                   	.byte 0x35
  2b:	31 36                	xor    %esi,(%esi)
	...

Disassembly of section .eh_frame:

00000000 <.eh_frame>:
   0:	14 00                	adc    $0x0,%al
   2:	00 00                	add    %al,(%eax)
   4:	00 00                	add    %al,(%eax)
   6:	00 00                	add    %al,(%eax)
   8:	01 7a 52             	add    %edi,0x52(%edx)
   b:	00 01                	add    %al,(%ecx)
   d:	7c 08                	jl     17 <.eh_frame+0x17>
   f:	01 1b                	add    %ebx,(%ebx)
  11:	0c 04                	or     $0x4,%al
  13:	04 88                	add    $0x88,%al
  15:	01 00                	add    %eax,(%eax)
  17:	00 2c 00             	add    %ch,(%eax,%eax,1)
  1a:	00 00                	add    %al,(%eax)
  1c:	1c 00                	sbb    $0x0,%al
  1e:	00 00                	add    %al,(%eax)
  20:	00 00                	add    %al,(%eax)
  22:	00 00                	add    %al,(%eax)
  24:	b5 00                	mov    $0x0,%ch
  26:	00 00                	add    %al,(%eax)
  28:	00 41 0e             	add    %al,0xe(%ecx)
  2b:	08 85 02 42 0d 05    	or     %al,0x50d4202(%ebp)
  31:	46                   	inc    %esi
  32:	87 03                	xchg   %eax,(%ebx)
  34:	86 04 83             	xchg   %al,(%ebx,%eax,4)
  37:	05 02 61 0a c3       	add    $0xc30a6102,%eax
  3c:	41                   	inc    %ecx
  3d:	c6 41 c7 41          	movb   $0x41,-0x39(%ecx)
  41:	c5 0c 04             	lds    (%esp,%eax,1),%ecx
  44:	04 43                	add    $0x43,%al
  46:	0b 00                	or     (%eax),%eax
  48:	10 00                	adc    %al,(%eax)
  4a:	00 00                	add    %al,(%eax)
  4c:	4c                   	dec    %esp
  4d:	00 00                	add    %al,(%eax)
  4f:	00 00                	add    %al,(%eax)
  51:	00 00                	add    %al,(%eax)
  53:	00 04 00             	add    %al,(%eax,%eax,1)
  56:	00 00                	add    %al,(%eax)
  58:	00 00                	add    %al,(%eax)
	...
