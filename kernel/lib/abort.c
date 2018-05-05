#include "common.h"
#include "x86.h"
#include "device.h"

static char *i2A(int a) {
	static char buf[30];
	char *p = buf + sizeof(buf) - 1;
	do {
		*--p = '0' + a % 10;
	} while (a /= 10);
	return p;
}

static char *i2H(int a) {
	static char buf[30];
	char *p = buf + sizeof(buf) - 1;
	do {
		if(a % 16 < 10)
			*--p = '0' + a % 16;
		else
			*--p = 'a' + a % 16 - 10;
	} while (a /= 16);
	
	return p;
}

static void append(char **p, const char *str) {
	while (*str) {
		*((*p) ++) = *str ++;
	}
}

/* 将文件名和assert fail的行号显示在屏幕上 */
#define BLUE_SCREEN_TEXT "Assertion failed: "
static void displayMessage(const char *file, int line) {
	static char buf[256] = BLUE_SCREEN_TEXT;
	char *p = buf + sizeof(BLUE_SCREEN_TEXT) - 1;

	append(&p, file);
	append(&p, ":");
	append(&p, i2A(line));
	append(&p, "\n");

	for (p = buf; *p; p ++) {
		putChar(*p);
	}
}

#define LOG_TEXT "LOG: "
void Log_int(const char *name, int data) {
	char buf[256] = LOG_TEXT;
	char *p = buf + sizeof(LOG_TEXT) - 1;

	append(&p, name);
	append(&p, ":");
	append(&p, i2A(data));
	append(&p, "\n");

	for (p = buf; *p; p ++) {
		putChar(*p);
	}
}

void Log_hex(const char *name, int data) {
	char buf[256] = LOG_TEXT;
	char *p = buf + sizeof(LOG_TEXT) - 1;

	append(&p, name);
	append(&p, ": 0x");
	append(&p, i2H(data));
	append(&p, "\n");

	for (p = buf; *p; p ++) {
		putChar(*p);
	}
}

void Panic(const char *info) {
	char buf[256] = "";
	char *p = buf;

	append(&p, info);

	for (p = buf; *p; p ++) {
		putChar(*p);
	}
}

int abort(const char *fname, int line) {
	disableInterrupt();
	displayMessage(fname, line);
	while (TRUE) {
		waitForInterrupt();
	}
}
