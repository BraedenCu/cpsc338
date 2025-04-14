#include "./concurrency/concurrency.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* 
   Global variables for our process list.
   current_process points to the currently running process.
   head_of_process holds the head of our process list so we can always wrap around.
*/
process_t *head_of_process = NULL;
process_t *current_process = NULL;

/*
 * process_create
 *
 * This function creates a process that starts at function f(), with an initial
 * stack size that can hold n bytes. It returns -1 on error, and 0 on success.
 *
 * NOTE: The lab-provided helper process_init is used to allocate and initialize
 * the stack. In addition, this implementation uses additional fields (f, status_flag,
 * and priority) to support scheduling. The entire update to the process list is done
 * in a critical section (with interrupts disabled).
 */
int process_create(void (*f)(void), int n) {
    // Disable interrupts for allocation and list modification
    asm volatile ("cli");

    // Allocate memory for the new process using process_malloc()
    process_t *new_process = (process_t *) process_malloc(sizeof(process_t));
    if (new_process == NULL) {
        asm volatile ("sei");  // Re-enable interrupts before returning
        return -1;
    }

    // Initialize the basic fields of the process
    new_process->sp = 0;            // Will be updated after stack initialization
    new_process->f = f;             // Function pointer for the process
    new_process->status_flag = 0;   // Not ready initially
    new_process->priority = 0;      // Default priority (can be adjusted later)
    new_process->next = NULL;       // End-of-list marker

    asm volatile ("sei");  // Re-enable interrupts temporarily

    // Initialize the process stack using the helper function process_init()
    unsigned int stack_pointer = process_init(f, n);
    if (stack_pointer == 0) {
        // If stack initialization fails, free memory in a critical section
        asm volatile ("cli");
        free(new_process);
        asm volatile ("sei");
        return -1;
    }
    new_process->sp = stack_pointer;  // Set the newly initialized stack pointer
    new_process->status_flag = 1;       // Mark process as ready
    new_process->priority = 1;          // Set a default priority

    // Add the new process to our process list in a critical section
    asm volatile ("cli");
    if (head_of_process == NULL) {
        // First process in the list
        head_of_process = new_process;
        current_process = new_process;
    } else {
        // Append to the end of the existing list
        process_t *temp = head_of_process;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_process;
    }
    asm volatile ("sei");

    // DO NOT call process_begin() here.
    // process_begin() should be called only once from process_start() after all processes are created.
    return 0;
}

/*
 * process_start
 *
 * This function starts the execution of concurrent processes. It initializes
 * necessary scheduling data structures and invokes process_begin() to start the first process.
 * After process_begin() returns (e.g., when all processes have terminated or deadlock occurs),
 * this function enters an infinite loop.
 */
void process_start(void) {
    if (head_of_process == NULL) {
        // No processes have been created; nothing to schedule.
        return;
    }
    // Set current_process to the head of the process list.
    current_process = head_of_process;
    // Begin process execution; process_begin() is provided and will call process_select()
    process_begin();

    // If process_begin() returns, then all processes have finished or a deadlock occurred.
    while (1);
}

/*
 * process_select
 *
 * This function is invoked in a critical section (with interrupts disabled) to
 * choose the next ready process. It implements a basic round-robin scheduler.
 * It returns the stack pointer of the next process to run, or 0 if no process is ready.
 */
unsigned int process_select(unsigned int cursp) {
    if (current_process == NULL) {
        return 0;   // No process is currently running.
    }
    // For round-robin scheduling: move to the next process in the list,
    // wrapping around to the head if necessary.
    process_t *next_process = current_process->next;
    if (next_process == NULL) {
        next_process = head_of_process;
    }
    current_process = next_process;
    
    // Optionally, additional checks on status_flag or priority can be added here.
    return current_process->sp;
}

/*
 * display_process_list
 *
 * Helper function to print out the current process list. Useful for debugging.
 */
void display_process_list() {
    process_t *temp = head_of_process;
    while (temp != NULL) {
        // Print the process's function pointer, stack pointer, status flag, and priority.
        printf("Process: %p, Stack Pointer: %u, Status: %u, Priority: %u\n",
                temp->f, temp->sp, temp->status_flag, temp->priority);
        temp = temp->next;
    }
}

/*
 * display_stack
 *
 * Helper function to display the current process's stack pointer and the first 10 values of its stack.
 */
void display_stack() {
    if (current_process == NULL) {
        printf("No current process.\n");
        return;
    }
    unsigned int *stack_pointer = (unsigned int *) current_process->sp;
    printf("Stack Pointer: %u\n", current_process->sp);
    printf("Stack Contents:\n");
    for (int i = 0; i < 10; i++) {  // Display the first 10 stack values
        printf("%u ", stack_pointer[i]);
    }
    printf("\n");
}