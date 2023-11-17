
#include "keyboard.h"
#include "i8259.h"
#include "lib.h"
#include "idt_handlers.h"
#include "cursor.h"
#include "file_system.h"
#include "System_Calls.h"
#include "terminal.h"
#include "paging_asm.h"
#include "cursor.h"



void terminal_init(){
    //terminal_switch(0);
    int i; 
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


    if(terminal_id > 2 || terminal_id < 0){
        return;
    }

 strncpy(terminal_array[current_terminal_id].keyboard_buffer, keyboard_buffer, 128);
 terminal_array[current_terminal_id].buffer_index = keyboard_buffer_index;


    terminal_array[current_terminal_id].curr_x = screen_x ; 
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
    // execute("shell");   
    if (terminal_array[terminal_id].turned_on == 0){
    terminal_array[terminal_id].turned_on = 1; 
    int i; 
    int space; 
    space = -1; 
    for (i = 0; i < 8; i++){
        if (pcb_array[i] == NULL){
            space = i; 
        terminal_array[terminal_id].pid = space; 
        break; 
        }
        
    }
    if (space == -1){
        printf("no room"); 
        return -1; 
    }
    execute("shell");
    
}

//calling execute eveyrtime we switch to a new shell IS THIS RIGHT IAN???

    

    

}
