#ifndef _PAGING_ASM_H
#define _PAGING_ASM_H

#include "types.h"



/* A page directory struct (stores 1024 pdes) */
typedef struct page_dir_0 {
        uint32_t P : 1;
        uint32_t RW : 1;
        uint32_t US : 1;
        uint32_t PWT : 1;
        uint32_t PCD : 1;
        uint32_t A:  1;
        uint32_t AVL1 : 1;
        uint32_t PS : 1;
        uint32_t AVL4 : 4;
        uint32_t bits_31_12 : 20;
} page_dir_0_t;


typedef struct page_dir_1 {
        uint32_t P : 1;
        uint32_t RW : 1;
        uint32_t US : 1;
        uint32_t PWT : 1;
        uint32_t PCD : 1;
        uint32_t A:  1;
        uint32_t D : 1;
        uint32_t PS : 1;
        uint32_t G : 1;
        uint32_t AVL : 3;
        uint32_t PAT : 1;
        uint32_t bits_39_32 : 8;
        uint32_t RSVD : 1;
        uint32_t bits_31_22 :10;
} page_dir_1_t;

typedef union page_dir_t{
    page_dir_0_t dir_0;
    page_dir_1_t dir_1;
} page_dir_t;

/* A page table struct (stores 1024 ptes) */
    typedef struct page_table  {
        uint32_t P : 1;
        uint32_t RW : 1;
        uint32_t US : 1;
        uint32_t PWT : 1;
        uint32_t PCD : 1;
        uint32_t A:  1;
        uint32_t D : 1;
        uint32_t PAT : 1;
        uint32_t G : 1;
        uint32_t AVL : 3;
        uint32_t bits_31_12 : 20;
    } page_table_t;

page_dir_0_t pd[1024] __attribute__((aligned(4096)));
page_table_t pt[1024] __attribute__((aligned(4096)));

void loadPageDirectory_asm(page_dir_t *pd);
// void enablePaging_asm();
// void loadPSE(); 
#endif /* ASM */

//#endif /* _x86_DESC_H */