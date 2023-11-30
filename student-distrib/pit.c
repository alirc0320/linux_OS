

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
    enable_irq(0);
    // terminal_init();
}



/*
Bits         Usage
6 and 7      Select channel :
                0 0 = Channel 0
                0 1 = Channel 1
                1 0 = Channel 2
                1 1 = Read-back command (8254 only)
4 and 5      Access mode :
                0 0 = Latch count value command
                0 1 = Access mode: lobyte only
                1 0 = Access mode: hibyte only
                1 1 = Access mode: lobyte/hibyte
1 to 3       Operating mode :
                0 0 0 = Mode 0 (interrupt on terminal count)
                0 0 1 = Mode 1 (hardware re-triggerable one-shot)
                0 1 0 = Mode 2 (rate generator)
                0 1 1 = Mode 3 (square wave generator)
                1 0 0 = Mode 4 (software triggered strobe)
                1 0 1 = Mode 5 (hardware triggered strobe)
                1 1 0 = Mode 2 (rate generator, same as 010b)
                1 1 1 = Mode 3 (square wave generator, same as 011b)
0            BCD/Binary mode: 0 = 16-bit binary, 1 = four-digit BCD
*/


//pit handler 
void pit_interrupt(){

    
        send_eoi(0); //moved up here above execute shell call bc of inf while loop in execute

 if (terminal_array[counter].turned_on == 0){
    terminal_array[counter].turned_on = 1;  
   

    execute("shell");
    
}
       
        process_switch(counter);
counter = (counter + 1)%3;
    

}

