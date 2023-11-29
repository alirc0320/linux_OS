

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

//save current process esp and ebp 

    counter = counter % 3; 
    // printf("counter: %d\n", counter);
    //saving the esp and ebp for the current process for the round robin 
    register uint32_t ebp_value asm("ebp"); //cuirrentm vals
    register uint32_t esp_value asm("esp"); 
  
    // terminal_array[current_terminal_id].esp = (uint8_t*) esp_value; 
    // terminal_array[current_terminal_id].ebp = (uint8_t*) ebp_value; 
    // if (terminal_array[counter].turned_on == 1){

    //    // scheduler(terminal_array[counter].pid); 
        send_eoi(0); //moved up here above execute shell call bc of inf while loop in execute
        process_switch((counter + 1)%3);
        //terminal_switch((counter + 1)%3);
    // }
    // cli(); 
    // send_eoi(0);
    //  process_switch((counter + 1)%3);
    // sti(); 

    //  register uint32_t ebp_value asm("ebp"); //cuirrentm vals
    // register uint32_t esp_value asm("esp");
    // save_old(counter, (counter + 1)%3, ebp_value, esp_value);
    // restore_new( (counter + 1)%3);
    
    counter++;


    

//call scheduler function 
// send_eoi(0);
}




void scheduler(uint32_t pidd){
    
    // if(target_pid == -1){
    //     return;
    // }
    // do schedulings

    //1) Save esp & ebp 
    // Save the previous ESP and EBP values
    // register uint32_t ebp_value asm("ebp");
    // register uint32_t esp_value asm("esp");

    //2)get the current pcb

    //3)find out what the next process to run is 

    //4)get that processed pid 

    //5)in case the next terminal process has not been initialized remap the video memory and execute the shell for initialization

    //6)remap user program pages for the next scheduled process && flush tlb

    //7)check and remap buffer and vid mem

    //8)Set TSS with proper kernel and stack pointer

    //9)retrieve pcb of next scheduled process and update ebp for context switching

    //10)perform context switch using return intrucrion with correct ebp



// int old_counter = counter - 1; 
// if (old_counter < 0){
//     old_counter = 2; 
// }

//     uint32_t ESP;
// uint32_t EBP;


//     register uint32_t ebp_value asm("ebp"); //cuirrentm vals
//     register uint32_t esp_value asm("esp");
  
  
// //get rid of pointers
//     terminal_array[old_counter].esp =  esp_value; 
//     terminal_array[old_counter].ebp =  ebp_value; 
//     terminal_array[old_counter].ss =  tss.ss0; 
//     terminal_array[old_counter].esp0 = tss.esp0;


//     if(counter > 2 || counter < 0){
//         return;
//     }



  
//     int i = 0; 
//     for (i = 0; i< 6; i++){
//         terminal_array[old_counter].pcb_array[i] = pcb_array[i]; //store cb array of what you are about to change from
//         pcb_array[i] = terminal_array[counter].pcb_array[i]; //store pcb array with what y9ou are going to
//     }

//     terminal_array[old_counter].pid = target_pid; 
//     target_pid = terminal_array[counter].pid; 






//  tss.ss0 = terminal_array[counter].ss; 
//  tss.esp0 = terminal_array[counter].esp0; 
 






// ESP = terminal_array[counter].esp; 
// EBP = terminal_array[counter].ebp; 


//  asm volatile(
//             "movl %0, %%esp \n"
//               "movl %1, %%ebp \n"
//             : /* no output */
//             : "r" (ESP), "r" (EBP)
//             : "memory");

//             return;


}
