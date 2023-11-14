#include "keyboard.h"
#include "i8259.h"
#include "lib.h"
#include "idt_handlers.h"
#include "cursor.h"
#include "file_system.h"
#include "System_Calls.h"
#include "terminal.h"

void terminal_init(){
    int i; 
    for(i = 0; i<3; i++){
        terminal_array[i].curr_x = 0;
        terminal_array[i].curr_y = 0;
        terminal_array[i].buffer_character_count = 0;
        terminal_array[i].terminal_id = i;
        // enable_cursor(terminal_array[i].curr_x);   
        // update_cursor(terminal_array[i].curr_x,terminal_array[i].curr_y);
    }
    current_terminal_id = 0;
    return; 
}



void terminal_switch (uint32_t terminal_id){ //pass in the terminal 


//check if current terminal is in a valid range, 0-2, or check if currterminal global id is equal to terminal_id

//back up current terminal
//paging?

//set new terminal
//paging?

//ret




}