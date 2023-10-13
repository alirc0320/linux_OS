
#ifndef _IDT_HANDLERS_H
#define _IDT_HANDLERS_H


extern void division_error_handler();
extern void debug_handler();
extern void non_maskable_interrupt_handler();
extern void breakpoint_handler();
extern void overflow_handler();
extern void bound_range_exceeded_handler();
extern void invalid_opcode_handler();
extern void device_not_available_handler();
extern void double_fault_handler();
extern void invalid_tss_handler();
extern void segment_not_present_handler();
extern void stack_segment_fault_handler();
extern void general_protection_fault_handler();
extern void page_fault_handler();
extern void x87_floating_point_exception_handler();
extern void alignment_check_handler();
extern void machine_check_handler();
extern void SIMD_floating_point_exception_handler();
extern void virtualization_exception();
extern void control_protection_exception_handler();
extern void hypervisor_exception_handler();
extern void vmm_communication_exception_handler();
extern void security_exception_handler();
extern void rtc_interrupt();
extern void keyboard_interrupt();
extern void system_call(); 

#endif /* _IDT_HANDLERS_H */