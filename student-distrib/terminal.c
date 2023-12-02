
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
         terminal_array[i].enter_flag = 0;
        //update_cursor(terminal_array[i].curr_x, terminal_array[i].curr_y);
        terminal_array[i].pid = 0;
        terminal_array[i].turned_on = 0;
      //  terminal_array[i].pcb_array[0]->global_pcb_value = i; 
    }
    current_terminal_id = 0;
    terminal_switch(0);
    target_pid = 0; 
    return; 
}



void process_switch (){
cli(); 

if (terminal_array[counter].turned_on == 0){
    terminal_array[counter].turned_on = 1;  
     execute((const uint8_t*)"shell");
     //terminal_switch(counter)
  }



register uint32_t ebp_value asm("ebp"); //cuirrentm vals
register uint32_t esp_value asm("esp");

int old = counter  -1; 
  if (old < 0){
    old = 2; 
  }
  if(terminal_array[old].turned_on == 0)
  return;
  save_old(old, counter, ebp_value, esp_value); 


  // program_pages(pcb_array[global_pcb_counter]->global_pcb_value);


  //   asm volatile (" \
  //   movl %%cr3, %%eax ; \
  //   movl %%eax, %%cr3 ; \
  //   " : /* no outputs */ \
  //   : /* no inputs */ \
  //   : "eax", "cc");

  
  restore_new(counter); 
  counter = (counter + 1 )%3;
  sti();


//     register uint32_t ebp_value asm("ebp"); //cuirrentm vals
//     register uint32_t esp_value asm("esp");
  
  // int old = process_id  -1; 
  // if (old < 0){
  //   old = 2; 
  // }
  //   save_old(old, process_id, ebp_value, esp_value); 


//     program_pages(pcb_array[target_pid]->global_pcb_value);


//     asm volatile (" \
//     movl %%cr3, %%eax ; \
//     movl %%eax, %%cr3 ; \
//     " : /* no outputs */ \
//     : /* no inputs */ \
//     : "eax", "cc");

//  //counter = (counter + 1)%3;
// restore_new(process_id); 

 //sti(); 
}


void save_old(int cur, int new, register uint32_t ebp_value,register uint32_t esp_value){

//cli();

//get rid of pointers
    terminal_array[cur].esp =  esp_value; 
    terminal_array[cur].ebp =  ebp_value; 
    // terminal_array[cur].ss =  tss.ss0; 
    // terminal_array[cur].esp0 = tss.esp0;

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
//sti();
    return; 

}


void restore_new(int new){
cli();
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
sti();
            return;
}


void terminal_switch(uint32_t terminal_id){
  
  //////////////////////////////////////////////////////////////////

  // register uint32_t ebp_value asm("ebp"); //cuirrentm vals
  // register uint32_t esp_value asm("esp");
  // save_old(current_terminal_id, terminal_id, ebp_value, esp_value);
   
  //////////////////////////////////////////////////////////////////
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

    // if (terminal_array[current_terminal_id].turned_on == 0){
    // terminal_array[current_terminal_id].turned_on = 1;  

    // execute("shell"); 
    // }
  
  //////////////////////////////////////////////////////////////////

  // program_pages(pcb_array[target_pid]->global_pcb_value);


  //   asm volatile (" \
  //   movl %%cr3, %%eax ; \
  //   movl %%eax, %%cr3 ; \
  //   " : /* no outputs */ \
  //   : /* no inputs */ \
  //   : "eax", "cc");

  // //counter = (counter + 1)%3;
  // restore_new(terminal_id); 

  //////////////////////////////////////////////////////////////////
}