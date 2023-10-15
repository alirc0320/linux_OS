//This file is where we will initialize device and RTC

// #include <stdio.h>
// #include <stdlib.h>
#include "Device_init.h"
#include "lib.h"

//commands
#define RESET	0xFF
#define ENABLE_SCANNING 0xF4

//responses
#define ACK 0xFA
#define RESEND 0xFE
#define TEST_PASSED 0xAA
#define TEST_FAILED 0xFC
#define TEST_FAILED_2 0xFD
// 0X70
// 0X71
// IRQ NUM 8
//  0X8A
// 0X8B
// 0X8C
// 1024
// 6

//ports-apparently these are convention for PS2 keyboard
#define COMMAND_PORT 0x64
#define DATA_PORT 0x60 


void init_keyboard(){
//outb(0, 0x64);
enable_irq(1);
}


void init_RTC() {
    //cli();
    outb(0x8B,0x70); // Select Register B and disable NMI forsureskis
    char prev = inb(0x71); // Read the current value of Register B forsureskis
    outb(0x8B, 0x70); // Set the index again forsureskis
    outb(prev | 0x40, 0x71); // Enable bit 6 of Register B to turn on IRQ 8 forsureskis
    

    outb(0x8A, 0x70);		// set index to register A, disable NMI
    prev=inb(0x71);	// get initial value of register A
    outb(0x8A, 0x70);		// reset index to A
  
   
   

    enable_irq(8);
    // //sti();
}

// 