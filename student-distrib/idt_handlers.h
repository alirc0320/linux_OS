
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



.global division_error_handler_asm
.global debug_handler_asm
.global non_maskable_interrupt_handler_asm
.global breakpoint_handler_asm
.global overflow_handler_asm
.global bound_range_exceeded_handler_asm
.global invalid_opcode_handler_asm
.global device_not_available_handler_asm
.global double_fault_handler_asm
.global invalid_tss_handler_asm
.global segment_not_present_handler_asm
.global stack_segment_fault_handler_asm
.global general_protection_fault_handler_asm
.global page_fault_handler_asm
.global x87_floating_point_exception_handler_asm
.global alignment_check_handler_asm
.global machine_check_handler_asm
.global SIMD_floating_point_exception_handler_asm
.global virtualization_exception_asm
.global control_protection_exception_handler_asm
.global hypervisor_exception_handler_asm
.global vmm_communication_exception_handler_asm
.global security_exception_handler_asm
.global rtc_interrupt_asm
.global keyboard_interrupt_asm
.global system_call_asm

#endif /* _IDT_HANDLERS_H */