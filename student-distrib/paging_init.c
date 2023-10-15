#include "x86_desc.h"
#include "paging_asm.h"



void init_paging(); 

void init_paging(){
    int i,j;

    // This is for each PDE. (2) 1 for video mem and one for kernel
    for(i = 0; i < 1024; i++){
        pd[i].dir_0.P = 0;
        pd[i].dir_0.RW = 1;
        pd[i].dir_0.US = 0;
        pd[i].dir_0.PWT = 0;
        pd[i].dir_0.PCD = 0;
        pd[i].dir_0.A=  0;
        pd[i].dir_0.AVL1 = 0;
        pd[i].dir_0.PS = 0;
        pd[i].dir_0.AVL4 = 0;
        pd[i].dir_0.bits_31_12 = 0;

        if(i == 0 || i == 1){
            pd[i].dir_0.P = 1;
        }
    }



    // This is for each video mem (PTE)
    for(i = 0; i < 1024; i++){ // each PTE
        pt[i].P = 1;
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
            }
    }

    //0 video memory
    pd[0].dir_0.P = 1; 
    pd[0].dir_0.bits_31_12 = ((unsigned int)pt>>12); // pt[0] prolly cause 2D arr now

    //for kernel
    pd[1].dir_1.P = 1; 
    pd[1].dir_1.PS = 1; 
    pd[1].dir_1.bits_31_22 = 1;



    loadPageDirectory_asm((unsigned int)pd);
    enablePaging_asm();
    loadPSE();

}