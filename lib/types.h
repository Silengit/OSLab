#ifndef __TYPES_H__
#define __TYPES_H__

#define	SYS_read	3
#define	SYS_write	4
#define	SYS_fork	2
#define	SYS_sleep	88
#define	SYS_exit	1
#define SYS_pvinit	49
#define	SYS_P		50
#define	SYS_V		51
#define SYS_pvdsty	52

#define NULL		((void*) 0)
typedef unsigned int   uint32_t;
typedef          int   int32_t;
typedef unsigned short uint16_t;
typedef          short int16_t;
typedef unsigned char  uint8_t;
typedef          char  int8_t;
typedef unsigned char  boolean;

typedef uint32_t size_t;
typedef uint32_t sem_t;
typedef int32_t  pid_t;



#endif
