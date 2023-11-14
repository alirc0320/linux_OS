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
} terminal_t;

uint32_t current_terminal_id; //global current terminal running (like target_pid)

terminal_t terminal_array[3]; //3 instances of the struct for 3 terminals 

void terminal_init();
void terminal_switch (uint32_t terminal_id); //pass in the terminal 








#endif /* _TERMINAL_H */
