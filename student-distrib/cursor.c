#include "cursor.h"
#include "lib.h"




void init_cursor() {
    // Initialize the cursor start and end to zero to hide the cursor initially
    outb(0x0A, 0x3D4);  // Set cursor start line
    outb(0, 0x3D5);

    outb(0x0B, 0x3D4);  // Set cursor end line
    outb(25, 0x3D5);

    // Set the cursor location to top-left (row 0, column 0)
    outb(0x0F, 0x3D4);  // High byte of cursor location
    outb(0, 0x3D5); //vert, row

    outb(0x0E, 0x3D4);  // Low byte of cursor location
    outb(0, 0x3D5); //horiz, col
    
}


void disable_cursor() {
    // Disable the cursor by setting both start and end scanlines to 0
    outb(0x0A, 0x3D4);  // Set cursor start line-this is a boundary
    outb(0x20, 0x3D5);
}

void update_cursor(int x, int y) {
    // Set the cursor location
   unsigned short position = y * VGA_WIDTH + x;
    outb(0x0F, 0x3D4);  //low
    outb((unsigned short) (position & 0xFF), 0x3D5);
    outb(0x0E, 0x3D4);  //high
    outb((unsigned short) ((position >> 8) & 0xFF), 0x3D5);
}


