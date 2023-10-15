// #include <stdio.h>
// #include <stdlib.h>
// #include "x86_desc.h"
// #include "i8259.h"
#include "lib.h"
#include "idt_handlers.h"
// #include "Device_init.c"


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


unsigned char keyScanCodeArray_one[0x59] = {'\0','\0','1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', 
'=', '\0', '\0', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\0', '\0', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', 'quote', 'little floating tab next to 1', '\0', 'backslash', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', 'forwardslash', 
'\0', '*', '\0', ' ', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '7', '8', 
'9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', '\0', '\0', '\0', '\0', '\0'};   


void keyboard_interrupt(){
  //cli();
    int scan_code = inb(0x60);
    if (scan_code < 89){
        char output = keyScanCodeArray_one[scan_code]; 
        printf(" %c", output);
    }
    
    //printf(" %d", scan_code);
    send_eoi(1);
    //sti();
    
}

void rtc_interrupt(){
    //cli();
    //printf("RTC called");
    //test_interrupts();
    outb(0x0C, 0x70);	// select register C
    inb(0x71);		// just throw away contents
    send_eoi(8);
    //sti();
    //printf("eoi sent");
  
}


void system_call(){
printf("system called");
while(1);
}





