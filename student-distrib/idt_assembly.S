#define ASM  
include "idt_handlers.h"




#define linker(asm_function, c_function) 

    asm_function:                            
    pushal                               
    pushfl                              
    call c_function                        
    popfl                                
    popal                               
    iret

linker(division_error_handler_asm, division_error_handler)
linker(debug_handler_asm, debug_handler)
linker(non_maskable_interrupt_handler_asm, non_maskable_interrupt_handler)
linker(breakpoint_handler_asm, breakpoint_handler)
linker(overflow_handler_asm, overflow_handler)
linker(bound_range_exceeded_handler_asm, bound_range_exceeded_handler)
linker(invalid_opcode_handler_asm, invalid_opcode_handler)
linker(device_not_available_handler_asm, device_not_available_handler)
linker(double_fault_handler_asm, double_fault_handler)
linker(invalid_tss_handler_asm, invalid_tss_handler)
linker(segment_not_present_handler_asm, segment_not_present_handler)
linker(stack_segment_fault_handler_asm, stack_segment_fault_handler)
linker(general_protection_fault_handler_asm, general_protection_fault_handler)
linker(page_fault_handler_asm, page_fault_handler)
linker(x87_floating_point_exception_handler_asm, x87_floating_point_exception_handler)
linker(alignment_check_handler_asm, alignment_check_handler)
linker(machine_check_handler_asm, machine_check_handler)
linker(SIMD_floating_point_exception_handler_asm, SIMD_floating_point_exception_handler)
linker(virtualization_exception_asm, virtualization_exception)
linker(control_protection_exception_handler_asm, control_protection_exception_handler)
linker(hypervisor_exception_handler_asm, hypervisor_exception_handler)
linker(vmm_communication_exception_handler_asm, vmm_communication_exception_handler)
linker(security_exception_handler_asm, security_exception_handler)
linker(rtc_interrupt_asm, rtc_interrupt)
linker(keyboard_interrupt_asm, keyboard_interrupt)
linker(system_call_asm, system_call)
