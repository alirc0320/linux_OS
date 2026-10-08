



#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "types.h"
#include "tests.h"
#include "System_Calls.h"

#define NUM_TERMINALS 3

typedef struct terminal {
  uint32_t curr_x;          //x current cursor position
  uint32_t curr_y;          //y current cursor position
  uint32_t buffer_character_count;   //the buffers current count 128 max
  char keyboard_buffer[128];          //saved line buffer while this terminal is not on screen
  uint32_t terminal_id;
   int buffer_index;
  int turned_on;
  int pid; // index of the active process in this terminal's pcb array
  pcb_t* pcb_array[6]; //have one pcb array per terminal, swapped in by the scheduler
  uint32_t esp;             //saved kernel stack pointer (set by context_switch)
  volatile int enter_flag;  //a full line (ending in '\n') is waiting to be read
} terminal_t;


extern uint32_t current_terminal_id; //terminal shown on screen / receiving keyboard input
extern uint32_t sched_terminal;      //terminal whose process currently owns the CPU

terminal_t terminal_array[NUM_TERMINALS]; //3 instances of the struct for 3 terminals

void terminal_init();
void terminal_switch (uint32_t terminal_id); //pass in the terminal


#endif /* _TERMINAL_H */

