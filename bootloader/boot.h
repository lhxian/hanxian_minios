#ifndef BOOT_H
#define BOOT_H
#include"kint.h"


void waitDisk(void);

void readSect(void *dst, int offset);

/* I/O functions */
static inline char inByte(short port) {
	char data;
	asm volatile("in %1,%0" : "=a" (data) : "d" (port));
	return data;
}

static inline int inLong(short port) {
	int data;
	asm volatile("in %1, %0" : "=a" (data) : "d" (port));
	return data;
}

static inline void outByte(short port, char data) {
	asm volatile("out %0,%1" : : "a" (data), "d" (port));
}

typedef struct PMemInfo{
	uint64_t baseaddr;
	uint64_t lenght;
	uint32_t type;
	uint32_t option;
}PMemInfo;

#endif
