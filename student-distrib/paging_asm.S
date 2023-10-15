#define ASM     1


.globl loadPageDirectory_asm 
.globl enablePaging_asm
.globl loadPSE



loadPageDirectory_asm:
    pushl %ebp
    movl %esp, %ebp
    movl 8(%esp), %eax
    movl %eax, %cr3

    movl %cr0, %eax
    orl $0x80000001, %eax
    movl %eax, %cr0

    movl %cr4, %eax
    orl  $0x00000010, %eax
    movl %eax, %cr4

    movl %ebp, %esp


    popl %ebp
    ret

// enablePaging_asm:
//     pushl %ebp
//     movl %esp, %ebp
  
//     movl %ebp, %esp
//     popl %ebp
//     ret

// loadPSE:    
//     pushl %ebp
//     movl %esp, %ebp
   
//     popl %ebp
//     ret
