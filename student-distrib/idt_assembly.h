#ifndef _IDT_ASSEMBLY_H
#define _IDT_ASSEMBLY_H
#include "idt_handlers.h"

void division_error_handler_asm();
void debug_handler_asm();
void non_maskable_interrupt_handler_asm();
void breakpoint_handler_asm();
void overflow_handler_asm();
void bound_range_exceeded_handler_asm();
void invalid_opcode_handler_asm();
void device_not_available_handler_asm();
void double_fault_handler_asm();
void invalid_tss_handler_asm();
void segment_not_present_handler_asm();
void stack_segment_fault_handler_asm();
void general_protection_fault_handler_asm();
void page_fault_handler_asm();
void x87_floating_point_exception_handler_asm();
void alignment_check_handler_asm();
void machine_check_handler_asm();
void SIMD_floating_point_exception_handler_asm();
void virtualization_exception_asm();
void control_protection_exception_handler_asm();
void hypervisor_exception_handler_asm();
void vmm_communication_exception_handler_asm();
void security_exception_handler_asm();
void rtc_interrupt_asm();
void keyboard_interrupt_asm();
void system_call_asm();


#endif 




