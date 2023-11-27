
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



void terminal_init(){
    //terminal_switch(0);
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
        //update_cursor(terminal_array[i].curr_x, terminal_array[i].curr_y);
        terminal_array[i].pid = -1;
        terminal_array[i].turned_on = 0;

    }
    //execute("shell");
    current_terminal_id = 0;
    terminal_switch(0);

    return; 
}



void terminal_switch (uint32_t terminal_id){ //pass in the terminal --this value is ZERO INDEXED

uint32_t* ESP;
uint32_t* EBP;


   asm volatile (
        "pushal \n"
        "pushfl" // Push all general-purpose registers and flags
    );

    register uint32_t ebp_value asm("ebp"); //cuirrentm vals
    register uint32_t esp_value asm("esp");
  
  

    terminal_array[current_terminal_id].esp = (uint8_t*) esp_value; 
    terminal_array[current_terminal_id].ebp = (uint8_t*) ebp_value; 
    //terminal_array[current_terminal_id].ss =  tss.ss0; 
    // terminal_array[current_terminal_id].esp0 = tss.esp0;
 




    if(terminal_id > 2 || terminal_id < 0){
        return;
    }
    int i = 0; 
    for (i = 0; i< 6; i++){
        terminal_array[current_terminal_id].pcb_array[i] = pcb_array[i]; //store cb array of what you are about to change from
        pcb_array[i] = terminal_array[terminal_id].pcb_array[i]; //store pcb array with what y9ou are going to
    }

    terminal_array[current_terminal_id].pid = target_pid; 
    target_pid = terminal_array[terminal_id].pid; 

 strncpy(terminal_array[current_terminal_id].keyboard_buffer, keyboard_buffer, 128);
 terminal_array[current_terminal_id].buffer_index = keyboard_buffer_index;


    terminal_array[current_terminal_id].curr_x = screen_x; 
    terminal_array[current_terminal_id].curr_y = screen_y;   

    //which t4erminal to switch to
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

    if (terminal_array[terminal_id].turned_on == 0){
    terminal_array[terminal_id].turned_on = 1;  
   

    execute("shell");
    
}


    //tss.ss0 = terminal_array[current_terminal_id].ss; 
    //tss.esp0 = terminal_array[current_terminal_id].esp0; 
 
    ESP = (uint32_t*)(terminal_array[terminal_id].esp); 
    EBP = (uint32_t*)(terminal_array[terminal_id].ebp); 


 asm volatile(
            "cli                 \n"
            "movl %0, %%esp \n"
              "movl %1, %%ebp \n"
              "popfl \n"
              "popal \n"
              "sti                 \n"
              "ret"
            : /* no output */
            : "r" (ESP), "r" (EBP)
            : "memory");



}
