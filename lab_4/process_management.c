#include "concurrency.h"
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>

/* 
   Global variables for our process list.
   head_of_process: pointer to the head of the process list.
   current_process: pointer to the currently running process.
*/
process_t *head_of_process = NULL;
process_t *current_process = NULL;

/*
 * process_create
 *
 * Creates a process to start at function f with an initial stack size of n bytes.
 * Returns 0 on success, -1 on error.
 */
int process_create(void (*f)(void), int n) {
    cli(); // Disable interrupts for critical section

    process_t *new_process = (process_t *) process_malloc(sizeof(process_t));
    if (new_process == NULL) {
        sei();
        return -1;
    }
    new_process->sp = 0;
    new_process->f = f;
    new_process->status_flag = 0;  // Not ready until stack initialized
    new_process->priority = 0;     // Default priority; can be adjusted later
    new_process->next = NULL;
    sei();

    // Call process_init with interrupts disabled as required.
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
    new_process->status_flag = 1; // Mark as ready
    new_process->priority = 1;    // Set default priority

    // Insert the new process into the list
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
 * Starts concurrent execution by invoking process_begin(). If process_begin()
 * returns (when all processes have terminated or a deadlock occurs), loop forever.
 */
void process_start(void) {
    if (head_of_process == NULL) {
        return;
    }
    current_process = head_of_process;
    process_begin();
    while (1);
}

/*
 * process_select
 *
 * Called with interrupts disabled to select the next ready process.
 * If the current process is terminated, remove it from the list.
 * Returns the stack pointer for the next process or 0 if no ready process remains.
 */
unsigned int process_select(unsigned int cursp) {
    // Remove terminated processes from the list
    while (current_process != NULL && current_process->status_flag != 1) {
        // Remove current_process from the list.
        process_t *to_remove = current_process;
        // If it's the head, update head_of_process.
        if (head_of_process == to_remove) {
            head_of_process = to_remove->next;
        } else {
            // Find previous node to bypass the terminated process.
            process_t *prev = head_of_process;
            while (prev && prev->next != to_remove) {
                prev = prev->next;
            }
            if (prev) {
                prev->next = to_remove->next;
            }
        }
        current_process = to_remove->next;
        free(to_remove);
        // If current_process becomes NULL, wrap to head
        if (current_process == NULL)
            current_process = head_of_process;
    }

    if (current_process == NULL) {
        return 0;  // No ready process remains.
    }

    // Implement round-robin scheduling.
    process_t *next_process = current_process->next;
    if (next_process == NULL) {
        next_process = head_of_process;
    }
    current_process = next_process;

    // If the newly selected process isn't ready, try selecting again.
    if (current_process->status_flag != 1)
        return process_select(cursp);
    return current_process->sp;
}

/*
 * display_process_list
 *
 * Debug helper to print details of each process in the list.
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
 * Debug helper: prints the current process's stack pointer and the first 10 words of its stack.
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