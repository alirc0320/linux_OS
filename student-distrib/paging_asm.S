#define ASM     1


.globl loadPageDirectory_asm 
.globl enablePaging_asm
.globl loadPSE



loadPageDirectory_asm:
    push %ebp
    mov %esp, %ebp
    mov 8(%esp), %eax
    mov %eax, %cr3
    mov %ebp, %esp
    pop %ebp
    ret

enablePaging_asm:
    push %ebp
    mov %esp, %ebp
    mov %cr0, %eax
    or $0x80000000, %eax
    mov %eax, %cr0
    mov %ebp, %esp
    pop %ebp
    ret

loadPSE:    
    push %ebp
    mov %esp, %ebp
    mov %eax, %cr4
    or  $0x00000010, %eax
    mov %cr4, %eax
    pop %ebp
    ret
