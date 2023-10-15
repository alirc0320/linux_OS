#include "x86_desc.h"
#include "paging_asm.h"



void init_paging(); 

void init_paging(){
    int i;

    // This is for each PDE. (2) 1 for video mem and one for kernel
    //for(i = 0; i < 1024; i++){
        pd[0].P = 1;
        pd[0].RW = 1;
        pd[0].US = 0;
        pd[0].PWT = 0;
        pd[0].PCD = 0;
        pd[0].A=  0;
        pd[0].AVL1 = 0;
        pd[0].PS = 0;
        pd[0].AVL4 = 0;
        pd[0].bits_31_12 = ((unsigned int)pt>>12);//this was previously zero but like why 

        // if(i == 0 || i == 1){
        //     pd[i].P = 1;
        //     // pd[0].bits_31_12 = ((unsigned int)pt>>12);
        // }
        
    //}

    pd[1].P = 1;
    pd[1].RW = 1;
    pd[1].US = 0;
    pd[1].PWT = 0;
    pd[1].PCD = 0;
    pd[1].A=  0;
    pd[1].AVL1 = 0;
    pd[1].PS = 1;
    pd[1].AVL4 = 1;
    pd[1].bits_31_12 = 1024;//this was previously zero but like why 

    //ali
     //for kernel
    // pd[1].dir_1.P = 1; 
    // pd[1].dir_1.PS = 1; 
    // pd[1].dir_1.bits_31_22 = 1024;
    // pd[1].dir_1.G = 1; 
    // pd[1].dir_1.RW = 1; 
    // pd[1].dir_1.US = 0; 
    // pd[1].dir_1.PWT = 0; 
    // pd[1].dir_1.PCD = 0; 
    // pd[1].dir_1.A = 0; 
    // pd[1].dir_1.D = 0; 
    // pd[1].dir_1.AVL = 0; 
    // pd[1].dir_1.PAT = 0; 
    // pd[1].dir_1.bits_39_32 = 0; 
    // pd[1].dir_1.RSVD = 0; 

    // This is for each video mem (PTE)
    for(i = 0; i < 1024; i++){ // each PTE
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
        pt[i].bits_31_12 = i; // POST SHIFTING

            if(i==184){
                pt[i].bits_31_12 = i; // this is the addr for the page in video mem. this is right shift by 12
                pt[i].P = 1; 
            }
    }

    //0 video memory
    // pd[0].P = 1; 
    // pd[0].bits_31_12 = ((unsigned int)pt>>12);


    loadPageDirectory_asm((unsigned int*)pd);//we need to pass by pointer I think thissl automaticalyl take mem address
    //printf("calling enable paging");
//     enablePaging_asm();
//    // printf("paging enable called");
//     loadPSE();

}