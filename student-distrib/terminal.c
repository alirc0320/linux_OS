#include "keyboard.h"
#include "i8259.h"
#include "lib.h"
#include "idt_handlers.h"
#include "cursor.h"
#include "file_system.h"
#include "x86_desc.h"
#include "terminal.h"
#include "paging_asm.h"
#include "pit.h"

#define VIDEO_ADDR          0xB8000                                 //start of video memory
#define BACKUP_ADDR(tid)    (VIDEO_ADDR + (((tid) + 1) << 12))      //backup pages at 0xB9000, 0xBA000, 0xBB000
#define VIDEO_PAGE_SIZE     4096
#define BOOTSTRAP_WORDS     2048                                    //8KB bootstrap stack per terminal

uint32_t current_terminal_id = 0;
uint32_t sched_terminal = 0;

//stacks used only to run execute("shell") the first time a terminal is scheduled
static uint32_t bootstrap_stacks[NUM_TERMINALS][BOOTSTRAP_WORDS];

/*
 * Start a terminal's base shell
 *
 * Inputs: None
 * Outputs: None
 * Function: First code a terminal runs when the scheduler switches to it. The
 *           scheduler already swapped in this terminal's (empty) pcb array.
 */
static void start_terminal_shell(void) {
    execute((const uint8_t *)"shell");
    while (1);  //execute never returns
}

/*
 * Initialize terminal
 *
 * Inputs: None
 * Outputs: None
 * Function: Initializes terminal-related variables and structures, builds a
 *           bootstrap stack for terminals 1 and 2 and runs the shell on terminal 0.
 */
void terminal_init() {
    int i, t;
    uint32_t* sp;

    for (i = 0; i < MAX_PROCESSES; i++) {
        pcb_array[i] = 0;
        my_pids[i] = 0;
        for (t = 0; t < NUM_TERMINALS; t++) {
            terminal_array[t].pcb_array[i] = 0;
        }
    }

    for (t = 0; t < NUM_TERMINALS; t++) {
        terminal_array[t].curr_x = 0;
        terminal_array[t].curr_y = 0;
        terminal_array[t].buffer_character_count = 0;
        terminal_array[t].terminal_id = t;
        terminal_array[t].buffer_index = 0;
        terminal_array[t].pid = 0;
        terminal_array[t].turned_on = 0;
        terminal_array[t].enter_flag = 0;
        for (i = 0; i < 128; i++) {
            terminal_array[t].keyboard_buffer[i] = '\0';
        }

        //fake a context_switch frame: edi, esi, ebx, ebp, then "return" into start_terminal_shell
        sp = &bootstrap_stacks[t][BOOTSTRAP_WORDS];
        *(--sp) = 0;                                //return address for start_terminal_shell (never used)
        *(--sp) = (uint32_t)start_terminal_shell;
        *(--sp) = 0;                                //ebp
        *(--sp) = 0;                                //ebx
        *(--sp) = 0;                                //esi
        *(--sp) = 0;                                //edi
        terminal_array[t].esp = (uint32_t)sp;
    }

    current_terminal_id = 0;
    sched_terminal = 0;
    target_pid = 0;
    init_buffer();

    //terminal 0 runs right away on the boot stack, the PIT starts the other two
    terminal_array[0].turned_on = 1;
    execute((const uint8_t *)"shell");
}

/*
 * Switch terminal
 *
 * Inputs: terminal_id - Index of the terminal to show
 * Outputs: None
 * Function: Changes which terminal is on screen and receives keyboard input.
 *           Processes keep running; the scheduler decides who owns the CPU.
 */
void terminal_switch(uint32_t terminal_id) {
    uint32_t flags;
    cli_and_save(flags);

    if (terminal_id >= NUM_TERMINALS || terminal_id == current_terminal_id) {
        restore_flags(flags);
        return;
    }

    //save the visible screen into the old terminal's backup page, show the new one
    memcpy((uint8_t *)BACKUP_ADDR(current_terminal_id), (uint8_t *)VIDEO_ADDR, VIDEO_PAGE_SIZE);
    memcpy((uint8_t *)VIDEO_ADDR, (uint8_t *)BACKUP_ADDR(terminal_id), VIDEO_PAGE_SIZE);

    //the global keyboard buffer always belongs to the visible terminal
    memcpy(terminal_array[current_terminal_id].keyboard_buffer, keyboard_buffer, 128);
    terminal_array[current_terminal_id].buffer_index = keyboard_buffer_index;
    memcpy(keyboard_buffer, terminal_array[terminal_id].keyboard_buffer, 128);
    keyboard_buffer_index = terminal_array[terminal_id].buffer_index;

    current_terminal_id = terminal_id;
    update_cursor(terminal_array[terminal_id].curr_x, terminal_array[terminal_id].curr_y);

    //the running process may now need to draw to (or stop drawing to) the real screen
    update_vidmap();

    restore_flags(flags);
}
