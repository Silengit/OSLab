
app/main.o:     file format elf32-i386


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
  19:	40                   	inc    %eax
  1a:	74 44                	je     60 <uEntry+0x60>
  1c:	c7 83 00 00 00 00 01 	movl   $0x1,0x0(%ebx)
  23:	00 00 00 
  26:	b8 01 00 00 00       	mov    $0x1,%eax
  2b:	be 08 00 00 00       	mov    $0x8,%esi
  30:	8d bb 00 00 00 00    	lea    0x0(%ebx),%edi
  36:	eb 06                	jmp    3e <uEntry+0x3e>
  38:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
  3e:	4e                   	dec    %esi
  3f:	52                   	push   %edx
  40:	56                   	push   %esi
  41:	50                   	push   %eax
  42:	57                   	push   %edi
  43:	e8 fc ff ff ff       	call   44 <uEntry+0x44>
  48:	c7 04 24 80 00 00 00 	movl   $0x80,(%esp)
  4f:	e8 fc ff ff ff       	call   50 <uEntry+0x50>
  54:	83 c4 10             	add    $0x10,%esp
  57:	85 f6                	test   %esi,%esi
  59:	75 dd                	jne    38 <uEntry+0x38>
  5b:	e8 fc ff ff ff       	call   5c <uEntry+0x5c>
  60:	31 c0                	xor    %eax,%eax
  62:	8d 65 f4             	lea    -0xc(%ebp),%esp
  65:	5b                   	pop    %ebx
  66:	5e                   	pop    %esi
  67:	5f                   	pop    %edi
  68:	5d                   	pop    %ebp
  69:	c3                   	ret    

Disassembly of section .text.__x86.get_pc_thunk.bx:

00000000 <__x86.get_pc_thunk.bx>:
   0:	8b 1c 24             	mov    (%esp),%ebx
   3:	c3                   	ret    
