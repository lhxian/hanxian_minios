#include "boot.h"
#include"kelf.h"
#include"kint.h"
#define BEGIN_ADDR 0x100000
#define MID_MAX_SZ (0x80000)
#define BUFFER_BEGIN (BEGIN_ADDR + MID_MAX_SZ)
#define SECTSIZE 512
#define MID_MAX_SECT_CNT (MID_MAX_SZ/ SECTSIZE)
static inline void rmemcpy(uint8_t* dst,const uint8_t* src,int sz){
	while(sz-->0) dst[sz] = src[sz];
}
static inline void rmemset(uint8_t* dst,uint8_t val,int sz){
	while(sz-- > 0) dst[sz] = val;
}
typedef void(*f_entry)();
void bootMid(void* a){
	word_t mid_addr = BEGIN_ADDR, buffer_addr = BUFFER_BEGIN;
	for(int i =0;i<MID_MAX_SECT_CNT;++i){
		readSect((void*)buffer_addr,i+1);
		buffer_addr += SECTSIZE;
	}
	ELFHeader* pheader = (ELFHeader*)BUFFER_BEGIN;
	ProgramHeader* pro_header = (ProgramHeader*)(pheader->phoff);
	for(int i =0;i < pheader->phnum;++i){
		if(pro_header[i].type == PT_LOAD){
			uint8_t* addr = (uint8_t*)(BEGIN_ADDR + pro_header[i].vaddr);
			rmemcpy(addr,
			(uint8_t*)(BUFFER_BEGIN+pro_header[i].off),pro_header[i].filesz);
			rmemset(addr+pro_header[i].filesz,0,pro_header[i].memsz - pro_header[i].filesz);
		}
	}
	f_entry entry = (f_entry)pheader->entry; // mid entry;
	entry();
}


void waitDisk(void) { // waiting for disk
	while((inByte(0x1F7) & 0xC0) != 0x40);
}

void readSect(void *dst, int offset) { // reading a sector of disk
	waitDisk();
	outByte(0x1F2, 1);
	outByte(0x1F3, offset);
	outByte(0x1F4, offset >> 8);
	outByte(0x1F5, offset >> 16);
	outByte(0x1F6, (offset >> 24) | 0xE0);
	outByte(0x1F7, 0x20);

	waitDisk();
	for (int i = 0; i < SECTSIZE / 4; i ++) {
		((int *)dst)[i] = inLong(0x1F0);
	}
}
