#include "i8259.h"
#include "lib.h"
#include "idt_handlers.h"
#include "cursor.h"
#include "file_system.h"
#include "paging_asm.h"
#include "System_Calls.h"
#include "x86_desc.h"
#include "keyboard.h"


/* Function to halt the currently executing process
 * 
 * Restores the parent process's data and paging, clears file descriptors,
 * and jumps to the parent process's execution context.
 * Inputs: status - The status code to return to the parent process
 * Outputs: Returns 0 on success (or 256 if status is 255)
 * Side Effects: Restores parent process data and paging, clears file descriptors
 * Return: 0 on success (or 256 if status is 255)
 */
int32_t halt(uint8_t status){

    // Restore parent data (The specific data being referred to is not documented here, but it's about restoring some context data from the parent process.)
   uint32_t RETURN_VALUE;
   RETURN_VALUE = (uint32_t)status; // we sign extend the status for RETURN_VALUE
   int i;

   if(status == 255){ // the provided value for exception within halt
       RETURN_VALUE = 256;
   }

   uint8_t* ESP = pcb_array[target_pid]->prev_esp;
   uint8_t* EBP = pcb_array[target_pid]->prev_ebp;

   asm volatile (" \
   movl %%cr3, %%eax ; \
   movl %%eax, %%cr3 ; \
   " : /* no outputs */ \
   : /* no inputs */ \
   : "eax", "cc");

   // Restore parent paging (from ls back to shell)
   program_pages(target_pid-1);

   // Clear file descriptor array (Resetting the file descriptor entries for the current process)
   for(i = 0; i < 8; i++){ // 8 because we are iterating over all 8 indeces of the fd array 
       if(pcb_array[target_pid]->fd_array[i] == NULL){
           break;
       }
       close_syscall(i); // Close the file descriptors for the current process
   }

   // Write Parent process’ info back to TSS(esp0)
   tss.esp0 = 0x800000 - 4; // THIS IS 8MB - 4 which is the pointer to the start of the shell program
   tss.ss0 = KERNEL_DS;
   my_pids[target_pid] = 0;
   
    if(pcb_array[target_pid]->pid == 0){ 
        execute((const uint8_t*)"shell"); 
        }

    target_pid--;


   // Jump to execute return (Switch back to the parent process)
   asm volatile("movl %0, %%esp \n"
              "movl %1, %%ebp \n"
              "movl %2, %%eax \n"
              "leave \n"
              "ret \n"
            : /* no output */
            : "r" (ESP), "r" (EBP), "r" (RETURN_VALUE)
            : "memory");

    if(status == 255){
        return 256;
    }

    return 17;
}

/* Function to execute a user-level program
 * 
 * Checks if the given command is an executable, creates a PCB for the process,
 * loads the program into memory, and switches to user mode to execute the program.
 * Inputs: command - The command to execute, which should be the name of an executable file
 * Outputs: Returns a value if the program execution is successful
 * Side Effects: Creates a new process with its own PCB, loads the program into memory
 * Return: Returns a value if the program execution is successful, or -1 on failure
 */
int32_t execute(const uint8_t* command) {
    uint8_t name[32];
    dentry_t temp_dentry[1];
    int i = 0;
    int u = 0;
    uint8_t boofer[32];
    pcb_t* pcb;
    uint8_t buffy[1];
    uint32_t start_adrr;
    uint8_t args[128]; // Add an array to store command-line arguments

    // Check if the given command is an executable
    for (i = 0; i < 32; i++) { // 32 because this is each character in the array that we are clearing
        name[i] = 0;
    }
    i = 0;

    // Create a filename array for the first word in the space-separated command
    while (command[i] != ' ' && command[i] != '\0'){
        name[i] = command[i];
        i++;
    }
    u = i; 




    // If the file doesn't exist, return an error
    if (read_dentry_by_name(name, temp_dentry) == -1){
        return -1;
    }

    // Check if the file has the expected magic numbers signifying an executable
    read_data(temp_dentry->inode_num, 0, boofer, 4);
    if (boofer[0] != 0x7f || boofer[1] != 0x45 || boofer[2] != 0x4c || boofer[3] != 0x46)
        return -1;

    // Create a PCB and set the target PID
    for (i = 0; i < 6; i++) { // 6 because there are a maximum of 6 programs we can run in our system
        if (my_pids[i] == 0)
            break;
    }

    if (i == 6) // if we are the max number of programs return -1
        return -1;

    my_pids[i] = 1;
    target_pid = i;

    // Get a PCB structure and set its properties
    pcb = create_pcb(target_pid);
    pcb->pid = target_pid;
    pcb->inode_num = temp_dentry->inode_num;



///////////////////////////////////////////GETARGS///////////////////////////////////////////////////////////////////////////////////////////
    // Extract and store command-line arguments in the args buffer
    if (command[u] == ' ') {
        u++; // Move past the space

        // Copy the arguments into the args buffer
        int arg_idx = 0;
        while (command[u] != '\0') {
            args[arg_idx] = command[u];
            u++;
            arg_idx++;
        }
    }

    strncpy((int8_t*)pcb_array[target_pid]->args, (const int8_t*)args, 128);
///////////////////////////////////////////GETARGS///////////////////////////////////////////////////////////////////////////////////////////


    // Reload the page directory and load the program into memory
    asm volatile (" \
    movl %%cr3, %%eax ; \
    movl %%eax, %%cr3 ; \
    " : /* no outputs */ \
      : /* no inputs */ \
      : "eax", "cc");

    program_pages(target_pid);
    uint8_t* pointer = (uint8_t*)0x8048000; // This is the address to the start of the program image
    read_data(temp_dentry->inode_num, 0, pointer, 0x8400000 - 0x8048000); // the size is 132MB - start of our prog img

    read_data(temp_dentry->inode_num, 24, buffy, 4);
    start_adrr = *(uint32_t*)(buffy);

    // Set ESP0 and SS0 for the TSS
    tss.esp0 = 0x800000 - (0x2000 * (target_pid + 1)) - 4; // 8MB - (8KB * current_pid) - 4 bytes
    tss.ss0 = KERNEL_DS;

    // Save the previous ESP and EBP values
    register uint32_t ebp_value asm("ebp");
    register uint32_t esp_value asm("esp");

    pcb_array[target_pid]->prev_esp = (uint8_t*)esp_value;
    pcb_array[target_pid]->prev_ebp = (uint8_t*)ebp_value;

    // Perform an iret to switch to the user program
    asm volatile("pushl %0 \n"
                  "pushl %1 \n"
                  "pushfl   \n"
                  "pushl %2 \n"
                  "pushl %3 \n"
                  "iret     \n"
                : /* no output */
                : "r" (USER_DS), "r" (START_OF_USER_STACK - sizeof(uint32_t)), "r" (USER_CS), "r" (start_adrr)
                : "memory");

    return 0;
}



/* Function to read from a file descriptor (system call)
 * 
 * Checks the validity of the file descriptor and reads from it using the appropriate function.
 * Inputs: fd - File descriptor index
 *         buf - A buffer to store the read data
 *         nbytes - The number of bytes to read
 * Outputs: The number of bytes successfully read, or -1 on failure
 * Return: The number of bytes successfully read, or -1 on failure
 */
int32_t read_syscall(int32_t fd, void* buf, int32_t nbytes) {
    // Check if the file descriptor is valid and the associated functions exist
    if (fd < 0 || fd > 7 || buf == NULL || nbytes < 0) // 0 is a test for negative and 7 is the max number of fds
        return -1;

    // Check if the current process and file descriptor are valid
    if (pcb_array[target_pid] == NULL || pcb_array[target_pid]->fd_array[fd] == NULL || pcb_array[target_pid]->fd_array[fd]->inode == -1)
        return -1;

    // Check if the read function for the file descriptor exists, and if so, call it
    if (pcb_array[target_pid]->fd_array[fd]->jmp_table[0] == NULL)
        return -1;

    return pcb_array[target_pid]->fd_array[fd]->jmp_table[0](fd, buf, nbytes);

}

/* Function to write to a file descriptor (system call)
 * 
 * Checks the validity of the file descriptor and writes to it using the appropriate function.
 * Inputs: fd - File descriptor index
 *         buf - A buffer containing data to write
 *         nbytes - The number of bytes to write
 * Outputs: The number of bytes successfully written, or -1 on failure
 * Return: The number of bytes successfully written, or -1 on failure
 */
int32_t write_syscall(int32_t fd, const void* buf, int32_t nbytes) {
    // Check if the file descriptor is valid and the associated functions exist
    if (fd < 0 || fd > 7 || buf == NULL || nbytes < 0)
        return -1;

    // Check if the current process and file descriptor are valid
    if (pcb_array[target_pid] == NULL || pcb_array[target_pid]->fd_array[fd] == NULL || pcb_array[target_pid]->fd_array[fd]->inode == -1)
        return -1;

    // Check if the write function for the file descriptor exists, and if so, call it
    if (pcb_array[target_pid]->fd_array[fd]->jmp_table[1] == NULL)
        return -1;

    return pcb_array[target_pid]->fd_array[fd]->jmp_table[1](fd, buf, nbytes);
}




/* Function to open a file or directory (system call)
 * 
 * This function checks the validity of the filename, finds an available file descriptor, determines the file type,
 * calls the appropriate open function, and adds the file to the file descriptor array.
 * Inputs:
 * - filename: The name of the file or directory to open
 * Outputs:
 * - The file descriptor index or -1 on failure
 * Return: The file descriptor index or -1 on failure
 */
uint32_t open_syscall(uint8_t* filename) {
    // Check the validity of the filename
    if (filename == NULL || filename[0] == '\0'){
        return -1; 
    }

    int i = 0;
    fd_full = 0;  
    // Find an available file descriptor
    while (pcb_array[target_pid]->fd_array[i] != NULL){
        i++;
        if (i == 8){ // 8 is the maximum number of 
            fd_full = 1;
            return -1; // Open failed, no space in the array
        }
    }
    fd_index = i;

    dentry_t temp_dentry; 
    // Determine the file type by reading the directory entry
    read_dentry_by_name(filename, &temp_dentry); 
    int ret_val; 
    // Check the file type and call the appropriate open function
    if (temp_dentry.filetype < 0 || temp_dentry.filetype > 2){
        return -1; 
    }
    if (temp_dentry.filetype == 0){
        // If the file type is 0, it's a real-time clock (RTC) file
        ret_val = rtc_open(NULL); 
    }
    if (temp_dentry.filetype == 1){
        // If the file type is 1, it's a directory
        ret_val = open_dir(filename); 
    }
    if (temp_dentry.filetype == 2){
        // If the file type is 2, it's a regular file
        ret_val = open_file(filename); 
    }
    if (ret_val == -1)
        return -1; 

    // Add the file to the file descriptor array
    add_file_descriptor_table_entry(i, filename, pcb_array[target_pid]->fd_array);

    return fd_index; 
}

/* Function to close a file descriptor (system call)
 * 
 * This function closes the file descriptor, clears its associated data, and sets it to null in the file descriptor array.
 * Inputs:
 * - fd: File descriptor index
 * Outputs:
 * - Clears and closes the file descriptor
 * Return: 0 on success, or -1 on failure
 */
int32_t close_syscall(int32_t fd) {
    global_offset = 0;
    index_offset = 0;

    // Check the validity of the file descriptor
    if (fd < 2 || fd > 7){ // we want to return negative 1 if we close an fd out of bounds 
        return -1;
    }
    if (pcb_array[target_pid] == NULL){
        return -1; 
    }
    if (pcb_array[target_pid]->fd_array[fd] == NULL){
        return -1;
    }
    if (pcb_array[target_pid]->fd_array[fd]->inode == -1){
        return -1;
    }

    // Clear the file descriptor and associated data
    pcb_array[target_pid]->fd_array[fd]->inode = -1;
    pcb_array[target_pid]->fd_array[fd]->file_position = -1;
    pcb_array[target_pid]->fd_array[fd]->flags = 0;
    pcb_array[target_pid]->fd_array[fd]->jmp_table[0] = NULL;
    pcb_array[target_pid]->fd_array[fd]->jmp_table[1] = NULL;
    pcb_array[target_pid]->fd_array[fd]->jmp_table[2] = NULL;
    pcb_array[target_pid]->fd_array[fd]->jmp_table[3] = NULL;
    return 0;
}

/* Function to get a new process ID (PID)
 * 
 * This function finds an available PID for a new process.
 * Outputs: A new process ID (PID) or -1 if none is available
 * Return: A new process ID (PID) or -1 if none is available
 */
uint8_t get_new_pid() {
    int i;
    // Find an available PID for a new process
    for(i = 0; i < 6; i++){ // 6 is the 
        if(my_pids[i] == 0){
            return i;
        } 
    }
    return -1;
}

/* Function to create a PCB (Process Control Block)
 * 
 * This function initializes a PCB structure for a new process with the given PID.
 * Inputs:
 * - pid: Process ID (PID)
 * Outputs:
 * - Initializes the PCB structure
 * Return: A pointer to the created PCB
 */
pcb_t* create_pcb(uint32_t pid) {
    // Calculate the PCB's memory location based on the PID
    pcb_t* temp_pcb = (pcb_t*)(0x800000 - (pid + 1) * 0x2000); // 8MC - current_pid * 8 KB
    temp_pcb->pid = pid; 
    temp_pcb->state = 1; 

    pcb_array[pid] = temp_pcb; 

    // Initialize the PCB's file descriptor array
    init_file_descriptor(pcb_array[pid]->fd_array);

    return pcb_array[pid]; 
}


/* Function to get arguments for a process (system call)
 * 
 * Retrieves arguments for the current process.
 * Inputs: buf - A buffer to store the arguments
 *         nbytes - The number of bytes to read
 * Return: -1 (Not implemented)
 */
int32_t getargs(uint8_t* buf,  int32_t nbytes){
    if (buf == NULL || nbytes <= 0) {
        return -1; 
    }
    
    int32_t arg_length = strlen((const int8_t*)pcb_array[target_pid]->args) + 1;
    
     if (nbytes < arg_length) {
        return -1; 
    }

    memcpy(buf, pcb_array[target_pid]->args, arg_length);
    
    return 0;
}

/* Function to map video memory (system call)
 * 
 * Maps video memory to a user-level virtual address.
 * Inputs: screen_start - A pointer to the screen start address
 * Return: -1 (Not implemented)
 */
int32_t vidmap(uint8_t** screen_start) {



    
    create_new_pte_for_video_memory(screen_start);
    return 0;
}

/* Function to set a signal handler (system call)
 * 
 * Sets a handler function for a specific signal.
 * Inputs: signum - The signal number
 *         handler_address - The address of the handler function
 * Return: -1 (Not implemented)
 */
int32_t set_handler(int32_t signum, void* handler_address) {
    return -1;
}

/* Function to return from a signal (system call)
 * 
 * Returns from a signal handler.
 * Return: -1 (Not implemented)
 */
int32_t sigreturn(void) {
    return -1;
}
