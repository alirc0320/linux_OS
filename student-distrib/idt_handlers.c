#include "i8259.h"
#include "lib.h"
#include "idt_handlers.h"
#include "cursor.h"
#include "file_system.h"


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

/* System Call Handler Function
 * Inputs: None
 * Outputs: None
 * Function: Handles a system call interrupt. It prints a message and enters an infinite loop. */
// void system_call() {
//     printf("System call invoked.\n");  // Print a message indicating that a system call was invoked.
//     while (1);  // Enter an infinite loop.
// }


void halt() {
    printf("halt");
    //while (1);
}


int32_t execute(const uint8_t* command) {
    printf("execute");
    //check if string is an executable
    
    uint8_t name[999]; //this initialization might be kinda sus 
    dentry_t temp_dentry; 
    int i = 0; 
    int fd; 
    int ret_val; 
    uint8_t boofer[999];
    
    ////////////////////CHECK IF IS AN EXECUTABLE/////////////////////////////////////////////////////////////
    
    //create filename array for just first word in space separated string
    while (command[i] != ' ' && command[i] != '\0'){
        name[i] = command[i];
         i ++; 
    }
    read_dentry_by_name(name, &temp_dentry);//get inode num to find file descriptor

    //IDRK if this is necesary
    for (i = 0; i< 8; i++){
        //NULL check
        if (file_descriptor_array[i] == NULL){
            fd = -1; 
            return -1; 
        }
            
        if (temp_dentry.inode_num == file_descriptor_array[i]->inode){
            fd = i; 
            break; 
        }
    }


//we may need some invalid file checks here idrk 

    ret_val = read_data(temp_dentry.inode_num, 0, boofer, 4); //read first four bytes, check if its the magic numbers signifying an executable
    if (ret_val < 4) 
        return -1; 
        //0: 0x7f; 1: 0x45; 2: 0x4c; 3: 0x46.
    if (boofer[0] != 0x7f || boofer[1] != 0x45 || boofer[2] != 0x4c || boofer[3] != 0x46)
        return -1; 
    file_descriptor_array[fd]->file_position = 4; 

    //////////////////////////////////CHECK IF IS AN EXECUTABLE//////////////////////////////////////////////////////////////

    //while (1);
    return 255; 
}

void read_syscall(void){
     printf("read syscall");
}
void write_syscall(void){
     printf("write syscall");

}
// void open_syscall(void){
//      printf("open syscall");
// }

uint32_t open_syscall(uint8_t* filename){
    //step one find fd
    int i = 0; 
    while (file_descriptor_array[i] !=NULL){
        i ++; 
        if (i == 8)
            return -1; //open failed, no space in arrau
    }
    printf("i: %d\n", i);

    //step two figure out which open youre calling
    dentry_t temp_dentry; 
    read_dentry_by_name(filename, &temp_dentry); 
    int ret_val; 
    if (temp_dentry.filetype == 0){
        printf("file type 0");
        ret_val = rtc_open(); //this should take a filename but I think we set up the arg st it didn't we prib need to change this idrk tho 
    }

     if (temp_dentry.filetype == 1){
        printf("file type 1");
        ret_val = open_dir(filename); 
    }
    if (temp_dentry.filetype == 2){
        printf("open file called");
        ret_val = open_file(filename); 
    }
    if (ret_val == -1)
        return -1; 

    //step 3 add the file to the fd array
    printf("entering add function");
    add_file_descriptor_table_entry(i, filename);

    //printf("ret val: %d\n", ret_val);
    return ret_val; 


}