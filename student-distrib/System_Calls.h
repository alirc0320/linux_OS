#ifndef _SYSTEM_CALLS_H
#define _SYSTEM_CALLS_H


#include "tests.h"


#define START_OF_USER_STACK 0x8400000

typedef struct pcb {
    // Process ID
    int pid;
    int state;
    uint8_t* sp;
    uint8_t* prev_ebp;
    uint8_t* prev_esp;
    void* pc;
    file_descriptor_struct* fd_array[8];
} pcb_t;



pcb_t pcb_array[6];
pcb_t create_pcb(int index);

uint8_t send_pid;




void halt(void); 
int32_t execute(const uint8_t* command);
void read_syscall(void);
void write_syscall(void);
uint32_t open_syscall(uint8_t* filename);
uint8_t my_pids[6];
uint8_t get_new_pid();


#endif /* _SYSTEM_CALLS_H */


