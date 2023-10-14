/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"


#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)


#define ICW1_ICW4	0x01		/* Indicates that ICW4 will be present */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */
 
#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

#define EOI 0x60 



/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask; /* IRQs 0-7  */
uint8_t slave_mask;  /* IRQs 8-15 */

/* Initialize the 8259 PIC */
void i8259_init(void) {
//unsigned char a1, a2;
slave_mask = 0xFF; //added 
master_mask = 0xFF; //added 
 
	// a1 = inb(PIC1_DATA);                        // save masks
	// a2 = inb(PIC2_DATA);

    //masking interrupts
	outb( master_mask, PIC1_DATA);                 // ICW2: Master PIC vector offset
	outb(slave_mask, PIC2_DATA);                 // ICW2: Slave PIC vector offset



	outb(ICW1_INIT | ICW1_ICW4, PIC1_COMMAND);  // starts the initialization sequence (in cascade mode)
	outb(PIC1_COMMAND, PIC1_DATA); 
    outb(0x04, PIC1_DATA);  
	outb(ICW4_8086, PIC1_DATA);  


    
    outb(ICW1_INIT | ICW1_ICW4, PIC2_COMMAND);
	outb (0x28, PIC2_DATA);
    outb(0x02, PIC2_DATA);     
    outb(ICW4_8086, PIC2_DATA);

}

/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) {
     uint16_t port;
    int var;
    if(irq_num < 8) {
        port = PIC1_DATA;
        var = master_mask; 

    } else {
        port = PIC2_DATA;
        irq_num -= 8;
        var = slave_mask; 
    }
    var = var & ~(1 << irq_num);
    outb(var, port);   
}

/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) {
     uint16_t port;
    int var;
    if(irq_num < 8) {
        port = PIC1_DATA;
        var = master_mask; 

    } else {
        port = PIC2_DATA;
        irq_num -= 8;
        var = slave_mask; 
    }
    var = var | (1 << irq_num);
    outb(var, port);   

}

/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num) {
    if(irq_num >= 8){
    outb(EOI | (irq_num-8), PIC2_COMMAND);
    outb(EOI | 2, PIC1_COMMAND);
    return;
    }
    outb(EOI | irq_num, PIC1_COMMAND);
}



