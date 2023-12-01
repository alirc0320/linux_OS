

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


#define Channel_data_port  0x40
#define PIT_MODE           0x43
#define PIT_IRQ            0
#define SET_ACCESS_MODE    0x36       // 00110110 


void init_pit(){

    unsigned int divisor = 1193180 / 100;   // 1193180 is the PIT frequency. Reload = PIT/Desired
    // Send the divisor (LSB followed by MSB)
    outb(SET_ACCESS_MODE, PIT_MODE);
    outb(divisor & 0xFF, 0x40);             // LSB
    outb((divisor >> 8) & 0xFF, 0x40);      // MSB
    counter = -1; 
    enable_irq(0);
    // terminal_init();
    
}



//pit handler 
void pit_interrupt(){
    //  cli(); 
    send_eoi(0); //moved up here above execute shell call bc of inf while loop in execute
    // sti(); 
    // int old = counter; 
    // counter = (counter + 1 )%3;
    // if (terminal_array[counter].turned_on == 0){
    // //terminal_array[counter].turned_on = 1;  
    // // cli(); 
    // //  //moved up here above execute shell call bc of inf while loop in execute
    // // sti(); 
    // //execute("shell"); 
    // register uint32_t ebp_value asm("ebp"); //cuirrentm vals
    // register uint32_t esp_value asm("esp");
    // save_old(old, counter, ebp_value, esp_value);
    // in_a_pit_interrupt = 1; 
    // terminal_switch(counter);
    // }
       
    // process_switch(counter);
    //counter = (counter + 1)%3;
    // cli(); 
    // send_eoi(0); //moved up here above execute shell call bc of inf while loop in execute
    // sti(); 
}

