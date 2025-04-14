#include "concurrency.h"
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>

/* 
   Global variables for our process list.
   current_process: pointer to the currently running process.
   head_of_process: pointer to the head of the process list.
*/
process_t *head_of_process = NULL;
process_t *current_process = NULL;

/*
 * process_create
 *
 * Creates a process that starts at function f, with an initial stack size n bytes.
 * Returns 0 on success, -1 on error.
 */
int process_create(void (*f)(void), int n) {
    cli(); // Disable interrupts for critical section

    // Allocate memory for the new process structure using process_malloc
    process_t *new_process = (process_t *) process_malloc(sizeof(process_t));
    if (new_process == NULL) {
        sei(); // Re-enable interrupts
        return -1;
    }
    // Initialize fields; note that the minimal spec requires sp and next.
    new_process->sp = 0;            
    new_process->f = f;             
    new_process->status_flag = 0;   // Not ready until the stack is initialized
    new_process->priority = 0;      // Default; may be augmented later
    new_process->next = NULL;       

    sei(); // End critical section for process structure initialization

    // Initialize the process's stack.
    // The helper process_init (and by extension process_malloc) MUST be called with interrupts disabled.
    cli();
    unsigned int stack_pointer = process_init(f, n);
    sei();
    
    if (stack_pointer == 0) {
        cli();
        free(new_process);
        sei();
        return -1;
    }
    new_process->sp = stack_pointer;
    new_process->status_flag = 1;   // Mark the process as ready
    new_process->priority = 1;      // Assign default priority

    // Insert the new process into the process list (protected by disabling interrupts)
    cli();
    if (head_of_process == NULL) {
        head_of_process = new_process;
        current_process = new_process;
    } else {
        process_t *temp = head_of_process;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_process;
    }
    sei();

    return 0;
}

/*
 * process_start
 *
 * Starts concurrent execution by initializing the process list and invoking process_begin().
 * If process_begin() returns, it indicates that all processes have finished or a deadlock occurred.
 */
void process_start(void) {
    if (head_of_process == NULL) {
        return;
    }
    current_process = head_of_process;
    process_begin();
    while (1);  // Halt here if process_begin() ever returns
}

/*
 * process_select
 *
 * Called as a critical section by the runtime system to select the next ready process.
 * Implements a basic round-robin scheduler.
 * Returns the stack pointer of the next process to run, or 0 if no process is ready.
 */
unsigned int process_select(unsigned int cursp) {
    if (current_process == NULL)
        return 0;

    // Select the next process in the list (wrap around to head if at end)
    process_t *next_process = current_process->next;
    if (next_process == NULL)
        next_process = head_of_process;
    current_process = next_process;

    // Check if the process is in the ready state (status_flag == 1). If not, return 0.
    if (current_process->status_flag != 1)
        return 0;
    return current_process->sp;
}

/*
 * display_process_list
 *
 * Debugging helper: prints the list of processes with their function pointer, stack pointer,
 * status, and priority.
 */
void display_process_list() {
    process_t *temp = head_of_process;
    while (temp != NULL) {
        printf("Process: %p, Stack Pointer: %u, Status: %u, Priority: %u\n",
                temp->f, temp->sp, temp->status_flag, temp->priority);
        temp = temp->next;
    }
}

/*
 * display_stack
 *
 * Debugging helper: displays the current process's stack pointer and the first 10 values of its stack.
 */
void display_stack() {
    if (current_process == NULL) {
        printf("No current process.\n");
        return;
    }
    unsigned int *stack_pointer = (unsigned int *) current_process->sp;
    printf("Stack Pointer: %u\n", current_process->sp);
    printf("Stack Contents:\n");
    for (int i = 0; i < 10; i++) {
        printf("%u ", stack_pointer[i]);
    }
    printf("\n");
}