#include "types.h"
#include "multiboot.h"
#include "rtc.h"
#include "keyboard.h"

typedef struct dentry {
    int8_t filename[32]; //each char is 1 byte - 32 characters 
    int32_t filetype;
    int32_t inode_num;
    int8_t reserved[24];
} dentry_t; 

typedef struct boot_block {
    int32_t dir_count; 
    int32_t inode_count; 
    int32_t data_count; 
    int8_t reserved[52]; //52 * 8 bits 
    dentry_t direntries[63]; //63 * 8 bits 
} boot_block_t; 

typedef struct inode {
    int32_t length;
    int32_t data_block_num[1023];
} inode_t; 

typedef struct data_block {
    int8_t data_value[4096]; //this is 4096 bytes 
} data_block_t;




// int i;
// for (i = 0; i < 10; i++) {
//     fd.sys_call_table[i] = sys_call_table[i]; // Assign the addresses of system call functions
// }


//jump tables used on struict- will depend on devices





int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry);
int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry);
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);




uint32_t* myfileSystem;
boot_block_t* my_boot_block; //ADDED - MADE IT GLOBAL   
inode_t* inodeArr[63]; //inode array - how to make global with size boot_block_count; 
data_block_t* data_block_arr[1023]; 
//dentry_t* global_dentry;
dentry_t global_dentry[1];
uint32_t global_offset;
uint32_t index_offset;
 dentry_t dir_dentry[1]; //for directory 

///////////////////////read,write,open,close////////////////////////////////

int32_t read_file(int32_t fd, void* buf, int32_t nbytes);
int32_t write_file(int32_t fd, const void* buf, int32_t nbytes);
int32_t open_file(const uint8_t* filename);
int32_t close_file(int32_t fd);


int32_t read_dir(int32_t fd, void* buf, int32_t nbytes);
int32_t write_dir(int32_t fd, const void* buf, int32_t nbytes);
int32_t open_dir(const uint8_t* filename);
int32_t close_dir(int32_t fd);


static int32_t (*file_jump_table[4]) () = {read_file, write_file, open_file, close_file};

static int32_t (*dir_jump_table[4]) () = {read_dir, write_dir, open_dir, close_dir};

static int32_t (*rtc_jump_table[4]) () = {rtc_read, rtc_write, rtc_open, rtc_close};

static int32_t (*terminal_jump_table[4]) () = {read, write, terminal_open, terminal_close};



typedef struct file_descriptor_struct {
    //check syntax for all tjhe ump table stuff
    int32_t (*jmp_table[4])(); //set up an arbibtrary jump table, will be done by device
    int32_t inode;
    int32_t file_position;
    int32_t flags;
} file_descriptor_struct;

void init_file_descriptor();

file_descriptor_struct* file_descriptor_array[8]; 



void init_file_sys(uint32_t* myfileSystem);
void add_file_descriptor_table_entry(int fd, uint8_t* filename);



