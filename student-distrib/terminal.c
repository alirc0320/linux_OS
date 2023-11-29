
#include "keyboard.h"
#include "i8259.h"
#include "lib.h"
#include "idt_handlers.h"
#include "cursor.h"
#include "file_system.h"
#include "x86_desc.h"
#include "terminal.h"
#include "paging_asm.h"
#include "cursor.h"
#include "pit.h"



void terminal_init(){
    global_pcb_counter = 0; 
    int i; 
    for (i = 0; i< 6; i++){
        pcb_array[i] = 0; 
        terminal_array[0].pcb_array[i] = 0; 
        terminal_array[1].pcb_array[i] = 0; 
        terminal_array[2].pcb_array[i] = 0; 
    }
    for(i = 0; i<3; i++){
        terminal_array[i].curr_x = 0;
        terminal_array[i].curr_y = 0;
        terminal_array[i].buffer_character_count = 0;
        terminal_array[i].terminal_id = i;
         terminal_array[i].buffer_index = 0; 
        terminal_array[i].pid = -1;
        terminal_array[i].turned_on = 0;

    }
    current_terminal_id = 0;
    enable_irq(0);
    terminal_switch(0);

    return; 
}


void terminal_switch(uint32_t terminal_id){

    register uint32_t ebp_value asm("ebp"); //cuirrentm vals
    register uint32_t esp_value asm("esp");
  
    strncpy(terminal_array[current_terminal_id].keyboard_buffer, keyboard_buffer, 128);
    terminal_array[current_terminal_id].buffer_index = keyboard_buffer_index;

    terminal_array[current_terminal_id].curr_x = screen_x; 
    terminal_array[current_terminal_id].curr_y = screen_y;   

    // which t4erminal to switch to
    int old_addr = (185 + current_terminal_id) <<12; 
    int addr = (185 + terminal_id) << 12; 

    send_eoi(1);
    memcpy((uint8_t*)old_addr, (uint8_t*)0xB8000, 4096);
    memcpy((uint8_t*)0xB8000, (uint8_t*)addr, 4096);
    
    strncpy(keyboard_buffer, terminal_array[terminal_id].keyboard_buffer ,128);
    keyboard_buffer_index = terminal_array[terminal_id].buffer_index;
   
    screen_x = terminal_array[terminal_id].curr_x ; 
    screen_y = terminal_array[terminal_id].curr_y ;   
    update_cursor(screen_x, screen_y);    

    current_terminal_id = terminal_id;

    sti();


}

void process_switch (){

   // Variables to store process and terminal information
    int current_pid;
    int previous_pid;
    int previous_terminal_id;

    // Static variable to track the current scheduling index
    static int schedule_idx = -1;

    // Increment the scheduling index and reset if it exceeds the limit
    schedule_idx++;
    if(schedule_idx > 2) {
        schedule_idx = 0;
    }

    // Update terminal indices for current and previous terminals
    previous_terminal_id = current_terminal_id;
    current_terminal_id = schedule_idx;

    // Obtain Process Control Blocks (PCBs) for the current and previous processes
    current_pid = terminal_array[current_terminal_id].pid;
    previous_pid = terminal_array[previous_terminal_id].pid;
    pcb_t * current_pcb = (pcb_t*)(0x800000 - ((current_pid + 1) * 0x2000));
    pcb_t * previous_pcb = (pcb_t*)(0x800000 - ((previous_pid + 1) * 0x2000));

    // Obtain the current values of stack pointers (esp and ebp)
    register uint32_t ebp_value asm("ebp");
    register uint32_t esp_value asm("esp");

    // Launch shells for uninitialized terminals
    if (terminal_array[schedule_idx].turned_on == 0) {
        terminal_array[schedule_idx].turned_on = 1;  
        send_eoi(0);
        execute("shell");
    }

    // Save the current stack pointers for the previous process
    previous_pcb->ebp_for_scheduling = ebp_value;
    previous_pcb->esp_for_scheduling = esp_value;


    //remap paging
    pd[32].bits_31_12 = (0x800000 + (current_pid * 0x2000 )) >> 12; 
        // if(current_terminal_id == ){
        // vid_mem[0xB8].bits_31_12 = 0xB800 >> 12; 
        // }
        // else{
        // vid_mem[0xB8].bits_31_12 = (0xB800 + 4096*(current_terminal_id+1)) >> 12;
        // }
    
    //FLUSH THE TLB
    asm volatile (" \
   movl %%cr3, %%eax ; \
   movl %%eax, %%cr3 ; \
   " : /* no outputs */ \
   : /* no inputs */ \
   : "eax", "cc");

    // Set up the Task State Segment (TSS) for the context switch
    tss.esp0 = (0x800000 - ((current_pid) * 0x2000) - sizeof(int));

    // Obtain the updated values of stack pointers for the current process
    int updated_esp = current_pcb->esp_for_scheduling;
    int updated_ebp = current_pcb->ebp_for_scheduling;

    // Perform a context switch using inline assembly
    asm volatile(
        "movl %0, %%esp;"
        "movl %1, %%ebp;"
        :
        : "r"(updated_esp), "r"(updated_ebp)
        : "%esp", "%ebp"
    );
 
}


void save_old(int cur, int new, register uint32_t ebp_value,register uint32_t esp_value){

    terminal_array[cur].esp =  esp_value; 
    terminal_array[cur].ebp =  ebp_value; 
    terminal_array[cur].ss =  tss.ss0; 
    terminal_array[cur].esp0 = tss.esp0;

    if(new > 2 || new < 0){
        return;
    }
    
    int i = 0; 
    
    for (i = 0; i< 6; i++){
        terminal_array[cur].pcb_array[i] = pcb_array[i]; //store cb array of what you are about to change from
        pcb_array[i] = terminal_array[new].pcb_array[i]; //store pcb array with what y9ou are going to
    }

    terminal_array[cur].pid = target_pid; 
    target_pid = terminal_array[new].pid; 

    return; 
}


void restore_new(int new){

    uint32_t ESP;
    uint32_t EBP;
    
    tss.ss0 = terminal_array[new].ss; 
    tss.esp0 = terminal_array[new].esp0; 
 
    ESP = terminal_array[new].esp; 
    EBP = terminal_array[new].ebp; 

    asm volatile(
    "movl %0, %%esp \n"
    "movl %1, %%ebp \n"
    : /* no output */
    : "r" (ESP), "r" (EBP)
    : "memory");

    return;
}

