#include "file_system.h"
#include "lib.h"

//initalize the giant buffer 
void init_file_sys(uint32_t * myfileSystem, uint32_t* fileSysEnd){
     printf("entering init\n");
    //?????? we don't  have access to size_t 
    int copy_size = 4096; // 4kB - size we want to copy
    
    int var = (int)&my_boot_block;
    //printf("Value of var: %x\n", var);
    
    //memcpy(&my_boot_block, myfileSystem, copy_size); //taking the first 4kb of the file sys and put it in boot block 
    my_boot_block = (boot_block_t*)(myfileSystem);
    if(my_boot_block == (boot_block_t*)(myfileSystem)){

    }
    //printf("after memcpy \n");


///////////DO WE WANT THE ARRAY TO THE SIZE OF INODES?//////////////////////////////////////////////////////
    
    int i; 
    // int counter = 0; 
    for(i=0; i<my_boot_block->dir_count; i++){ //this is to only create the number of inodes needed 
        //had to pass inodeArr as an addr 
        //memcpy(&inodeArr[i], myfileSystem + copy_size * (i+1),copy_size); //creating inode arr at i+1 index of the giant buffer 
        dentry_t temp_dentry = my_boot_block->direntries[i];
        
        read_dentry_by_name(temp_dentry.filename, &temp_dentry);
        // if (temp_dentry.inode_num == 5){
        //     printf("5 reached");
        // }
        
        inodeArr[temp_dentry.inode_num] = (inode_t*)(myfileSystem + 1024  + (1024 * temp_dentry.inode_num));
        
        
    }  

   
    //printf("after first for loop");

    
    //make new arr for data blocks 
    for(i = 0; i< my_boot_block->data_count; i++){
        // memcpy(&data_block_arr[i], myfileSystem + copy_size * (i+1+my_boot_block->inode_count),copy_size);
         data_block_arr[i] = (data_block_t*)(myfileSystem + (1024 * 65) + (i * 1024));
    }
    //printf("after forst for loop");

}

int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry) {
    int i; 
    for(i = 0; i< 32; i++){
        printf("%c", fname[i]);
    }
    printf("entered dentry by name\n");

    uint8_t bounded_fname[32];  // 32 characters plus null-terminator
    int length = 0;

    if (fname == NULL)
        return -1;  // Not a valid string (null pointer)

    while (fname[length] != '\0' && length < 32) { // get the length of the fname
       length++;
    }
    printf("finished while looop for length\n");

  
   // int i; 
    for ( i = 0; i < 32; i++) {
        bounded_fname[i] = fname[i];
    }
    
    // for (i = 0; i< 32; i++){
    //     printf("%c", bounded_fname[i]);
    // }
    int index = 0; 
    strcpy(dentry->filename, bounded_fname);
    // printf("bounded name: %c", bounded_fname[0]);
    // printf("dentry fileName: %c", dentry->filename[0]);
        for(i = 0; i < 64; i++){        ////WE NEED THE LENGTH AS A PARAM 
            if(i < 3)
                printf("Currname: %c\n", my_boot_block->direntries[i].filename[0]);
            if (strncmp(bounded_fname, my_boot_block->direntries[i].filename, length) == 0) {//if 0, means the strings are the same 
                index = i; 
                read_dentry_by_index(index, dentry);
                return 0; 
            }
            
        }
 return -1;
}

int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry){
    if (index < 0 || index >= 63){
        return -1; 
    }
    dentry->filetype = my_boot_block->direntries[index].filetype;
    dentry->inode_num = my_boot_block->direntries[index].inode_num;
    return 0; 
}

int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length){
    //offset = offset within file - where you want to start reading 
            // in bytes, can start within a data block 

            printf("entering read data");
    int i;
  
      //null check
    if(inode < 0 || inode >= 63){ //if outside of num of files 
        return -1; 
    }

    inode_t* tempInode = inodeArr[inode]; //create a temporary inode that holds just the current inode that we're getting data from 


    if(length <0 || offset > tempInode->length) {
        return -1 ;
    }

    if(tempInode->length - offset < length){ //if length (what we want to read) is greater than remaining bytes - if offset + length is greater than file length
        length = tempInode->length - offset; //make the length the max bytes that can be read with the given offset 
    }

      int ret_length = length;

    printf("a bad word\n");

    int iNode_block_idx = offset/4096; //index of the block data array from out inode struct 

    int start_block_num = tempInode->data_block_num[iNode_block_idx]; //get data block located in the inode - nemory addr 

    data_block_t* start_block = data_block_arr[start_block_num]; //get the data block from the actual file 

    int offset_into_start_block = offset % 4096; //getting the offset for the correct 4kb block of data in the file 
    int temp_start_len = 0; //
    if(4096 - offset_into_start_block > length ){  //if the space between the end of the data block and the offset is larger than the area we want to read (read within a block)
        temp_start_len = length; //then set our temp length = to bytes we want to read, bc its within a block 
        //printf("if ");
    }
    else{ //if the space between the end of the block and the offset is smaller than the total length we want to read 
        temp_start_len = 4096 - offset_into_start_block; //our temp length is just from the offset to the end of the block 
       // printf("else");
    }
    printf("temp_start_len is: %d\n", temp_start_len);
    for( i = 0; i < temp_start_len; i++){
        //index the buff at the # of the block at the certain index of our inode 
        //start block num = 237 
        //start block num * 4096 = to become kB
        // + offset to get the start 
        //+ i so that we can increment through each byte 
        //
        //printf("i: %u", i);
        
       //memcpy(&buf[(start_block_num * 4096) + offset_into_start_block + i], &(data_block_arr[start_block_num]->data_value[i + offset_into_start_block]) ,1);
    //    buf[(start_block_num * 4096) + offset_into_start_block + i] = (data_block_arr[start_block_num]->data_value[i + offset_into_start_block]);
        buf[i] = (data_block_arr[start_block_num]->data_value[i + offset_into_start_block]);
       // &data_block_arr[curr_block_num].data_value[i]
       //buf[(start_block_num * 4096) + offset_into_start_block + i] = data_block_arr[(start_block_num * 4096) + offset_into_start_block + i];
    }
     printf("first for loop completed\n");
    // printf("pee pee poo poo");
    length -= temp_start_len;
   // printf("length: \n", length);
    iNode_block_idx+=1;

    int count = 0; 
//offset is actually temp _start_length
    while(length/4096 != 0){
        int curr_block_num = tempInode->data_block_num[iNode_block_idx]; //get data block located in the inode - nemory addr 
        data_block_t* curr_block = data_block_arr[curr_block_num]; //get the data block from the actual file

        for(i=0; i<4096; i++){
            //memcpy(&buf[(curr_block_num * 4096) + i], &data_block_arr[curr_block_num]->data_value[i] ,1); 
            //buf[(curr_block_num * 4096) + i] = data_block_arr[curr_block_num]->data_value[i];
            //data_block_arr - holds all data blocks , curr_block_num - is the data block we want
            //then data_value[i] is getting the actual data value 
            buf[i + temp_start_len] = data_block_arr[curr_block_num]->data_value[i];
        }

        length -= 4096;
        iNode_block_idx+=1; 
        count ++; 
    }

    printf("first while loop completed\n");


    if(length != 0){
        int curr_block_num = tempInode->data_block_num[iNode_block_idx]; //get data block located in the inode - nemory addr 
        data_block_t* curr_block = data_block_arr[curr_block_num]; //get the data block from the actual file

        for(i=0; i<length; i++){
            //memcpy(&buf[(curr_block_num * 4096) + i],  &data_block_arr[start_block_num]->data_value[i] ,1);
            //buf[(curr_block_num * 4096) + i] = data_block_arr[start_block_num]->data_value[i];
            buf[i + 4096 * count + temp_start_len] = data_block_arr[curr_block_num]->data_value[i];
        }
        length = 0;
    }
    return ret_length; 
}




int32_t read_file(int32_t fd, void* buf, int32_t nbytes){
    return read_data(inode_passed_in, offset_passed_in, buf, nbytes); 
}


int32_t open_file(const uint8_t* filename){
    init_file_sys(myfileSystem, fileSysEnd);
    return 0;
}


int32_t write_file(int32_t fd, const void* buf, int32_t nbytes){
    return -1; 
}

int32_t close_file(int32_t fd){
    int i;
    
    my_boot_block = 0; 

    for(i = 0; i<63; i++){
        inodeArr[i] = 0; 
    }
    
    for(i = 0; i<1023; i++){
        data_block_arr[i] = 0; 
    }

    return 0; 
}




int32_t open_dir(const uint8_t* filename){
    dentry_t dentry;
    read_dentry_by_name(filename, &dentry);
    //open a directory file
    return 0;
}

int32_t read_dir(int32_t fd, void* buf, int32_t nbytes){
  // read each file's file name by file name
  //read_dentry_by_index(uint32_t index, dentry_t* dentry);
  return 0; 
}

int32_t close_dir(int32_t fd){
    return 0; 
}

int32_t write_dir(int32_t fd, const void* buf, int32_t nbytes){
    return -1; 
}






