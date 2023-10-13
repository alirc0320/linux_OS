#include "idt_handlers.h"
#include "x86_desc.h"


void initialize_idt();

void initialize_idt() {

idt_init_array[0] = &division_error_handler;
idt_init_array[1] = &debug_handler;
idt_init_array[2] = &non_maskable_interrupt_handler;
idt_init_array[3] = &breakpoint_handler;
idt_init_array[4] = &overflow_handler;
idt_init_array[5] = &bound_range_exceeded_handler;
idt_init_array[6] = &invalid_opcode_handler;
idt_init_array[7] = &device_not_available_handler;
idt_init_array[8] = &double_fault_handler;
idt_init_array[9] = &invalid_tss_handler;
idt_init_array[10] = &segment_not_present_handler;
idt_init_array[11] = &stack_segment_fault_handler;
idt_init_array[12] = &general_protection_fault_handler;
idt_init_array[13] = &page_fault_handler;
idt_init_array[14] = &x87_floating_point_exception_handler;
idt_init_array[15] = &alignment_check_handler;
idt_init_array[16] = &machine_check_handler;
idt_init_array[17] = &SIMD_floating_point_exception_handler;
idt_init_array[18] = &virtualization_exception;
idt_init_array[19] = &control_protection_exception_handler;
idt_init_array[20] = &hypervisor_exception_handler;
idt_init_array[21] = &vmm_communication_exception_handler;
idt_init_array[22] = &security_exception_handler;
idt_init_array[23] = &rtc_interrupt;
idt_init_array[24] = &keyboard_interrupt;
idt_init_array[25] = &pic_interrupt;
idt_init_array[128] = &system_call;


//EXCEPTIONS//
for(int i = 0; i<23; i++){
    idt_desc_t idt_init;  
    idt_init.__attribute__.offset_15_00 = i*8;       //we think its thid bc diff offset evry 8 bytes
    idt_init.__attribute__.seg_selector = KERNEL_CS ;
    idt_init.__attribute__.reserved4 = 0; //I think this is always set to zero its the 8 bits on the end
    idt_init.__attribute__.reserved3 = 1;//reserved 3-1 choose if interrupt or trap, size tells if 32 or 16
    idt_init.__attribute__.reserved2 = 1;
    idt_init.__attribute__.reserved1 = 1;
    idt_init.__attribute__.size = 1;
    idt_init.__attribute__.reserved0 = 0;
    idt_init.__attribute__.dpl = 0;
    idt_init.__attribute__.present = 1;
    idt_init.__attribute__.offset_31_16 = 0;

    SET_IDT_ENTRY(idt_init.__attribute__, idt_init_array[i]);
    idt[i] = idt_init; 

}


//INTERRUPTS//
    for (int i = 23; i< 26; i++){
    idt_desc_t idt_interrupt_init;  
    idt_interrupt_init.__attribute__.offset_15_00 = i*8;
    idt_interrupt_init.__attribute__.seg_selector = KERNEL_CS ;
    idt_interrupt_init.__attribute__.reserved4 = 0;
    idt_interrupt_init.__attribute__.reserved3 = 0;
    idt_interrupt_init.__attribute__.reserved2 = 1;
    idt_interrupt_init.__attribute__.reserved1 = 1;
    idt_interrupt_init.__attribute__.size = 1;
    idt_interrupt_init.__attribute__.reserved0 = 0;
    idt_interrupt_init.__attribute__.dpl = 0;
    idt_interrupt_init.__attribute__.present = 1;
    idt_interrupt_init.__attribute__.offset_31_16 = 0;
   
    SET_IDT_ENTRY(idt_interrupt_init.__attribute__, idt_init_array[i]); 
    idt[i] = idt_interrupt_init; 

    }


//SYSTEM CALL//
    idt_desc_t idt_system_call_init; 
    idt_system_call_init.__attribute__.offset_15_00 = 128;
    idt_system_call_init.__attribute__.seg_selector = KERNEL_CS ;
    idt_system_call_init.__attribute__.reserved4 = 0;
    idt_system_call_init.__attribute__.reserved3 = 0;
    idt_system_call_init.__attribute__.reserved2 = 1;
    idt_system_call_init.__attribute__.reserved1 = 1;
    idt_system_call_init.__attribute__.size = 1;
    idt_system_call_init.__attribute__.reserved0 = 0;
    idt_system_call_init.__attribute__.dpl = 0;
    idt_system_call_init.__attribute__.present = 1; 
    idt_system_call_init.__attribute__.offset_31_16 = 0;
   
    SET_IDT_ENTRY(idt_system_call_init.__attribute__, idt_init_array[128]); 
    idt[128] = idt_system_call_init;
    

}
