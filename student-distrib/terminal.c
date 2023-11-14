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
        //current_terminal_id = i; 
        
        terminal_switch(i);
         
        execute("shell");//boots up all 3
        //printf("cur terminal:%d\n",  i);

        
         

        
    }
    //current_terminal_id = 0;
    terminal_switch(0);

    return; 
}



void terminal_switch (uint32_t terminal_id){ //pass in the terminal --this value is ZERO INDEXED

//which t4erminal to switch to
int old_addr = (185 + current_terminal_id) <<12; 

//save old cursor
terminal_array[current_terminal_id].curr_x = screen_x; 
terminal_array[current_terminal_id].curr_y = screen_y; 


current_terminal_id = terminal_id; //determine terminal id if its +1%3 or -1%3 in keyboard handler


int addr = (185 + terminal_id) << 12; 
memcpy((uint8_t*)old_addr, (uint8_t*)0xB8000, 4096);
memcpy((uint8_t*)0xB8000, (uint8_t*)addr, 4096);
//enable_cursor(terminal_array[terminal_id].curr_x,terminal_array[terminal_id].curr_y);   

//save old cursor

//update_cursor(terminal_array[terminal_id].curr_x,terminal_array[terminal_id].curr_y);



//check if current terminal is in a valid range, 0-2, or check if currterminal global id is equal to terminal_id

//back up current terminal
//paging?

//set new terminal
//paging?

//ret




}