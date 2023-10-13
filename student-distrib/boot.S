# boot.S - start point for the kernel after GRUB gives us control
# vim:ts=4 noexpandtab

#define ASM     1


#include "multiboot.h"
#include "x86_desc.h"

.text

    # Multiboot header (required for GRUB to boot us)
    .long MULTIBOOT_HEADER_MAGIC
    .long MULTIBOOT_HEADER_FLAGS
    .long -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
    

# Entrypoint to the kernel
.globl start, _start

.align 4
start:
_start:
    # Make sure interrupts are off
    cli
    jmp     continue

continue:
# Load the GDT
LGDT gdt_desc 

                                                        

    /* 
                REGISTERS       INSTR.(Ld/Str)          SIZE        PROPERTIES                                                 PURPOSE

                GDTR            LGDT,SGDT               48          holds base addr(32),table limit(16)                        The data structure for the global descriptor table (public)
                LDTR            LLDT,SLDT               64          holds base addr(32),table limit(16),seg selector(16)       The memory table for local descriptor table (private)
                IDTR            LIDT,SIDT               48          holds base addr(32),table limit(16)                        The data structure for the interrupt descriptor table 
                TR              LTR,STR                 64          holds base addr(32),seg limit(16),seg selector(16)         The memory table for facilitating task shifting
    */



    # pages are 2.4.1, 3.5.1, mp2 documentation page 22 
        
    # initialize the IDT
    # For entries 0x14 to 0x1F in the IDT, you need not fill in anything for the IDT
    #LGDT & SGDT LOAD AND STORE GDTR 
    # set the base addr on powerUp and reset to 0 and limit to FFFF so, GDTR is 0x00000000FFFF (bc 4096 bits)
    #GDTR - 48 bit register - a pointer to the GDT 
    #LDTR - 64 bit regiter - holds pointer and index of LDT within the GDT 
    /*. On power up or reset of the processor, the base address is set to the default value of 0 and
    the limit is set to FFFFH */







    # Load CS with the new descriptor value
    ljmp    $KERNEL_CS, $keep_going

keep_going:
    # Set up ESP so we can have an initial stack
    movl    $0x800000, %esp

    # Set up the rest of the segment selector registers
    movw    $KERNEL_DS, %cx
    movw    %cx, %ss
    movw    %cx, %ds
    movw    %cx, %es
    movw    %cx, %fs
    movw    %cx, %gs

    # Push the parameters that entry() expects (see kernel.c):
    # eax = multiboot magic
    # ebx = address of multiboot info struct
    pushl   %ebx
    pushl   %eax

    # Jump to the C entrypoint to the kernel.
    call    entry

    # We'll never get back here, but we put in a hlt anyway.
halt:
    hlt
    jmp     halt



    /*
                CONTROL REG         PURPOSE

                CR0                 Stores all control flags to determine operating mode and states
                 L---> for CRO:     [PE,MP,EM,TS,ET,NE,{9 bits},WP,{1 bit},AM,{9 BITS},NW,CD,PG] } 32 bits
                CR1                 NO DEFINED USAGE (RESERVED)
                 L---> for CR3:     [{3 bits},PWT,PCD,{7 bits},Page-Directory Base] } 32 bits
                CR2                 Contains linear addr. that causes page fault, used for finding addr. that caused page fault             
                CR3                 PDBR(Page Directory Base Register). wtf is this
                CR4                 Contains additional set of flags 
                 L---> for CR4:     [VME,PVI,TSD,DE,PSE,PAE,MCE,PGE,PCE,OSFXSR,OSXMMEXCPT] } 32 bits
    */