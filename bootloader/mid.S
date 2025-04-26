.code16
.global mid_entry
.equ DATA_BEGIN 0x1000
.equ META_DATA_BEGIN 0x4000
.equ STACK_TOP 0xffff
mid_entry:
    movw DATA_BEGIN, %ax
    movw %ax, %ds
    movw %ax, %ss
    movw STACK_TOP, %sp

    # get the physical memory size and disk size;
    xorl %ebx, %ebx
    movw META_DATA_BEGIN, %ax
    movw %ax, %es
    movw $0x20, %di
    xorw %cx, %cx
    movw %cx, %es:(0x0) 
    movw %cx, %es:(0x2)
.get_ram_loop:
    xorl %eax, %eax
    movw $0xe820, %ax   #movl $0xe820, %eax
    
    movw $0x534d, %dx    #movl $0x534d4150, %edx
    shll $16, %edx
    movw $0x4150, %dx
    movl $24, %ecx

    int 0x15

    jc .done
    cmp $0x4150 ,%ax
    jne .done
    shrl $16, %eax
    cmp $0x534d, %ax
    jne .done
    addw $24, %dx # to next struct
    movl %ebx, %esi
    xorl %esi, %esi
    cmp %esi, %ebx
    je .done
    # update the meta data;
    movl %ebx, %esi
    movw %si, %es:(0x0)
    shrl %16, %esi
    movw %si, %es:(0x2)
    
    jmp get_ram_loop
    
.done:
    data32 addr32 lgdt gdtDesc
    movl %cr0, %eax
    orb $0x1, %al
    movl %eax, %cr0
    data32 ljmp $0x8, $start32


.code32
start32:
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %ss
    movw %ax, %gs

.p2align 2
gdt:
    .word 0,0
    .byte 0,0,0,0
    # code
    .word 0xffff, 0
    .byte 0,0x9a,0xcf,0
    #data
    .word 0xffff, 0
    .byte 0,0x92,0xcf,0

gdtDesc:
    .word (gdtDesc - gdt)
    .long gdt