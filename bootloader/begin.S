.code16

.gloabl start
start:
	movw %cs, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %ss
    cli

	inb $0x92, %al # Fast setup A20 Line with port 0x92, necessary or not?
	orb $0x02, %al
	outb %al, $0x92
    

