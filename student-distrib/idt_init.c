#include "idt_init.h"



void initialize_idt() {
int i;

    for (i = 0; i < NUM_VEC; i++){
        idt[i].seg_selector = KERNEL_CS ;
        idt[i].reserved4 = 0;
        idt[i].reserved3 = 1;
        idt[i].reserved2 = 1;
        idt[i].reserved1 = 1;
        idt[i].size = 1;
        idt[i].reserved0 = 0;
        idt[i].dpl = 0;
        idt[i].present = 0; //should it always be 1??????????????????????????

        if (i >= 0x20 && i <= 0x2F){
            idt[i].reserved3 = 0; // pics
        }

        if (i == 0x80){
            idt[i].dpl = 3; // syscall
            idt[i].present = 1; 
        }

        if (i >= 0x0 && i <= 0x2F){
            idt[i].present = 1; 
        }

    }

    SET_IDT_ENTRY(idt[0], division_error_handler_asm);
    SET_IDT_ENTRY(idt[1], debug_handler_asm);
    SET_IDT_ENTRY(idt[2], non_maskable_interrupt_handler_asm);
    SET_IDT_ENTRY(idt[3], breakpoint_handler_asm);
    SET_IDT_ENTRY(idt[4], overflow_handler_asm);
    SET_IDT_ENTRY(idt[5], bound_range_exceeded_handler_asm);
    SET_IDT_ENTRY(idt[6], invalid_opcode_handler_asm);
    SET_IDT_ENTRY(idt[7], device_not_available_handler_asm);
    SET_IDT_ENTRY(idt[8], double_fault_handler_asm);
    SET_IDT_ENTRY(idt[9], invalid_tss_handler_asm);
    SET_IDT_ENTRY(idt[10], segment_not_present_handler_asm);
    SET_IDT_ENTRY(idt[11], stack_segment_fault_handler_asm);
    SET_IDT_ENTRY(idt[12], general_protection_fault_handler_asm);
    SET_IDT_ENTRY(idt[13], page_fault_handler_asm);
    SET_IDT_ENTRY(idt[14], x87_floating_point_exception_handler_asm);
    SET_IDT_ENTRY(idt[15], alignment_check_handler_asm);
    SET_IDT_ENTRY(idt[16], machine_check_handler_asm);
    SET_IDT_ENTRY(idt[17], SIMD_floating_point_exception_handler_asm);
    SET_IDT_ENTRY(idt[18], virtualization_exception_asm);
    SET_IDT_ENTRY(idt[19], control_protection_exception_handler_asm);
    SET_IDT_ENTRY(idt[20], hypervisor_exception_handler_asm);
    SET_IDT_ENTRY(idt[21], vmm_communication_exception_handler_asm);
    SET_IDT_ENTRY(idt[22], security_exception_handler_asm);
    // SET_IDT_ENTRY(idt[33], rtc_interrupt_asm);
    // SET_IDT_ENTRY(idt[40], keyboard_interrupt_asm);
    // SET_IDT_ENTRY(idt[128], system_call_asm);


    

}








