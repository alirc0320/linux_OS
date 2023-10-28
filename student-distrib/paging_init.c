#include "x86_desc.h"
#include "paging_asm.h"



/* Initialize paging 
 * Inputs: None
 * Outputs: None
 * Function: this funtion sets up paging my creating enteries in the PD and the PT. We have 2 entried in the directory and 1 PTE - used for video 
 * memeory and for kernel 
 *  */


void init_paging(){
    int i;

    // This is for each PDE. (2) 1 for video mem and one for kernel
    //here we are settng the respective bits in the 32 PDE to the correct values 
    //this i th create a PDE at index 0 of the page directory 
    //for(i = 0; i < 1024; i++){
        pd[0].P = 1; //set present bit 1 - meaning that the this entry exists 
        pd[0].RW = 1;
        pd[0].US = 0;
        pd[0].PWT = 0;
        pd[0].PCD = 0;
        pd[0].A =  0;
        pd[0].AVL1 = 0;
        pd[0].PS = 0;
        pd[0].AVL4 = 0;
        pd[0].bits_31_12 = ((unsigned int)pt>>12);//this is to a put the address of the the page table into the PDE 
     


//this is to create an entry of the PD at index 1 for the kernel space 
    pd[1].P = 1; //set present bit 1 - meaning that the this entry exists 
    pd[1].RW = 1;
    pd[1].US = 0;                   //urw for kernel
    pd[1].PWT = 0;
    pd[1].PCD = 0;
    pd[1].A=  0;
    pd[1].AVL1 = 0;
    pd[1].PS = 1; //set that the size should be 4MB 
    pd[1].AVL4 = 1; //since we're using the same 32 bits as when ps 0, we need to set the LSB of AVL4 to be 1, this sets the G bit high for flushing CR3 
    pd[1].bits_31_12 = 1024;//points to the pages in the kernel - we "skip" the PTE step 

    // This is for each video mem (PTE)
    for(i = 0; i < 1024; i++){ // each PTE - this is to create an entire 
        pt[i].P = 0;
        pt[i].RW = 1;
        pt[i].US = 0;
        pt[i].PWT = 0;
        pt[i].PCD = 0;
        pt[i].A = 0;
        pt[i].D = 0;
        pt[i].PAT = 0;
        pt[i].G = 0;
        pt[i].AVL = 0;
        pt[i].bits_31_12 = i; // POST SHIFTING (i * 4096 >> 12)

            if(i==184){ //we only want the entry at the video memory to be present - 
                 pt[i].bits_31_12 = i; // this is the addr for the page in video mem. this is right shift by 12
                pt[i].P = 1; //making it so that only this entry is present for the PTE 
            }
    }





//call for assembly linkage for setting up paging
    loadPageDirectory_asm((page_dir_t*)pd);//we need to pass by pointer I think thissl automaticalyl take mem address
}




void program_pages(uint8_t send_pid, uint8_t my_boofer){

    /////////////////////////////pde for progam image////////////////////////////////////////////
    pd[32].P = 1; 
    pd[32].RW = 1;
    pd[32].US = 0;                   
    pd[32].PWT = 0;
    pd[32].PCD = 0;
    pd[32].A=  0;
    pd[32].AVL1 = 0;
    pd[32].PS = 1; //set that the size should be 4MB 
    pd[32].AVL4 = 1; //since we're using the same 32 bits as when ps 0, we need to set the LSB of AVL4 to be 1, this sets the G bit high for flushing CR3 
    pd[32].bits_31_12 = 2048+send_pid*1024;//points to the pages in the kernel - we "skip" the PTE step 


    /*
    read_data(5,0,0x08048000,nbytes);
    */






    return;

}


