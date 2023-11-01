#include "i8259.h"
#include "lib.h"
#include "idt_handlers.h"
#include "cursor.h"
#include "file_system.h"
#include "paging_asm.h"
#include "System_Calls.h"
#include "x86_desc.h"
#include "keyboard.h"


void halt() {
    printf("halt");
    //while (1);
}


int32_t execute(const uint8_t* command) {
    printf("execute");
    //check if string is an executable
    
    uint8_t name[999]; //this initialization might be kinda sus 
    dentry_t temp_dentry; 
    int i = 0; 
    int fd; 
    int ret_val; 
    uint8_t boofer[999];
    
    ////////////////////CHECK IF IS AN EXECUTABLE/////////////////////////////////////////////////////////////
    
    //create filename array for just first word in space separated string
    while (command[i] != ' ' && command[i] != '\0'){
        name[i] = command[i];
         i ++; 
    }
    read_dentry_by_name(name, &temp_dentry);//get inode num to find file descriptor

    ret_val = read_data(temp_dentry.inode_num, 0, boofer, 4); //read first four bytes, check if its the magic numbers signifying an executable
    if (ret_val < 4) 
        return -1; 
        //0: 0x7f; 1: 0x45; 2: 0x4c; 3: 0x46.
    if (boofer[0] != 0x7f || boofer[1] != 0x45 || boofer[2] != 0x4c || boofer[3] != 0x46)
        return -1; 

    send_pid = get_new_pid(); //tells us which pcb is available 
    if(send_pid != -1){ //if its available 
        my_pids[send_pid] = 1;
    
    
    create_pcb(send_pid);
    program_pages(send_pid); //create page 
    read_data(temp_dentry.inode_num,0,0x8048000,0x8400000 - 0x8048000); //load page 
    
        asm volatile (" \
    movl %%cr3, %%eax ; \
    movl %%eax, %%cr3 ; \
" : /* no outputs */ \
  : /* no inputs */ \
  : "eax", "memory", "cc");
     }

uint8_t* buffy;
uint32_t start_adrr;
uint32_t ret;

read_data(temp_dentry.inode_num, 24,buffy, 4);
start_adrr = *(uint32_t*)(buffy);

tss.esp0 = 0x800000 - 4;
tss.ss0 = KERNEL_DS;

register uint32_t ebp_value asm("ebp");
register uint32_t esp_value asm("esp");

pcb_array[send_pid].prev_esp = esp_value;
pcb_array[send_pid].prev_ebp = ebp_value;


printf("aleenas armpits smell");
asm volatile("pushl %0 \n" //push the data segment 
              "pushl %1 \n"  //push the pointer to the top of the user stack(esp)
              "pushfl   \n" //push the flags
             " pushl %2 \n" //push the user CODE SEGMENT 
              "pushl %3 \n" //push the start address of ur program i thin k bytes 24-27
              "iret     \n"
            : /* no output */
            : "r" (USER_DS), \
              "r" (START_OF_USER_STACK - sizeof(uint32_t)), \
              "r" (USER_CS), \
              "r" (start_adrr)
            : "memory");
    printf("abhees poo poo hole smells");
    return -1; 
}










int32_t read_syscall(int32_t fd, void* buf, int32_t nbytes){
    pcb_array[send_pid].fd_array[fd]->jmp_table[0](fd, buf, nbytes);
    // printf("read syscall\n");
    //read(keyboard_buffer, 0); 
    return 10; 
    //  return; 
}
int32_t write_syscall(int32_t fd, const void* buf, int32_t nbytes){
    pcb_array[send_pid].fd_array[fd]->jmp_table[1](fd, buf, nbytes);
    //printf("write syscall\n");
    return 10;  

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t open_syscall(uint8_t* filename){
    //step one find fd
    int i = 0; 
    // while (file_descriptor_array[i] !=NULL){
    //     i ++; 
    //     if (i == 8)
    //         return -1; //open failed, no space in arrau
    // }
   // printf("i: %d\n", i);

    //step two figure out which open youre calling
    dentry_t temp_dentry; 
    read_dentry_by_name(filename, &temp_dentry); 
    int ret_val; 
    if (temp_dentry.filetype == 0){
        printf("file type 0");
        ret_val = rtc_open(); //this should take a filename but I think we set up the arg st it didn't we prib need to change this idrk tho 
    }

     if (temp_dentry.filetype == 1){
        printf("file type 1");
        ret_val = open_dir(filename); 
    }
    if (temp_dentry.filetype == 2){
        printf("open file called");
        ret_val = open_file(filename); 
    }
    if (ret_val == -1)
        return -1; 


    //step 3 add the file to the fd array
    printf("entering add function");
    add_file_descriptor_table_entry(i, filename, pcb_array[send_pid].fd_array);

    //printf("ret val: %d\n", ret_val);
    return ret_val; 


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



 void create_pcb(int index) {
    
    pcb_array[index].pid = index; 
    
    pcb_array[index].state = 1;

    init_file_descriptor(pcb_array[index].fd_array);
    
    return;
}