#include "keyboard.h"
#include "i8259.h"
#include "lib.h"
#include "idt_handlers.h"
#include "cursor.h"
 
int ctrl_flag = 0; 
int shift_flag = 0; 
int capslock_flag = 0;
int alt_flag = 0; 
int kb_flag = 0; 
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



/* Array that maps keyboard scan codes to characters and control codes.
 * Each element in the array represents the character or control code associated with a specific scan code.
 * Note: Some entries are marked as '\0', which means they have no corresponding character or control code.
 * The array provides a lookup table for translating scan codes into readable characters. */
//0x59 is limit on array whihch is 89 in decimal 


unsigned char keyScanCodeArray_one[0x59] = {'\0','\0','1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', 
'=', '\0', '\0', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', '\0', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', '\0', '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 
'\0', '*', '\0', ' ', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '7', '8', 
'9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', '\0', '\0', '\0', '\0', '\0'};   

unsigned char keyScanCodeArray_for_shift[0x59] = {'\0','\0','!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', 
'+', '\0', '\0', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', '\0', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', '\0', '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 
'\0', '*', '\0', ' ', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '7', '8', 
'9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', '\0', '\0', '\0', '\0', '\0'};   

unsigned char keyScanCodeArray_for_just_capslock[0x59] = {'\0','\0','1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', 
'=', '\0', '\0', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n', '\0', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`','\0', '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 
'\0', '*', '\0', ' ', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '7', '8', 
'9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', '\0', '\0', '\0', '\0', '\0'};   



unsigned char keyScanCodeArray_for_shift_and_capslock[0x59] = {'\0','\0','!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', 
'+', '\0', '\0', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '{', '}', '\n', '\0', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':', '"', '~', '\0', '|', 'z', 'x', 'c', 'v', 'b', 'n', 'm', '<', '>', '?', 
'\0', '*', '\0', ' ', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '7', '8', 
'9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', '\0', '\0', '\0', '\0', '\0'};  



/* Keyboard Interrupt Handler Function
 * Inputs: None
 * Outputs: None
 * Function: Handles keyboard interrupt, reads the scan code from the keyboard controller, 
 *           maps it to a character using keyScanCodeArray_one, and prints the corresponding character. */
int keyboard_interrupt() {
    scan_code = inb(0x60);  // Read the scan code from the keyboard controller. x60 is the keyboard data port 
    write(keyboard_buffer, 0); 
    read(keyboard_buffer, 0);
    send_eoi(1);

    return scan_code;
}

 void write (const char* buf, int nbytes){//need to add fd as a function once file system worknig
    if (scan_code == 0x3A){//Caps lock
        capslock_flag += 1; 
        capslock_flag %= 2; 
        return; 
    }
    if(scan_code == 0x1D){
        ctrl_flag = 1; 
        return;
    }
    if (scan_code == 0x9D){
        ctrl_flag = 0; 
        return; 
    }
    if (ctrl_flag==1 && scan_code == 0x26){//this is for ctrl+L, we'll need to make arrays for ctrl and shifted characters
        clear(); 
        init_buffer(); 
        return; 
    }

    if (ctrl_flag)
        return; 
    
    if (scan_code == 0x0E) { // Backspace
    if (keyboard_buffer_index == 0)
        return; 
    int i;
    int backspace_count = 0;

    for (i = 0; i < 4; i++) {
        if (keyboard_buffer_index > 0 && keyboard_buffer[keyboard_buffer_index - 1] == ' ') {
            backspace();
            keyboard_buffer_index--;
            backspace_count++;
        } else {
            break;
        }
    }

    // If no tab spaces were removed, perform a regular backspace
    if (backspace_count == 0) {
        backspace();
        if (keyboard_buffer_index > 0) {
            keyboard_buffer[keyboard_buffer_index - 1] = '\0';
            keyboard_buffer_index--;
        }
    }

    return;
}
//ali
if (scan_code == 0x0F) { // Tab
    int i;
    for (i = 0; i < 4; i++) {
        // Update the buffer with four spaces
        if (keyboard_buffer_index < 127) {
            keyboard_buffer[keyboard_buffer_index] = ' ';
            putc(' '); // Print a space character
            keyboard_buffer_index++;
        }
    }
    return;
}





    ///////SHIFT////////////
    if(scan_code == 0xAA || scan_code == 0xB6){//shift released
       shift_flag = 0; 
    }
    if(scan_code == 0x2A || scan_code == 0x36){//shift pressed
        shift_flag = 1;
    }

    //////Alt
    if(scan_code == 0xB8){//released
       alt_flag = 0; 
    }
    if(scan_code == 0x38){//pressed
        alt_flag = 1;
    }
    //used to test alt flag works when alt + l pressed bc idrk what alt does yet
    // if (alt_flag == 1 && scan_code == 0x26){
    //     printf("alt flag works");
    // }


    else if (scan_code < 89 && scan_code != 0x1C) { //array index is less than 89
    if(keyboard_buffer_index >= 127 || keyScanCodeArray_one[scan_code] == '\0'){
        return; 
    }
        if(shift_flag == 1 && capslock_flag == 0){
            char output = keyScanCodeArray_for_shift[scan_code];  // Map the scan code to a character.
            keyboard_buffer[keyboard_buffer_index] = output; 
            keyboard_buffer_index ++;
            putc(output);

        }
        else if (shift_flag == 1 && capslock_flag == 1){
            char output = keyScanCodeArray_for_shift_and_capslock[scan_code];  // Map the scan code to a character.
            keyboard_buffer[keyboard_buffer_index] = output; 
            keyboard_buffer_index ++;
            putc(output);
        }
         else if (shift_flag == 0 && capslock_flag == 1){
            char output = keyScanCodeArray_for_just_capslock[scan_code];  // Map the scan code to a character.
            keyboard_buffer[keyboard_buffer_index] = output; 
            keyboard_buffer_index ++;
            putc(output);
        }
        else{
        char output = keyScanCodeArray_one[scan_code];  // Map the scan code to a character.
        keyboard_buffer[keyboard_buffer_index] = output; 
        keyboard_buffer_index ++; 
        putc(output);  // SHOULD BE PUTC
        }
}
   
}

 int read(const char* buf, int nbytes){//need to add fd as a function once file system worknig

     if (scan_code != 0x1C){
        return 0; 
     }
     //enter case
    else if(scan_code == 0x1C){
       
        putc('\n');
        int i; 
        for (i = 0; i<keyboard_buffer_index; i++){
            putc(keyboard_buffer[i]);
        }
        putc('\n');
        init_buffer(); 
        return 0; 
     }
     return 0;

}

void init_buffer(){
    int i; 
    for (i = 0; i< 128; i++){
        keyboard_buffer[i] = '\0';
    }
    keyboard_buffer_index = 0; 
}

int terminal_open(){
    return 0;
}

int terminal_close(){
    return 0;
}
