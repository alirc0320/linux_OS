//This file is where we will initialize device and RTC

// #include <stdio.h>
// #include <stdlib.h>
#include "Device_init.h"
#include "i8259.h"
#include "lib.h"

// Commands
#define RESET               0xFF            // Command to reset
#define ENABLE_SCANNING     0xF4            // Command to enable scanning

// Responses
#define ACK                 0xFA            // Response: Acknowledgment
#define RESEND              0xFE            // Response: Re-send request
#define TEST_PASSED         0xAA            // Response: Test passed
#define TEST_FAILED         0xFC            // Response: Test failed
#define TEST_FAILED_2       0xFD            // Response: Test failed (alternative)

//ports-apparently these are convention for PS2 keyboard
#define COMMAND_PORT 0x64   // COMMAND_PORT: Port address for command communication with the keyboard controller
#define DATA_PORT 0x60      // DATA_PORT: Port address for data communication with the keyboard controller


/* Initialize the Real-Time Clock (RTC)
 *
 * This function initializes the Real-Time Clock (RTC) for system timekeeping.
 * It configures the RTC's Register B to enable RTC interrupts on IRQ 8, which
 * allows the system to receive periodic timekeeping updates.
 *
 * Inputs: None
 * Outputs: None
 */
void init_keyboard(){
enable_irq(1); //enable interrupt 1
}


/* Real-Time Clock (RTC) Initialization Function
 * Inputs: None
 * Outputs: None
 * Function: Initializes the RTC by configuring its control registers.
 *           It sets up the RTC to generate interrupts on IRQ 8 (RTC interrupt).
 *           This function should be called during system initialization. */
void init_RTC() {
    //cli();
    outb(0x8B,0x70); // Select Register B and disable NMI forsureskis. 0x8B =  Register B && 0x70 = used to specifiy index and disable NMI
    char prev = inb(0x71); // Read the current value of Register B forsureskis. 0x71 = Used to read or write to from that byte of CMOS configuration spcae (PORT)
    outb(0x8B, 0x70); // Set the index again forsureskis. 0x70 = used to specifiy index and disable NMI. 0x8B is register B 
    outb(prev | 0x40, 0x71); // Enable bit 6 of Register B to turn on IRQ 8 forsureskis
    

    outb(0x8A, 0x70);		// set index to register A, disable NMI. Reg A is 8A and 70 is the index
    prev=inb(0x71);	// get initial value of register A. 71 is used to read or write to and form 
    outb(0x8A, 0x70);		// reset index to A. Index is 70 and 8A is regsiter 

    enable_irq(8);  //8th Interrupt 
    // //sti();
}
