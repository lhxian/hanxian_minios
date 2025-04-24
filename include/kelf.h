#ifndef ELF_H
#define  ELF_H
typedef struct ELFHeader {
	unsigned int   magic;
	unsigned char  elf[12];
	unsigned short type;
	unsigned short machine;
	unsigned int   version;
	unsigned int   entry;
	unsigned int   phoff;
	unsigned int   shoff;
	unsigned int   flags;
	unsigned short ehsize;
	unsigned short phentsize;
	unsigned short phnum;
	unsigned short shentsize;
	unsigned short shnum;
	unsigned short shstrndx;
}ELFHeader ;

/* ELF32 Program header */
typedef struct ProgramHeader {
	unsigned int type;
	unsigned int off;
	unsigned int vaddr;
	unsigned int paddr;
	unsigned int filesz;
	unsigned int memsz;
	unsigned int flags;
	unsigned int align;
}ProgramHeader ;

#define PT_NULL	0x00000000
#define PT_LOAD	0x00000001
#define PT_DYNAMIC	0x00000002
#define PT_INTERP	0x00000003
#define PT_NOTE	0x00000004
#define PT_SHLIB	0x00000005
#define PT_PHDR	0x00000006
#define PT_TLS	0x00000007
#endif