#define ASM  1

.globl context_switch

/* void context_switch(uint32_t* save_esp, uint32_t new_esp)
 * Inputs: save_esp - where to store the current kernel stack pointer
 *         new_esp  - kernel stack pointer previously saved by context_switch
 * Outputs: None
 * Function: saves the callee-saved registers on the current kernel stack,
 *           stores esp, switches to the other stack and restores its registers.
 *           The "ret" returns into whoever called context_switch on that stack.
 */
context_switch:
    pushl %ebp
    pushl %ebx
    pushl %esi
    pushl %edi
    movl 20(%esp), %eax                 # save_esp (4 regs + return address above it)
    movl %esp, (%eax)
    movl 24(%esp), %esp                 # new_esp
    popl %edi
    popl %esi
    popl %ebx
    popl %ebp
    ret
