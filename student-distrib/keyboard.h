#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "types.h"
#include "tests.h"


void init_keyboard(); //initialize keyboard 


int32_t read(const char* buf, int nbytes) ;
int32_t write(const char* buf, int nbytes);
int keyboard_interrupt(void);
void init_buffer();
int32_t terminal_open();
int32_t terminal_close();


int ctrl_flag; 
int shift_flag; 
int capslock_flag;
int alt_flag; 

int scan_code;

char keyboard_buffer[128];
int keyboard_buffer_index; 
int kb_flag; 
 
int tab_count;
int flaggy;
int flaggy_2;

#endif /* _KEYBOARD_H */

