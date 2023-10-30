#include "i8259.h"
#include "lib.h"
#include "idt_handlers.h"
#include "cursor.h"
#include "file_system.h"
#include "paging_asm.h"
#include "System_Calls.h"
#include "x86_desc.h"


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



//we may need some invalid file checks here idrk 

 ////////////////////////////////////CHECK IF IS AN EXECUTABLE//////////////////////////////////////////////////////////////
   
    ret_val = read_data(temp_dentry.inode_num, 0, boofer, 4); //read first four bytes, check if its the magic numbers signifying an executable
    if (ret_val < 4) 
        return -1; 
        //0: 0x7f; 1: 0x45; 2: 0x4c; 3: 0x46.
    if (boofer[0] != 0x7f || boofer[1] != 0x45 || boofer[2] != 0x4c || boofer[3] != 0x46)
        return -1; 
/////////////////////////////////////////Set up program paging (4MB) && User-level Program Loader (load from FS to program page////////////////////////////////////////////////////////

    
    printf("at added part");
    
    uint8_t send_pid = get_new_pid();
    if(send_pid != -1){
        my_pids[send_pid] = 1;



    //we load into the same vm everytime but different phys everytime 
            program_pages(send_pid); //create page 
            read_data(5,0,0x08000000,37000); //load page 

    //////////////////flush//////////////////////////////////////////////
        asm volatile (" \
    movl %%cr3, %%eax ; \
    movl %%eax, %%cr3 ; \
" : /* no outputs */ \
  : /* no inputs */ \
  : "eax", "memory", "cc");
 
    ////////////////////////////////////////////////////////////////////

            for(i = 0; i < 1000; i++){
                printf("%c", *((char*)(0x08000000 + i)));
            }
    }

 ////////////////////////////////////////////////////////SET UP PCB///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 pcb_t my_pcb = create_pcb(send_pid );

uint32_t esp_value, ebp_value;

asm volatile(
    "mov %%esp, %0;"
    : "=r"(esp_value)
);

asm volatile(
    "mov %%ebp, %0;"
    : "=r"(ebp_value)
);

my_pcb.prev_esp = esp_value;
my_pcb.prev_ebp = ebp_value;
////////////////////////////////////////////////////////SET UP PCB///////////////////////////////////




 /////////////////////////////////////////////////CONTEXT SWITCH///////////////////////////////////////////////////////////////////////////////////////
//I think these are the only two variables you need to modify in your tss struct
//bc u need the kernel stack pointer and the starting address of the program you wanna execute(esp0) 



// Read the 4 bytes starting from offset 24 in the file and store it in 'buffer'
int32_t ret = read_data(temp_dentry.inode_num, 24, boofer, 4);
uint32_t start_prog_addr;

if (ret == 4) {
    // 'buffer' now contains the 4 bytes from bytes 24-27 of the file
    start_prog_addr = *((uint32_t*)boofer);

    // 'value' now holds the extracted 4-byte unsigned integer
    // You can use 'value' in your code as needed
} else {
    // Handle the case where not all 4 bytes were read
    return -1;
}


tss.esp0 = start_prog_addr; // setting up the 
tss.ss0 = KERNEL_DS;

/////////IRET//////////////////////////////////////
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
              "r" (start_prog_addr)
            : "memory");
 

    //while (1);
    return 255; 
}


void read_syscall(void){
     printf("read syscall");
}
void write_syscall(void){
     printf("write syscall");

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
    add_file_descriptor_table_entry(i, filename, );

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



 pcb_t create_pcb(int pid) {
//ONLY MODIFY THESE IN THE PCB FUNCTION for the current pcb
//init a new first two entries in the array forthe std values 
// my_pcb.fd_array = ;

// my_pcb.pid = ;

    pcb_t my_pcb;
    
    my_pcb.pid = pid; 
    
    my_pcb.state = 1;

    init_file_descriptor(my_pcb.fd_array);
    
    return my_pcb;
}