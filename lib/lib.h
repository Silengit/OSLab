#ifndef __LIB_H__
#define __LIB_H__
#include "types.h"

void printf(const char *format,...);
int getchar();
int gets(char *str);
int fork();
int sleep(unsigned int time);
int exit();
int sem_init(sem_t *sem, unsigned int value);
int sem_post(sem_t *sem);
int sem_wait(sem_t *sem);
int sem_destroy(sem_t *sem);

#endif
