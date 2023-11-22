#ifndef _PIT_H
#define _PIT_H


void init_pit();
void pit_interrupt();
void scheduler(uint32_t target_pid); 
static int counter = 0;

extern uint32_t scheduler_array[3]; 

#endif /* _LIB_H */