#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "types.h"
#include "tests.h"


typedef struct terminal {
  uint32_t curr_x;          //x current cursor position 
  uint32_t curr_y;          //y current cursor position 
  uint32_t buffer_character_count;   //the buffers current count 128 max
  char keyboard_buffer[128];          //buffers current state 
  uint32_t terminal_id;
  int active_flag; 
  
} terminal_t;

static uint32_t current_terminal_id = 0; //global current terminal running (like target_pid)

terminal_t terminal_array[3]; //3 instances of the struct for 3 terminals 

void terminal_init();
void terminal_switch (uint32_t terminal_id); //pass in the terminal 


//int pid_of_terminals_array[3]; //tells what the parent pid for each parent shell is





#endif /* _TERMINAL_H */
