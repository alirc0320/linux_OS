#include "i8259.h"
#include "lib.h"
#include "idt_handlers.h"
#include "cursor.h"
#include "file_system.h"
#include "paging_asm.h"
#include "System_Calls.h"
#include "x86_desc.h"
#include "keyboard.h"



int32_t halt(uint8_t status){
    printf("entered halt");
   // int my_eax = status;
    int i;

    /*
    Restore parent data
        What data?
        Where to find them?
    */

   uint8_t* ESP = pcb_array[target_pid]->prev_esp;
   uint8_t* EBP = pcb_array[target_pid]->prev_ebp;
   uint32_t RETURN_VALUE;
   

    asm volatile (" \
    movl %%cr3, %%eax ; \
    movl %%eax, %%cr3 ; \
    " : /* no outputs */ \
    : /* no inputs */ \
    : "eax", "cc");

    /*
    Restore parent paging (from ls back to shell)
    */
   program_pages(target_pid-1);
   //read_data(pcb_array[target_pid-1].inode_num,0,0x8048000,0x8400000 - 0x8048000);

    /*
    Clear file descriptor array
        What if you don’t…
    */
    for(i = 0; i < 8; i++){
        if(pcb_array[target_pid]->fd_array[i] == NULL){
            break;
        } 
        pcb_array[target_pid]->fd_array[i]->inode = 0;
        pcb_array[target_pid]->fd_array[i]->file_position = 0;
        pcb_array[target_pid]->fd_array[i]->flags = 0;
        pcb_array[target_pid]->fd_array[i]->jmp_table[0] = NULL;
        pcb_array[target_pid]->fd_array[i]->jmp_table[1] = NULL;
        pcb_array[target_pid]->fd_array[i]->jmp_table[2] = NULL;
        pcb_array[target_pid]->fd_array[i]->jmp_table[3] = NULL;
    }
    close_syscall(i-1);
    
    //Write Parent process’ info back to TSS(esp0)
    tss.esp0 =  0x800000 - (0x2000 * (target_pid - 1) )- 4;
    tss.ss0 = KERNEL_DS;

    my_pids[target_pid] = 0;
    target_pid--; 

    // if(status == 0){
    //     RETURN_VALUE = 0;
    // }
    // else{
    //     RETURN_VALUE = 256;
    // }
    RETURN_VALUE = 17;

    /*
    Jump to execute return (what does it even mean…)
    L-> in line assembly
    */
   printf("leaving halt");

    asm volatile("movl %0, %%esp \n"
              "movl %1, %%ebp \n"
              "movl %2, %%eax \n"
              "leave \n" // Jump to the label
              "ret \n" // Jump to the label
            : /* no output */
            : "r" (ESP), \
              "r" (EBP), \
              "r" (RETURN_VALUE)
            : "memory");

    return 0;
}


int32_t execute(const uint8_t* command) {
 if (command[0] == 'r')
        return -1; 
    uint8_t name[32]; //this initialization might be kinda sus 
    dentry_t temp_dentry[1]; 
    int i = 0; 
    int fd; 
    int ret_val; 
    uint8_t boofer[32];
    //uint32_t target_pid;
    pcb_t* pcb; 
uint8_t buffy[1];
uint32_t start_adrr;
uint32_t ret;
  
for (i = 0; i < 32; i++) {
    name[i] = 0;
}
i = 0;
    ////////////////////CHECK IF IS AN EXECUTABLE/////////////////////////////////////////////////////////////
    
    //create filename array for just first word in space separated string
    while (command[i] != ' ' && command[i] != '\0'){
        name[i] = command[i];
         i ++; 
    }
     
    if (read_dentry_by_name(name, temp_dentry) == -1){
        return -1; 
    }
    read_data(temp_dentry->inode_num, 0, boofer, 4);
    // ret_val = read_data(temp_dentry->inode_num, 0, boofer, 4); //read first four bytes, check if its the magic numbers signifying an executable
    // if (ret_val < 4) 
    //     return -1; 
        //0: 0x7f; 1: 0x45; 2: 0x4c; 3: 0x46.
    if (boofer[0] != 0x7f || boofer[1] != 0x45 || boofer[2] != 0x4c || boofer[3] != 0x46)
        return -1; 
//PCB//////////////////////////////////
    for (i = 0; i < 6; i++) {   // get a pid
        if (my_pids[i] == 0)
            break;
    }

        if (i == 6)                 // no pid can be used
        return -1;

        my_pids[i] = 1;           // activate pid
        target_pid = i;

     /* Get pcb structure */
    pcb = create_pcb(target_pid);
    pcb->pid = target_pid;
    pcb->inode_num = temp_dentry->inode_num;

//PCB//////////////////////////////////


    asm volatile (" \
    movl %%cr3, %%eax ; \
    movl %%eax, %%cr3 ; \
" : /* no outputs */ \
  : /* no inputs */ \
  : "eax", "cc");
     
    program_pages(target_pid); //create page 
    
    read_data(temp_dentry->inode_num,0,0x8048000,0x8400000 - 0x8048000); //load page 
    
      

read_data(temp_dentry->inode_num, 24,buffy, 4);
start_adrr = *(uint32_t*)(buffy);

tss.esp0 = 0x800000 - (0x2000 * (target_pid+1) )- 4;
tss.ss0 = KERNEL_DS;

register uint32_t ebp_value asm("ebp");
register uint32_t esp_value asm("esp");

pcb_array[target_pid]->prev_esp = esp_value;
pcb_array[target_pid]->prev_ebp = ebp_value;

asm volatile("pushl %0 \n" //push the data segment 
              "pushl %1 \n"  //push the pointer to the top of the user stack(esp)
              "pushfl   \n" //push the flags
              "pushl %2 \n" //push the user CODE SEGMENT 
              "pushl %3 \n" //push the start address of your program i think bytes 24-27
              "iret     \n"
            : /* no output */
            : "r" (USER_DS), \
              "r" (START_OF_USER_STACK - sizeof(uint32_t)), \
              "r" (USER_CS), \
              "r" (start_adrr)
            : "memory");

    // THIS IS SUPPOSED TO BE RETURN SOME SHIT

    return 17; 
}










int32_t read_syscall(int32_t fd, void* buf, int32_t nbytes){
    
    if(fd_full == 1){
        return -1; 
    }
    return pcb_array[target_pid]->fd_array[fd]->jmp_table[0](fd, buf, nbytes); 

    
    if(fd_full == 1){
        return -1; 
    }
   
    //printf("read");
    //return pcb_array[send_pid].fd_array[fd]->jmp_table[0](fd, buf, nbytes);
     if (fd <= 1){
        return read(fd, buf, nbytes);
     }
     int inode = pcb_array[target_pid]->fd_array[fd]->inode; 
     int i; 
     dentry_t temp_dentry;
     for (i = 0; i< my_boot_block->dir_count; i++){
        if (my_boot_block->direntries[i].inode_num == inode){
             temp_dentry = my_boot_block->direntries[i]; 
            break;
        }
     }

     if (temp_dentry.filetype == 0){
        return rtc_read(fd, buf, nbytes);
     }
     if (temp_dentry.filetype == 1){
        int i; 
        for (i = 0; i< 100; i++){
         read_dir(fd, buf, nbytes);
        }
        return 0; 
     }

    if (temp_dentry.filetype == 2){
        return read_file(fd, buf, nbytes);
     }

    return 0; 
    //printf("past read");
    // printf("read syscall\n");
    //read(keyboard_buffer, 0); 
}




int32_t write_syscall(int32_t fd, const void* buf, int32_t nbytes){
    if(fd_full == 1){
        return -1; 
    }

    return pcb_array[target_pid]->fd_array[fd]->jmp_table[1](fd, buf, nbytes);

    // if(fd_full == 1){
    //     return -1; 
    // }
   
    
    //printf("write");
    // pcb_array[send_pid].fd_array[fd]->jmp_table[1](fd, buf, nbytes);
    //  if (fd <= 1){
    //     return write(fd, buf, nbytes);
    //  }
    // int inode =  pcb_array->fd_array[fd]->inode; 
    // int i; 
    // dentry_t temp_dentry; 
    // for (i = 0; i< my_boot_block->dir_count; i++){
    //     if (my_boot_block->direntries[i].inode_num == inode){
    //          temp_dentry = my_boot_block->direntries[i]; 
    //         break;
    //     }
    //  }

    //  if (temp_dentry.filetype == 0){
    //     return rtc_write(fd, buf, nbytes);
    //  }
    //  if (temp_dentry.filetype == 1){
    //     return write_dir(fd, buf, nbytes);
    //  }

    // if (temp_dentry.filetype == 2){
    //     return write_file(fd, buf, nbytes);
    //  }

    //printf("write syscall\n");
    return 0; 








}






/////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t open_syscall(uint8_t* filename){
    //step one find fd
    int i = 0;
    fd_full = 0;  
    //execute(filename);
    while (pcb_array[target_pid]->fd_array[i] !=NULL){
        i ++; 
        if (i == 8){
            fd_full = 1; 
            return -1; //open failed, no space in arrau
        }
    }
    fd_index = i; 

//    printf("i: %d\n", i);

    //step two figure out which open youre calling
    dentry_t temp_dentry; 
    read_dentry_by_name(filename, &temp_dentry); 
    int ret_val; 
    if (temp_dentry.filetype == 0){
        //printf("file type 0");
        ret_val = rtc_open(NULL); //this should take a filename but I think we set up the arg st it didn't we prib need to change this idrk tho 
    }

     if (temp_dentry.filetype == 1){
        //printf("file type 1");
        
        ret_val = open_dir(filename); 
        
    }
    if (temp_dentry.filetype == 2){
        //printf("open file called");
        ret_val = open_file(filename); 
    }
    if (ret_val == -1)
        return -1; 


    //step 3 add the file to the fd array
    //printf("entering add function");
    add_file_descriptor_table_entry(i, filename, pcb_array[target_pid]->fd_array);

    //printf("ret val: %d\n", ret_val);
    return fd_index; 


}

int32_t close_syscall(int32_t fd){
    global_offset = 0;
    index_offset = 0;
}



 uint8_t get_new_pid(){
    int i;
    for(i =0; i < 6; i++){
        if(my_pids[i] == 0){
            return i;
        } 
    }
    return -1;
 }


 pcb_t* create_pcb(uint32_t pid) {
    pcb_t* temp_pcb = (pcb_t*)(0x800000 - (pid+1) * 0x2000);
    temp_pcb->pid = pid; 
    temp_pcb->state = 1; 

    pcb_array[pid] = temp_pcb; 


    init_file_descriptor(pcb_array[pid]->fd_array);
    
    
    
    
    
    
    
    
    
    return pcb_array[pid]; 
    
    // pcb_array[pid].pid = pid; 
    
    // pcb_array[pid].state = 1;

    

//    return (pcb_t*)(0x800000 - (pid+1) * 0x2000);


}