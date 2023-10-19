#include "tests.h"
#include "x86_desc.h"
#include "lib.h"

#define PASS 1
#define FAIL 0

/* format these macros as you see fit */
#define TEST_HEADER 	\
	printf("[TEST %s] Running %s at %s:%d\n", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result)	\
	printf("[TEST %s] Result = %s\n", name, (result) ? "PASS" : "FAIL");

static inline void assertion_failure(){
	/* Use exception #15 for assertions, otherwise
	   reserved by Intel */
	asm volatile("int $15"); //change this everytime you wanna test 
}

//ali likes you
/* Checkpoint 1 tests */

/* IDT Test - Example
 * 
 * Asserts that first 10 IDT entries are not NULL
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
int idt_test(){
	TEST_HEADER;

	int i;
	int result = PASS;
	for (i = 0; i < 10; ++i){
		if ((idt[i].offset_15_00 == NULL) && 
			(idt[i].offset_31_16 == NULL)){
			assertion_failure();
			result = FAIL;
		}
	}
	return PASS; 
}

// add more tests here

int invalidPageAccess(){
    int i;
    char * ptr;
    int value;
    for(i = 0x400000; i < 0x800000; i++){
        ptr = (char *)i;
        value = *ptr;
    }
    return PASS;
}

int invalidMaxTest(){
    int i = 0x810000;
    char* ptr;
    int value;
    ptr = (char *)i;
    value = *ptr;
    return FAIL;
}

int invalidMinTest(){
    int i = 0x390000;
    char* ptr;
    int value;
    ptr = (char *)i;
    value = *ptr;
    return FAIL;
}

int invalidVidMemTest(){
    int i;
    char* ptr;
    int value;
    for(i = 0xb8000; i < 0xb9000; i++){
        ptr = (char *)i;
        value = *ptr;
    }
    return PASS;
}



/* Checkpoint 2 tests */
/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */


/* Test suite entry point */
void launch_tests(){

	//NOT SURE IF WE NEED THIS 
	//TEST_OUTPUT("idt_test", idt_test());
	
	
	//MAKE FLAG = 1 FOR RTC OR 0 FOR KEYBOARD INTERRUPT 
	flag = 0;
	

	// // Trigger exception 0
	// asm volatile("int $0");

	// // Trigger exception 1
	// asm volatile("int $1");

    // // Trigger exception 2
    // asm volatile("int $2");

    // // Trigger exception 3
    // asm volatile("int $3");

    // // Trigger exception 4
    // asm volatile("int $4");

	// // Trigger exception 5
	// asm volatile("int $5");

    // // Trigger exception 6
    // asm volatile("int $6");

    // // Trigger exception 7
    // asm volatile("int $7");

    // // Trigger exception 8
    // asm volatile("int $8");

    // // Trigger exception 10
    // asm volatile("int $10");

    // // Trigger exception 11
    // asm volatile("int $11");

    // // Trigger exception 12
    // asm volatile("int $12");

    // // Trigger exception 13
    // asm volatile("int $13");

    // // Trigger exception 14
    // asm volatile("int $14");

    // // Trigger exception 16
    // asm volatile("int $16");

    // // Trigger exception 17
    // asm volatile("int $17");

    // // Trigger exception 18
    // asm volatile("int $18");

    // // Trigger exception 19
    // asm volatile("int $19");

    // // Trigger exception 20
    // asm volatile("int $20");

    // // Trigger exception 21
    // asm volatile("int $21");

    // // Trigger exception 28
    // asm volatile("int $28");

    // // Trigger exception 29
    // asm volatile("int $29");

    // // Trigger exception 30
    // asm volatile("int $30");
	
	// // System Call
   // asm volatile("int $128");


    // invalidPageAccess();
    // invalidMaxTest();
    // invalidMinTest();
    //invalidVidMemTest();


















	
	
	//Overflow excpetion 
	//asm volatile("int $4");
	
	// Trigger a Non-Maskable Interrupt (NMI)
    //asm volatile("int $2");

	//Debug exception
	//asm volatile("int $1");

	//Invalid Opcode Exception
	//asm volatile("ud2");  // Execute an undefined instruction (UD2)

	//breakpoint exception
	//asm volatile("int $3");
	
	//div 0 exception
	//int a = 1/0; 
	
	// Test for page fault
	// int* c = 0; 
	// int b = *c; 

}
