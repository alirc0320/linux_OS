#include "i8259.h"
#include "lib.h"
#include "idt_handlers.h"
#include "cursor.h"
#include "file_system.h"
#include "paging_asm.h"
#include "System_Calls.h"


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

    //IDRK if this is necesary
    for (i = 0; i< 8; i++){
        //NULL check
        if (file_descriptor_array[i] == NULL){
            fd = -1; 
            return -1; 
        }
            
        if (temp_dentry.inode_num == file_descriptor_array[i]->inode){
            fd = i; 
            break; 
        }
    }


//we may need some invalid file checks here idrk 

 ////////////////////////////////////CHECK IF IS AN EXECUTABLE//////////////////////////////////////////////////////////////
   
    ret_val = read_data(temp_dentry.inode_num, 0, boofer, 4); //read first four bytes, check if its the magic numbers signifying an executable
    if (ret_val < 4) 
        return -1; 
        //0: 0x7f; 1: 0x45; 2: 0x4c; 3: 0x46.
    if (boofer[0] != 0x7f || boofer[1] != 0x45 || boofer[2] != 0x4c || boofer[3] != 0x46)
        return -1; 
    file_descriptor_array[fd]->file_position = 4; 


/////////////////////////////////////////Set up program paging (4MB) && User-level Program Loader (load from FS to program page////////////////////////////////////////////////////////

    
    printf("at added part");
    
    uint8_t send_pid = get_new_pid();
     if(send_pid != -1){
        my_pids[send_pid] = 1;
        program_pages(send_pid); //create page 
        read_data(5,0,0x08000000,37000); //load page 

        for(i = 0; i < 1000; i++){
            printf("%c", *((char*)(0x08000000 + i)));
        }
    }

 ////////////////////////////////////////////////////////SET UP PCB///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//idk what to put in the arguments after send PID
 pcb_t my_pcb = create_pcb(send_pid, 1, 0, 0);
 





    

    //while (1);
    return 255; 
}















void read_syscall(void){
     printf("read syscall");
}
void write_syscall(void){
     printf("write syscall");

}


uint32_t open_syscall(uint8_t* filename){
    //step one find fd
    int i = 0; 
    while (file_descriptor_array[i] !=NULL){
        i ++; 
        if (i == 8)
            return -1; //open failed, no space in arrau
    }
    printf("i: %d\n", i);

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
    add_file_descriptor_table_entry(i, filename);

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



 pcb_t create_pcb(int pid, int initial_state, uint32_t initial_pc, uint32_t initial_sp) {
   pcb_t new_pcb;
    new_pcb.pid = pid;
    new_pcb.state = initial_state;
    new_pcb.pc = (void*)initial_pc;
    new_pcb.sp = (void*)initial_sp;
    // Initialize other fields as needed
    pcb_array[pid]= pcb_t new_pcb;
    return new_pcb;
}