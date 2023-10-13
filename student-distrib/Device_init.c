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
enable_irq(1);
}


void init_RTC() {
    cli(); // Disable interrupts
    // Initialize RTC
    outb(0x70, 0x8B); // Select Register B and disable NMI
    char prev = inb(0x71); // Read the current value of Register B
    outb(0x70, 0x8B); // Set the index again
    outb(0x71, prev | 0x40); // Enable bit 6 of Register B to turn on IRQ 8
    // Enable the RTC interrupt handler
    enable_irq(8);
    sti(); // Enable interrupts
}

