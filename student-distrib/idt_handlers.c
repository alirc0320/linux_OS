#include <stdio.h>
#include <stdlib.h>
#include "idt_handlers.h"
#include "i8259.h"
#include "Device_init.c"

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


// /* Interrupt masks to determine which interrupts are enabled and disabled */
// uint8_t master_mask; /* IRQs 0-7  */
// uint8_t slave_mask;  /* IRQs 8-15 */

// /* Initialize the 8259 PIC */
// void i8259_init(void) {
// }

// /* Enable (unmask) the specified IRQ */
// void enable_irq(uint32_t irq_num) {
// }

// /* Disable (mask) the specified IRQ */
// void disable_irq(uint32_t irq_num) {
// }

// /* Send end-of-interrupt signal for the specified IRQ */
// void send_eoi(uint32_t irq_num) {
// }

unsigned char keyScanCodeArray_one[85] = {
    "", 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x2D, 0x3D, 0x08, 0x09, 
    0x71, 0x77, 0x65, 0x72, 0x74, 0x79, 0x75, 0x69, 0x6F, 0x70, 0x5B, 0x5D, 0x0D, 0x1D, 0x61, 
    0x73, 0x64, 0x66, 0x67, 0x68, 0x6A, 0x6B, 0x6C, 0x3B, 0x27, 0x60, 0x2A, 0x5C, 0x7A, 0x78, 
    0x63, 0x76, 0x62, 0x6E, 0x6D, 0x2C, 0x2E, 0x2F, 0x36, 0x37, 0x38, 0x20, 0x3A, 0x3B, 0x3C, 
    0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 
    0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x57, 0x58
};

keyScanCodeArray_two[59] = {};
keyScanCodeArray_three[59] = {};
void keyboard_interrupt(){
     

enable_irq(uint32_t irq_num);



}

void rtc_interrupt(){

}

void pic_interrupt(){


}
void system_call(){

}

