#ifndef _SYSTEM_CALLS_H
#define _SYSTEM_CALLS_H


#include "tests.h"


#define START_OF_USER_STACK 0x8400000

typedef struct pcb {
    // Process ID
    uint32_t pid;
    int state;
    uint8_t* sp;
    int inode_num;
    uint8_t* prev_ebp;
    uint8_t* prev_esp;
    void* pc;
    file_descriptor_struct_t* fd_array[8];
} pcb_t;



pcb_t* pcb_array[6];

pcb_t* create_pcb(uint32_t pid);

uint32_t target_pid;

int fd_full; 




int32_t halt(uint8_t status); 

int32_t execute(const uint8_t* command);
int32_t read_syscall(int32_t fd, void* buf, int32_t nbytes);
int32_t write_syscall(int32_t fd, const void* buf, int32_t nbytes);
uint32_t open_syscall(uint8_t* filename);
int32_t close_syscall(int32_t fd);
uint8_t my_pids[6];
uint8_t get_new_pid();


#endif /* _SYSTEM_CALLS_H */


