#include <stdio.h>
#include <stdlib.h>
#include "idt_handlers.h"
#include "i8259.h"

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


// void keyboard_interrupt(){
//     struct KeyScanCodePair keyScanCodeArray[] = {
//         {'A', 0x1C},
//         {'B', 0x32},
//         {'C', 0x21},
//         {'D', 0x23},
//         {'E', 0x24},
//         {'F', 0x2B},
//         {'G', 0x34},
//         {'H', 0x33},
//         {'I', 0x43},
//         {'J', 0x3B},
//         {'K', 0x42},
//         {'L', 0x4B},
//         {'M', 0x3A},
//         {'N', 0x31},
//         {'O', 0x44},
//         {'P', 0x4D},
//         {'Q', 0x15},
//         {'R', 0x2D},
//         {'S', 0x1B},
//         {'T', 0x2C},
//         {'U', 0x3C},
//         {'V', 0x2A},
//         {'W', 0x1D},
//         {'X', 0x22},
//         {'Y', 0x35},
//         {'Z', 0x1A}
//     };

// enable_irq(uint32_t irq_num);



// }

// void rtc_interrupt(){

// }

// void pic_interrupt(){


// }
// void system_call(){

// }

