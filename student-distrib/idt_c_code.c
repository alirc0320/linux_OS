#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "idt_header_file.h"

// Signal handler function
void division_error_handler() {
        printf("Division Error Exception Caught!\n");
        while(1);
}

void debug_handler() {
    printf("Debug Exception Caught!\n");
    while (1);
}

void non_maskable_interrupt_handler() {
    printf("Non-Maskable Interrupt Exception Caught!\n");
    while (1);
}

void breakpoint_handler() {
    printf("Breakpoint Exception Caught!\n");
    while (1);
}

void overflow_handler() {
    printf("Overflow Exception Caught!\n");
    while (1);
}

void bound_range_exceeded_handler() {
    printf("Bound Range Exceeded Exception Caught!\n");
    while (1);
}

void invalid_opcode_handler() {
    printf("Invalid Opcode Exception Caught!\n");
    while (1);
}

void device_not_available_handler() {
    printf("Device Not Available Exception Caught!\n");
    while (1);
}

void double_fault_handler() {
    printf("Double Fault Exception Caught!\n");
    while (1);
}

void invalid_tss_handler() {
    printf("Invalid TSS Exception Caught!\n");
    while (1);
}

void segment_not_present_handler() {
    printf("Segment Not Present Exception Caught!\n");
    while (1);
}

void stack_segment_fault_handler() {
    printf("Stack-Segment Fault Exception Caught!\n");
    while (1);
}

void general_protection_fault_handler() {
    printf("General Protection Fault Exception Caught!\n");
    while (1);
}

void page_fault_handler() {
    printf("Page Fault Exception Caught!\n");
    while (1);
}

void x87_floating_point_exception_handler() {
    printf("x87 Floating-Point Exception Caught!\n");
    while (1);
}

void alignment_check_handler() {
    printf("Alignment Check Exception Caught!\n");
    while (1);
}

void machine_check_handler() {
    printf("Machine Check Exception Caught!\n");
    while (1);
}

void SIMD_floating_point_exception_handler() {
    printf("SIMD Floating-Point Exception Caught!\n");
    while (1);
}

void virtualization_exception() {
    printf("Virtualization Exception Caught!\n");
    while (1);
}

void control_protection_exception_handler() {
    printf("Control Protection Exception Caught!\n");
    while (1);
}

void hypervisor_exception_handler() {
    printf("Hypervisor Injection Exception Caught!\n");
    while (1);
}

void vmm_communication_exception_handler() {
    printf("VMM Communication Exception Caught!\n");
    while (1);
}

void security_exception_handler() {
    printf("Security Exception Caught!\n");
    while (1);
}




// typedef union idt_desc_t {
//     uint32_t val[2];
//     struct {
//         uint16_t offset_15_00;
//         uint16_t seg_selector;
//         uint8_t  reserved4;
//         uint32_t reserved3 : 1;
//         uint32_t reserved2 : 1;
//         uint32_t reserved1 : 1;
//         uint32_t size      : 1;
//         uint32_t reserved0 : 1;
//         uint32_t dpl       : 2;
//         uint32_t present   : 1;
//         uint16_t offset_31_16;
//     } __attribute__ ((packed));
// } idt_desc_t;

// Function to initialize the IDT
void initialize_idt() {

    idt_exception_init.offset_15_00 = 0;
    idt_exception_init.seg_selector = KERNEL_CS ;
    idt_exception_init.reserved4 = 1;
    idt_exception_init.reserved3 = 1;
    idt_exception_init.reserved2 = 1;
    idt_exception_init.reserved1 = 1;
    idt_exception_init.size = 1;
    idt_exception_init.reserved0 = 0;
    idt_exception_init.dpl = 0;
    idt_exception_init.present = 1;
    idt_exception_init.offset_31_16 = 0;

    SET_IDT_ENTRY(idt_exception_init, division_error_handler); 
    idt[0] = idt_exception_init; 
    
    SET_IDT_ENTRY(idt_exception_init, debug_handler); 
    idt[1] = idt_exception_init; 

    SET_IDT_ENTRY(idt_exception_init, non_maskable_interrupt_handler); 
    idt[2] = idt_exception_init; 

    SET_IDT_ENTRY(idt_exception_init, breakpoint_handler); 
    idt[3] = idt_exception_init; 

    SET_IDT_ENTRY(idt_exception_init, overflow_handler); 
    idt[4] = idt_exception_init; 
    
    SET_IDT_ENTRY(idt_exception_init, bound_range_exceeded_handler); 
    idt[5] = idt_exception_init; 
    
    SET_IDT_ENTRY(idt_exception_init, invalid_opcode_handler); 
    idt[6] = idt_exception_init; 

    SET_IDT_ENTRY(idt_exception_init, device_not_available_handler); 
    idt[7] = idt_exception_init; 
    
    SET_IDT_ENTRY(idt_exception_init, double_fault_handler); 
    idt[8] = idt_exception_init; 
    
    SET_IDT_ENTRY(idt_exception_init, invalid_tss_handler); 
    idt[9] = idt_exception_init; 
    
    SET_IDT_ENTRY(idt_exception_init, segment_not_present_handler); 
    idt[10] = idt_exception_init; 
    
    SET_IDT_ENTRY(idt_exception_init, stack_segment_fault_handler); 
    idt[11] = idt_exception_init; 
    
    SET_IDT_ENTRY(idt_exception_init, general_protection_fault_handler); 
    idt[12] = idt_exception_init; 
    
    SET_IDT_ENTRY(idt_exception_init, page_fault_handler);
    idt[13] = idt_exception_init; 

    SET_IDT_ENTRY(idt_exception_init, x87_floating_point_exception_handler);
    idt[14] = idt_exception_init; 

    SET_IDT_ENTRY(idt_exception_init, alignment_check_handler); 
    idt[15] = idt_exception_init; 

    SET_IDT_ENTRY(idt_exception_init, machine_check_handler); 
    idt[16] = idt_exception_init; 

    SET_IDT_ENTRY(idt_exception_init, SIMD_floating_point_exception_handler); 
    idt[17] = idt_exception_init;
    
    SET_IDT_ENTRY(idt_exception_init, virtualization_exception);
    idt[18] = idt_exception_init;
   
    SET_IDT_ENTRY(idt_exception_init, control_protection_exception_handler); 
    idt[19] = idt_exception_init; 
    
    SET_IDT_ENTRY(idt_exception_init, hypervisor_exception_handler); 
    idt[20] = idt_exception_init;
    
    SET_IDT_ENTRY(idt_exception_init, vmm_communication_exception_handler); 
    idt[21] = idt_exception_init; 

    SET_IDT_ENTRY(idt_exception_init, security_exception_handler);
    idt[22] = idt_exception_init; 







}
