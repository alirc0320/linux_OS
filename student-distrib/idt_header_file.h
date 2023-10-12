#ifndef EXCEPTION_HANDLERS_H
#define EXCEPTION_HANDLERS_H

#include <signal.h>


void division_error_handler();
void debug_handler();
void non_maskable_interrupt_handler();
void breakpoint_handler();
void overflow_handler();
void bound_range_exceeded_handler();
void invalid_opcode_handler();
void device_not_available_handler();
void double_fault_handler();
void invalid_tss_handler();
void segment_not_present_handler();
void stack_segment_fault_handler();
void general_protection_fault_handler();
void page_fault_handler();
void x87_floating_point_exception_handler();
void alignment_check_handler();
void machine_check_handler();
void SIMD_floating_point_exception_handler();
void virtualization_exception();
void control_protection_exception_handler();
void hypervisor_exception_handler();
void vmm_communication_exception_handler();
void security_exception_handler();

#endif // EXCEPTION_HANDLERS_H
