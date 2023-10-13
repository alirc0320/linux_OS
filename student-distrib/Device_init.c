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

}