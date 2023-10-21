#include "types.h"
#include "multiboot.h"


typedef struct dentry {
    int8_t filename[32]; //each char is 1 byte - 32 characters 
    int32_t filetype;
    int32_t inode_num;
    int8_t reserved[24];
} dentry_t; 

typedef struct boot_block {
    int32_t inode_count; 
    int32_t dir_count; 
    
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

int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry);
int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry);
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);
void init_file_sys(uint32_t* myfileSystem);

static uint32_t* myfileSystem;
boot_block_t* my_boot_block; //ADDED - MADE IT GLOBAL   
inode_t* inodeArr[63]; //inode array - how to make global with size boot_block_count; 
data_block_t* data_block_arr[1023]; 


///////////////////////read,write,open,close////////////////////////////////

int32_t read_file(int32_t fd, void* buf, int32_t nbytes);
int32_t write_file(int32_t fd, const void* buf, int32_t nbytes);
int32_t open_file(const uint8_t* filename);
int32_t close_file(int32_t fd);


int32_t read_dir(int32_t fd, void* buf, int32_t nbytes);
int32_t write_dir(int32_t fd, const void* buf, int32_t nbytes);
int32_t open_dir(const uint8_t* filename);
int32_t close_dir(int32_t fd);
