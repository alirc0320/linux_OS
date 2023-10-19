// #include <stdio.h>
// #include <stdlib.h>
// #include "x86_desc.h"
#include "i8259.h"
#include "lib.h"
#include "idt_handlers.h"
#include "cursor.h"
// #include "Device_init.c"


/* Signal handler function for Division Error Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Division Error Exception and display an error message. */
void division_error_handler() {
        printf("Division Error Exception Caught!\n");
        while(1);
}

/* Signal handler function for Debug Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Debug Exception and display an error message. */
void debug_handler() {
    printf("Debug Exception Caught!\n");
    while (1);
}

/* Signal handler function for Non-Maskable Interrupt Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Non-Maskable Interrupt Exception and display an error message. */
void non_maskable_interrupt_handler() {
    printf("Non-Maskable Interrupt Exception Caught!\n");
    while (1);
}

/* Signal handler function for Breakpoint Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Breakpoint Exception and display an error message. */
void breakpoint_handler() {
    printf("Breakpoint Exception Caught!\n");
    while (1);
}

/* Signal handler function for Overflow Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Overflow Exception and display an error message. */
void overflow_handler() {
    printf("Overflow Exception Caught!\n");
    while (1);
}

/* Signal handler function for Bound Range Exceeded Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Bound Range Exceeded Exception and display an error message. */
void bound_range_exceeded_handler() {
    printf("Bound Range Exceeded Exception Caught!\n");
    while (1);
}

/* Signal handler function for Invalid Opcode Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Invalid Opcode Exception and display an error message. */
void invalid_opcode_handler() {
    printf("Invalid Opcode Exception Caught!\n");
    while (1);
}

/* Signal handler function for Device Not Available Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Device Not Available Exception and display an error message. */
void device_not_available_handler() {
    printf("Device Not Available Exception Caught!\n");
    while (1);
}

/* Signal handler function for Double Fault Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Double Fault Exception and display an error message. */
void double_fault_handler() {
    printf("Double Fault Exception Caught!\n");
    while (1);
}

/* Signal handler function for Invalid TSS (Task State Segment) Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Invalid TSS Exception and display an error message. */
void invalid_tss_handler() {
    printf("Invalid TSS Exception Caught!\n");
    while (1);
}

/* Signal handler function for Segment Not Present Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Segment Not Present Exception and display an error message. */
void segment_not_present_handler() {
    printf("Segment Not Present Exception Caught!\n");
    while (1);
}

/* Signal handler function for Stack-Segment Fault Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Stack-Segment Fault Exception and display an error message. */
void stack_segment_fault_handler() {
    printf("Stack-Segment Fault Exception Caught!\n");
    while (1);
}

/* Signal handler function for General Protection Fault Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the General Protection Fault Exception and display an error message. */
void general_protection_fault_handler() {
    printf("General Protection Fault Exception Caught!\n");
    while (1);
}

/* Signal handler function for Page Fault Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Page Fault Exception and display an error message. */
void page_fault_handler() {
    printf("Page Fault Exception Caught!\n");
    while (1);
}

/* Signal handler function for x87 Floating-Point Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the x87 Floating-Point Exception and display an error message. */
void x87_floating_point_exception_handler() {
    printf("x87 Floating-Point Exception Caught!\n");
    while (1);
}

/* Signal handler function for Alignment Check Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Alignment Check Exception and display an error message. */
void alignment_check_handler() {
    printf("Alignment Check Exception Caught!\n");
    while (1);
}

/* Signal handler function for Machine Check Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Machine Check Exception and display an error message. */
void machine_check_handler() {
    printf("Machine Check Exception Caught!\n");
    while (1);
}

/* Signal handler function for SIMD Floating-Point Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the SIMD Floating-Point Exception and display an error message. */
void SIMD_floating_point_exception_handler() {
    printf("SIMD Floating-Point Exception Caught!\n");
    while (1);
}

/* Signal handler function for Virtualization Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Virtualization Exception and display an error message. */
void virtualization_exception() {
    printf("Virtualization Exception Caught!\n");
    while (1);
}

/* Signal handler function for Control Protection Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Control Protection Exception and display an error message. */
void control_protection_exception_handler() {
    printf("Control Protection Exception Caught!\n");
    while (1);
}

/* Signal handler function for Hypervisor Injection Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Hypervisor Injection Exception and display an error message. */
void hypervisor_exception_handler() {
    printf("Hypervisor Injection Exception Caught!\n");
    while (1);
}

/* Signal handler function for VMM Communication Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the VMM Communication Exception and display an error message. */
void vmm_communication_exception_handler() {
    printf("VMM Communication Exception Caught!\n");
    while (1);
}

/* Signal handler function for Security Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Security Exception and display an error message. */
void security_exception_handler() {
    printf("Security Exception Caught!\n");
    while (1);
}

/* Array that maps keyboard scan codes to characters and control codes.
 * Each element in the array represents the character or control code associated with a specific scan code.
 * Note: Some entries are marked as '\0', which means they have no corresponding character or control code.
 * The array provides a lookup table for translating scan codes into readable characters. */
//0x59 is limit on array whihch is 89 in decimal 


unsigned char keyScanCodeArray_one[0x59] = {'\0','\0','1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', 
'=', '\0', '\0', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', '\0', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', '\0', '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 
'\0', '*', '\0', ' ', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '7', '8', 
'9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', '\0', '\0', '\0', '\0', '\0'};   

unsigned char keyScanCodeArray_for_shift[0x59] = {'\0','\0','!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', 
'+', '\0', '\0', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', '\0', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', '\0', '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 
'\0', '*', '\0', ' ', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '7', '8', 
'9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', '\0', '\0', '\0', '\0', '\0'};   

unsigned char keyScanCodeArray_for_just_capslock[0x59] = {'\0','\0','1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', 
'=', '\0', '\0', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n', '\0', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`','\0', '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 
'\0', '*', '\0', ' ', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '7', '8', 
'9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', '\0', '\0', '\0', '\0', '\0'};   



unsigned char keyScanCodeArray_for_shift_and_capslock[0x59] = {'\0','\0','!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', 
'+', '\0', '\0', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '{', '}', '\n', '\0', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':', '"', '~', '\0', '|', 'z', 'x', 'c', 'v', 'b', 'n', 'm', '<', '>', '?', 
'\0', '*', '\0', ' ', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '7', '8', 
'9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', '\0', '\0', '\0', '\0', '\0'};  



/* Keyboard Interrupt Handler Function
 * Inputs: None
 * Outputs: None
 * Function: Handles keyboard interrupt, reads the scan code from the keyboard controller, 
 *           maps it to a character using keyScanCodeArray_one, and prints the corresponding character. */
int keyboard_interrupt() {
    scan_code = inb(0x60);  // Read the scan code from the keyboard controller. x60 is the keyboard data port 
    kb_flag = 1; 
    write(keyboard_buffer, 0); 
    read(keyboard_buffer, 0);
    send_eoi(1);

    return scan_code;
}

 void write (const char* buf, int nbytes){//need to add fd as a function once file system worknig
    if (scan_code == 0x3A){//Caps lock
        capslock_flag += 1; 
        capslock_flag %= 2; 
        return; 
    }
    if(scan_code == 0x1D){
        ctrl_flag = 1; 
        return;
    }
    if (scan_code == 0x9D){
        ctrl_flag = 0; 
        return; 
    }
    if (ctrl_flag==1 && scan_code == 0x26){//this is for ctrl+L, we'll need to make arrays for ctrl and shifted characters
        clear(); 
        init_buffer(); 
        return; 
    }
    
    if (scan_code == 0x0E) { // Backspace
    int i;
    int backspace_count = 0;

    for (i = 0; i < 4; i++) {
        if (keyboard_buffer_index > 0 && keyboard_buffer[keyboard_buffer_index - 1] == ' ') {
            backspace();
            keyboard_buffer_index--;
            backspace_count++;
        } else {
            break;
        }
    }

    // If no tab spaces were removed, perform a regular backspace
    if (backspace_count == 0) {
        backspace();
        if (keyboard_buffer_index > 0) {
            keyboard_buffer[keyboard_buffer_index - 1] = '\0';
            keyboard_buffer_index--;
        }
    }

    return;
}
//ali
if (scan_code == 0x0F) { // Tab
    int i;
    for (i = 0; i < 4; i++) {
        // Update the buffer with four spaces
        if (keyboard_buffer_index < 127) {
            keyboard_buffer[keyboard_buffer_index] = ' ';
            putc(' '); // Print a space character
            keyboard_buffer_index++;
        }
    }
    return;
}





    ///////SHIFT////////////
    if(scan_code == 0xAA || scan_code == 0xB6){//shift released
       shift_flag = 0; 
    }
    if(scan_code == 0x2A || scan_code == 0x36){//shift pressed
        shift_flag = 1;
    }

    //////Alt
    if(scan_code == 0xB8){//released
       alt_flag = 0; 
    }
    if(scan_code == 0x38){//pressed
        alt_flag = 1;
    }
    //used to test alt flag works when alt + l pressed bc idrk what alt does yet
    // if (alt_flag == 1 && scan_code == 0x26){
    //     printf("alt flag works");
    // }


    else if (scan_code < 89 && scan_code != 0x1C) { //array index is less than 89
    if(keyboard_buffer_index >= 127 || keyScanCodeArray_one[scan_code] == '\0'){
        return; 
    }
        if(shift_flag == 1 && capslock_flag == 0){
            char output = keyScanCodeArray_for_shift[scan_code];  // Map the scan code to a character.
            keyboard_buffer[keyboard_buffer_index] = output; 
            keyboard_buffer_index ++;
            putc(output);

        }
        else if (shift_flag == 1 && capslock_flag == 1){
            char output = keyScanCodeArray_for_shift_and_capslock[scan_code];  // Map the scan code to a character.
            keyboard_buffer[keyboard_buffer_index] = output; 
            keyboard_buffer_index ++;
            putc(output);
        }
         else if (shift_flag == 0 && capslock_flag == 1){
            char output = keyScanCodeArray_for_just_capslock[scan_code];  // Map the scan code to a character.
            keyboard_buffer[keyboard_buffer_index] = output; 
            keyboard_buffer_index ++;
            putc(output);
        }
        else{
        char output = keyScanCodeArray_one[scan_code];  // Map the scan code to a character.
        keyboard_buffer[keyboard_buffer_index] = output; 
        keyboard_buffer_index ++; 
        putc(output);  // SHOULD BE PUTC
        }
}
   
}

 int read(const char* buf, int nbytes){//need to add fd as a function once file system worknig

     if (scan_code != 0x1C){
        return; 
     }
     //enter case
    else if(scan_code == 0x1C){
       
        putc('\n');
        int i; 
        for (i = 0; i<keyboard_buffer_index; i++){
            putc(keyboard_buffer[i]);
        }
        putc('\n');
        init_buffer(); 
        return; 
     }

}


void init_buffer(){
    int i; 
    for (i = 0; i< 128; i++){
        keyboard_buffer[i] = '\0';
    }
    keyboard_buffer_index = 0; 
}


/* Real-Time Clock (RTC) Interrupt Handler Function
 * Inputs: None
 * Outputs: None
 * Function: Handles RTC interrupt. If the 'flag' is set, it calls 'test_interrupts' function.
 *           Reads register C of the RTC to clear the interrupt and acknowledges the interrupt. */
void rtc_interrupt() {
   if (flag) {
        test_interrupts();  // Call 'test_interrupts' function if the 'flag' is set.
    }
    outb(0x0C, 0x70);  // Select register C of the RTC. x70 is the index
    inb(0x71);         // Read register C to clear the interrupt. this is the read or write port is 0x71
    
    send_eoi(8);        // Send an End-of-Interrupt signal to acknowledge the interrupt. interrupt numbr 8 is the rtc
}


 int rtc_write(int32_t freq){
        if ((freq & (freq - 1)) != 0 || freq > 1024){
            return -1; 
        }

    int rate = 0; 
    int temp = 32768/freq; 
    //printf("temp: %d", temp);
    //log base 2
     while (temp >= 1) {
        temp /= 2;
        rate += 1;
        // printf("temp: %d", temp);
        // printf("rate: %d", rate);
    }
    //rate++; 
    //printf("rate: %d", rate);
    //Select Register A and disable the NMI
    outb(0x8A, 0x70);
    // Get the initial value of Register A
    char prev = inb(0x71);
    // Reset the index to Register A
    outb(0x8A, 0x70);
    // Write the new rate to Register A
    outb((prev & 0xF0) | rate, 0x71);

    return 4; 

 }


    


/* System Call Handler Function
 * Inputs: None
 * Outputs: None
 * Function: Handles a system call interrupt. It prints a message and enters an infinite loop. */
void system_call() {
    printf("System call invoked.\n");  // Print a message indicating that a system call was invoked.
    while (1);  // Enter an infinite loop.
}


