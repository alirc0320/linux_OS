#ifndef _SYSTEM_CALLS_H
#define _SYSTEM_CALLS_H


#include "tests.h"



typedef struct pcb {
    // Process ID
    int pid;
    // Process state (e.g., running, ready, blocked, terminated)
    int state;
    // CPU registers
    // You may include registers like EAX, EBX, ECX, etc. as needed for context switching
    uint32_t registers[8];
    // Program Counter (PC)
    void* pc;
    // Stack Pointer (SP)
    void* sp;

    //fd array
    file_descriptor_struct* fd_array[8];

} pcb_t;

pcb_t pcb_array[6];
pcb_t* create_pcb(int pid, int initial_state, uint32_t initial_pc, uint32_t initial_sp);




void halt(void); 
int32_t execute(const uint8_t* command);
void read_syscall(void);
void write_syscall(void);
uint32_t open_syscall(uint8_t* filename);
uint8_t my_pids[6];
uint8_t get_new_pid();


#endif /* _SYSTEM_CALLS_H */