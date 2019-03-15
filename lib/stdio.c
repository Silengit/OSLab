#include <stdarg.h>
#include "lib.h"
#include "types.h"

static int32_t syscall(uint32_t eax, uint32_t ecx, uint32_t edx)
{
	int32_t ret = 0;

	//内嵌汇编 保存 eax, ecx, edx 至通用寄存器

	asm volatile("int $0x80": "=a"(ret):"a"(eax),"c"(ecx),"d"(edx));
		
	return ret;//what will ret be like?
}

void printch(char c){
	syscall(SYS_write, (uint32_t)&c, 0);
}

void printstr(char *str){
	uint32_t index = 0;
	while(str[index]!='\0')
		printch(str[index++]);
}

void printint(int x){
	char buf[100];
	char *str = buf;
	int temp = x;
	int len = 0;
	int t = 0;
	uint32_t index = 0;
	if(x == 0){
		str = "0";
		syscall(SYS_write, (uint32_t)&str[0], 0);
		return;
	}
	if(x == 0x80000000){
		str = "-2147483648";
		while(str[index]!='\0')
			syscall(SYS_write, (uint32_t)&str[index++], 0);
		return;
	}
	if(x < 0){
		buf[0] = '-';
		syscall(SYS_write, (uint32_t)&buf[0], 0);
		x = -x;
	}

	while(temp != 0){
		temp /= 10;
		len++;	
	}
	for(int i = len-1;i>=0;i--){
		t = x % 10;
		x /= 10;
		buf[i] = '0' + t;
	}
	buf[len] = '\0';
	while(buf[index]!='\0')
		syscall(SYS_write, (uint32_t)&buf[index++], 0);
}

void printhex(uint32_t x){
	char buf[100];
	char *str=buf;
	uint32_t temp = x;
	int len = 0;
	int t = 0;
	uint32_t index = 0;
	if(x == 0){
		str = "0";
		syscall(SYS_write, (uint32_t)&str[0], 0);
		return;
	}

	while(temp != 0){
		temp /= 16;
		len++;	
	}
	for(int i = len-1;i>=0;i--){
		t = x % 16;
		x /= 16;
		if(t<10)
			buf[i] = '0' + t;
		else
			buf[i] = 'a' + t - 10;
	}
	buf[len] = '\0';
	while(buf[index]!='\0')
		syscall(SYS_write, (uint32_t)&buf[index++], 0);
}

void printf(const char *format,...){
	va_list vl;
	va_start(vl, format);
	if(format == NULL)
		return;
	while(*format){
		if(*format == '%'){
			switch(*(++format)){
			case 'c': printch(va_arg(vl, int));break;
			case 'd':case 'i': printint(va_arg(vl, int));break;
			case 'f': break;//maybe later
			case 's': printstr(va_arg(vl, char *));break;
			case 'b':case 'B': break;//maybe later
			case 'x':case 'X': printhex(va_arg(vl, int));break;
			case '%': printch('%');break;
			default:printch('%');printch(*format);break;
			}
		}
		else
			printch(*format);
		format++;
	}	
	va_end(vl);
}

int getchar(){
	return syscall(SYS_read, 0, 0);
}

int gets(char *str){
	return syscall(SYS_read, 1, (uint32_t)&str[0]);
}

