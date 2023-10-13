//This file is where we will initialize device and RTC

#include <stdio.h>
#include <stdlib.h>
#include "Device_init.h"

//commands
#define RESET	0xFF
#define ENABLE_SCANNING 0xF4

//responses
#define ACK 0xFA
#define RESEND 0xFE
#define TEST_PASSED 0xAA
#define TEST_FAILED 0xFC
#define TEST_FAILED_2 0xFD

//ports-apparently these are convention for PS2 keyboard
#define COMMAND_PORT 0x64
#define DATA_PORT 0x60 


void init_keyboard(){
//reset, command FF
outb(RESET, COMMAND_PORT);

//wait for TEST PAssed Signal
while (inb(DATA_PORT) != TEST_PASSED){
    //if there is a resend command, send the command to command port again
    if (inb(DATA_PORT) == RESEND){
        outb(RESET, COMMAND_PORT);
    }

    //reset creates a self test so If it fails you shouldnt proceed. IDK what to put here exactly tho
    if (inb(DATA_PORT) == TEST_FAILED || inb(DATA_PORT) == TEST_FAILED){
        printf("self test failed")
        return; 
    }
}

//enable, command F4
outb(ENABLE_SCANNING, COMMAND_PORT);

//wait for ACK
while (inb(DATA_PORT) != ACK){
    //if there is a resend command, send the command to command port again
    if (inb(DATA_PORT) == RESEND){
        outb(ENABLE_SCANNING, COMMAND_PORT);
    }
}
///////Should we also: Set scan code, typematic byte and enable LEDs?????///////////////////////////
}



void init_RTC(){

/////////////////Not sure if this is right but it was in OS dev documentation, prob need to add more//////////////
disable_interrupts();		// important that no interrupts happen (perform a CLI)
outportb(0x70, 0x8A);	// select Status Register A, and disable NMI (by setting the 0x80 bit)
outportb(0x71, 0x20);	// write to CMOS/RTC RAM
enable_interrupts();		// (perform an STI) and reenable NMI if you wish

}



void disable_interrupts() {
    __asm__ __volatile__ ("cli"); // Disable interrupts
}

void enable_interrupts() {
    __asm__ __volatile__ ("sti"); // Enable interrupts
}