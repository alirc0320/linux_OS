#define ASM  1
//#include "x86_desc.h"



.globl division_error_handler_asm
.globl debug_handler_asm
.globl non_maskable_interrupt_handler_asm
.globl breakpoint_handler_asm
.globl overflow_handler_asm
.globl bound_range_exceeded_handler_asm
.globl invalid_opcode_handler_asm
.globl device_not_available_handler_asm
.globl double_fault_handler_asm
.globl invalid_tss_handler_asm
.globl segment_not_present_handler_asm
.globl stack_segment_fault_handler_asm
.globl general_protection_fault_handler_asm
.globl page_fault_handler_asm
.globl x87_floating_point_exception_handler_asm
.globl alignment_check_handler_asm
.globl machine_check_handler_asm
.globl SIMD_floating_point_exception_handler_asm
.globl virtualization_exception_asm
.globl control_protection_exception_handler_asm
.globl hypervisor_exception_handler_asm
.globl vmm_communication_exception_handler_asm
.globl security_exception_handler_asm
.globl rtc_interrupt_asm
.globl keyboard_interrupt_asm
.globl system_call_asm


    division_error_handler_asm:                            
    pushal                               
    pushfl                              
    call division_error_handler                    
    popfl                                
    popal                               
    iret

    debug_handler_asm:                            
    pushal                               
    pushfl                              
    call debug_handler                    
    popfl                                
    popal                               
    iret

    non_maskable_interrupt_handler_asm:                            
    pushal                               
    pushfl                              
    call non_maskable_interrupt_handler                    
    popfl                                
    popal                               
    iret
    
    breakpoint_handler_asm:                            
    pushal                               
    pushfl                              
    call breakpoint_handler                    
    popfl                                
    popal                               
    iret
    
    overflow_handler_asm:                            
    pushal                               
    pushfl                              
    call overflow_handler                    
    popfl                                
    popal                               
    iret
    
    bound_range_exceeded_handler_asm:                            
    pushal                               
    pushfl                              
    call bound_range_exceeded_handler                    
    popfl                                
    popal                               
    iret
    
    invalid_opcode_handler_asm:                            
    pushal                               
    pushfl                              
    call invalid_opcode_handler                    
    popfl                                
    popal                               
    iret
    
    device_not_available_handler_asm:                            
    pushal                               
    pushfl                              
    call device_not_available_handler                    
    popfl                                
    popal                               
    iret
    
    double_fault_handler_asm:                            
    pushal                               
    pushfl                              
    call double_fault_handler                    
    popfl                                
    popal                               
    iret
    
    invalid_tss_handler_asm:                            
    pushal                               
    pushfl                              
    call invalid_tss_handler                    
    popfl                                
    popal                               
    iret



    segment_not_present_handler_asm:
    pushal                               
    pushfl                              
    call segment_not_present_handler                    
    popfl                                
    popal                               
    iret


    
    stack_segment_fault_handler_asm:
    pushal                               
    pushfl                              
    call stack_segment_fault_handler                    
    popfl                                
    popal                               
    iret



    general_protection_fault_handler_asm:
    pushal                               
    pushfl                              
    call general_protection_fault_handler                    
    popfl                                
    popal                               
    iret




    
    page_fault_handler_asm:
    pushal                               
    pushfl                              
    call page_fault_handler                    
    popfl                                
    popal                               
    iret




    
    x87_floating_point_exception_handler_asm:
    pushal                               
    pushfl                              
    call x87_floating_point_exception_handler                    
    popfl                                
    popal                               
    iret


    
    alignment_check_handler_asm:
    pushal                               
    pushfl                              
    call alignment_check_handler                    
    popfl                                
    popal                               
    iret
    
    machine_check_handler_asm:
    pushal                               
    pushfl                              
    call machine_check_handler                    
    popfl                                
    popal                               
    iret
    
    SIMD_floating_point_exception_handler_asm:
    pushal                               
    pushfl                              
    call SIMD_floating_point_exception_handler                    
    popfl                                
    popal                               
    iret
    
    virtualization_exception_asm:
    pushal                               
    pushfl                              
    call virtualization_exception                    
    popfl                                
    popal                               
    iret
    
    control_protection_exception_handler_asm:
    pushal                               
    pushfl                              
    call control_protection_exception_handler                    
    popfl                                
    popal                               
    iret
    
    hypervisor_exception_handler_asm:
    pushal                               
    pushfl                              
    call hypervisor_exception_handler                    
    popfl                                
    popal                               
    iret
    
    vmm_communication_exception_handler_asm:
    pushal                               
    pushfl                              
    call vmm_communication_exception_handler                    
    popfl                                
    popal                               
    iret
    
    security_exception_handler_asm:
    pushal                               
    pushfl                              
    call security_exception_handler                    
    popfl                                
    popal                               
    iret

    // rtc_interrupt_asm:
    // pushal                               
    // pushfl                              
    // call rtc_interrupt                    
    // popfl                                
    // popal                               
    // iret
    
    keyboard_interrupt_asm:
    pushal                               
    pushfl                              
    call keyboard_interrupt                    
    popfl                                
    popal                               
    iret
    
    // system_call_asm:
    // pushal                               
    // pushfl                              
    // call system_call                     
    // popfl                                
    // popal                               
    // iret






    