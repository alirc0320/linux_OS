#include "x86_desc.h"
#include "paging_asm.h"



void init_paging(); 

void init_paging(){
    int i,j;

    // This is for each PDE. (2) 1 for video mem and one for kernel
    //for(i = 0; i < 1024; i++){
        pd[0].dir_0.P = 1;
        pd[0].dir_0.RW = 1;
        pd[0].dir_0.US = 0;
        pd[0].dir_0.PWT = 0;
        pd[0].dir_0.PCD = 0;
        pd[0].dir_0.A=  0;
        pd[0].dir_0.AVL1 = 0;
        pd[0].dir_0.PS = 0;
        pd[0].dir_0.AVL4 = 0;
        pd[0].dir_0.bits_31_12 = ((unsigned int)pt>>12);//this was previously zero but like why 

        // if(i == 0 || i == 1){
        //     pd[i].dir_0.P = 1;
        //     // pd[0].dir_0.bits_31_12 = ((unsigned int)pt>>12);
        // }
        
    //}

    
     //for kernel
    pd[1].dir_1.P = 1; 
    pd[1].dir_1.PS = 1; 
    pd[1].dir_1.bits_31_22 = 1;
    pd[1].dir_1.G = 1; 
    pd[1].dir_1.RW = 1; 
    pd[1].dir_1.US = 0; 
    pd[1].dir_1.PWT = 0; 
    pd[1].dir_1.PCD = 0; 
    pd[1].dir_1.A = 0; 
    pd[1].dir_1.D = 0; 
    pd[1].dir_1.AVL = 0; 
    pd[1].dir_1.PAT = 0; 
    pd[1].dir_1.bits_39_32 = 0; 
    pd[1].dir_1.RSVD = 0; 

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
               pt[i].bits_31_12 = 0xB8000/4096; // this is the addr for the page in video mem. this is right shift by 12
                pt[i].P = 1; 
            }
    }

    //0 video memory
    // pd[0].dir_0.P = 1; 
    // pd[0].dir_0.bits_31_12 = ((unsigned int)pt>>12); // pt[0] prolly cause 2D arr now



   

    
    

//    uint32_t P : 1;
//         uint32_t RW : 1;
//         uint32_t US : 1;
//         uint32_t PWT : 1;
//         uint32_t PCD : 1;
//         uint32_t A:  1;
//         uint32_t D : 1;
//         uint32_t PS : 1;
//         uint32_t G : 1;
//         uint32_t AVL : 3;
//         uint32_t PAT : 1;
//         uint32_t bits_39_32 : 8;
//         uint32_t RSVD : 1;
//         uint32_t bits_31_22 :10;

    loadPageDirectory_asm((unsigned int*)pd);//we need to pass by pointer I think thissl automaticalyl take mem address
    //printf("calling enable paging");
//     enablePaging_asm();
//    // printf("paging enable called");
//     loadPSE();

}