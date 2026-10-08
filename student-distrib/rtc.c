#include "rtc.h"
#include "i8259.h"
#include "lib.h"
#include "idt_handlers.h"
#include "cursor.h"
#include "file_system.h"
#include "System_Calls.h"

#define RTC_BASE_FREQ   1024            // the hardware always runs at 1024 Hz
#define RTC_BASE_RATE   6               // 32768 >> (6 - 1) = 1024 Hz
#define RTC_DEFAULT_FREQ 2              // frequency after open

// counts hardware interrupts; each open rtc file waits for its own number of ticks
static volatile uint32_t rtc_ticks = 0;

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
    outb((prev & 0xF0) | RTC_BASE_RATE, 0x71);  // run the hardware at 1024 Hz, rtc_read virtualizes slower rates
    enable_irq(8);  //8th Interrupt 

    // //sti();
}



/* Real-Time Clock (RTC) Interrupt Handler Function
 * Inputs: None
 * Outputs: None
 * Function: Handles RTC interrupt. If the 'flag' is set, it calls 'test_interrupts' function.
 *           Reads register C of the RTC to clear the interrupt and acknowledges the interrupt. */
void rtc_interrupt() {
    //flag = 1; 
   if (flag) {
       // test_interrupts();  // Call 'test_interrupts' function if the 'flag' is set.
        printf("1");
    }
    outb(0x0C, 0x70);  // Select register C of the RTC. x70 is the index
    inb(0x71);         // Read register C to clear the interrupt. this is the read or write port is 0x71
    read_flag = 1; 
    rtc_ticks++;
    send_eoi(8);        // Send an End-of-Interrupt signal to acknowledge the interrupt. interrupt numbr 8 is the rtc
}

/* RTC Write Function
 *
 * Sets the interrupt rate this file descriptor sees. The hardware stays at
 * 1024 Hz; the rate is stored per open file so programs on different
 * terminals do not change each other's speed.
 *
 * Inputs:
 *   - fd: the rtc file descriptor
 *   - buf: pointer to a 4 byte frequency, a power of 2 between 2 and 1024 Hz
 *   - nbytes: must be 4
 *
 * Outputs:
 *   - Returns 4 if the frequency is set successfully, otherwise -1
 */
 int rtc_write(int32_t fd, void* buf, int32_t nbytes){
    int32_t freq;
    if (buf == NULL || nbytes != 4){
        return -1;
    }
    freq = *(int32_t*)buf;
    if (freq < 2 || freq > RTC_BASE_FREQ || (freq & (freq - 1)) != 0){ // power of 2 between 2 and 1024 Hz
        return -1;
    }
    pcb_array[target_pid]->fd_array[fd].file_position = freq;  // rtc files have no position, store the rate there
    return 4;
 }


/* Open the Real-Time Clock (RTC)
 *
 * Opening the RTC gives the file the default rate of 2 Hz (an unset rate
 * is read as 2 Hz by rtc_read).
 *
 * Inputs: filename - unused
 * Outputs: 0 if the RTC is successfully opened
 */
int rtc_open(uint8_t* filename){
    return 0; 
}


/* Read from the Real-Time Clock (RTC)
 *
 * Blocks until the next virtual interrupt for this file's rate.
 *
 * Inputs: fd - the rtc file descriptor
 * Outputs: Always returns 0
 */
int rtc_read(int32_t fd, void* buf, int32_t nbytes) {
    int32_t freq = pcb_array[target_pid]->fd_array[fd].file_position;
    uint32_t start;
    if (freq < RTC_DEFAULT_FREQ){
        freq = RTC_DEFAULT_FREQ;
    }

    // syscalls run with interrupts off, let the RTC and scheduler run while we wait
    sti();
    start = rtc_ticks;
    while (rtc_ticks - start < (uint32_t)(RTC_BASE_FREQ / freq));
    cli();
    return 0;
}
//return 0


/* Close the Real-Time Clock (RTC)
 *
 * This function is responsible for closing the RTC. In this minimal
 * implementation, it always returns 0 as closing the RTC is not implemented.
 *
 * Inputs: None
 * Outputs: Always returns 0 in this implementation
 */

int rtc_close(int32_t fd){
    return 0;
}

int32_t (*rtc_jump_table[4]) () = {rtc_read, rtc_write, rtc_open, rtc_close};
