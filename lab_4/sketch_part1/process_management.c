#include "concurrency.h"
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>


static process_t *head_of_process = NULL; // head of the process list
process_t *current_process = NULL; // currently running process

/*
 * process_create(void (*f)(void), int n)
 *
 * Creates a new process that starts at function f, using n bytes of stack space.
 * Returns 0 on success, -1 on error.
 * 1. Allocate a process_t structure.
 * 2. Call process_init(f, n) to get an initial stack pointer.
 * 3. Insert the new process into your process list or queue.
 */
int process_create(void (*f)(void), int n) {
    cli();  // Disable interrupts while creating a new process.

    // 1. Allocate memory for the new process control block
    process_t *new_proc = (process_t *) process_malloc(sizeof(process_t));
    if (new_proc == NULL) {
        sei();  // Re-enable interrupts before returning
        return -1; 
    }

    // Initialize minimal fields sp=0, next=NULL (and extended fields if you use them)
    new_proc->sp          = 0;
    new_proc->next        = NULL;
    // Extended fields (optional):
    new_proc->status_flag = 0;
    new_proc->priority    = 0;
    new_proc->f           = f;

    sei();  // Allow interrupts briefly (to avoid long lock)

    // 2. Initialize the new stack with process_init (must be called with interrupts disabled)
    cli();
    unsigned int sp_init = process_init(f, n);
    sei();
    if (sp_init == 0) {
        // process_init failed => free the block (also with interrupts disabled)
        cli();
        free(new_proc);
        sei();
        return -1;
    }

    // Mark it as "ready" (if you track statuses) and store the new stack pointer
    new_proc->sp          = sp_init;
    new_proc->status_flag = 1;  // e.g., 1 = ready
    new_proc->priority    = 1;  // default priority

    // 3. Insert the new process into a linked list of processes
    cli();
    if (head_of_process == NULL) {
        head_of_process  = new_proc;
        // If this is the first process we ever create, we might not have a current_process.
        // But the lab says "current_process = NULL until process_start() is called."
        // We'll leave current_process = NULL here. process_start() will set it.
    } else {
        // Append to the end of the singly linked list
        process_t *temp = head_of_process;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_proc;
    }
    sei();

    return 0;  // Success
}

/*
 * process_start(void)
 *
 * This function:
 *  1. Initializes data structures for concurrency as needed.
 *  2. Calls process_begin() (provided by concurrency.c), which calls process_select(0).
 *  3. If process_begin() returns, that means all processes are done or a deadlock occurred.
 */
void process_start(void) {
    // If no processes were created, there's nothing to run
    if (head_of_process == NULL) {
        current_process = NULL;
        return;
    }

    // The lab states "This variable should also be NULL until process_start() is called."
    // Now we pick the first ready process in your list. 
    // For a minimal approach, just point current_process to the head to start.
    current_process = head_of_process;

    // Call process_begin(), which jumps into the concurrency system
    process_begin();

    // If process_begin() returns, concurrency ended (all processes finished or deadlock).
    while (1) {
        // In the lab example, we just stay here forever
    }
}

/*
 * process_select(unsigned int cursp)
 *
 * Called by the runtime (with interrupts disabled) to pick the next ready process.
 *  - cursp is the stack pointer of the currently running process.
 *  - If cursp=0, the current process finished or we have no current process.
 *  - If no process is ready, return 0.
 *  - Otherwise, return the stack pointer for the next ready process in your list.
 */
unsigned int process_select(unsigned int cursp) {
    // If there are no processes at all, we can't schedule anything
    if (head_of_process == NULL) {
        current_process = NULL;  
        return 0;  // no processes to run
    }

    // If we have a current_process, store its updated stack pointer if cursp != 0
    if (current_process != NULL && cursp != 0) {
        current_process->sp = cursp;
    }

    // Round-robin scheduling:
    // If current_process is NULL, start from the head.
    if (current_process == NULL) {
        current_process = head_of_process;
    } else {
        // Move to the next process in the list, or wrap around
        if (current_process->next != NULL) {
            current_process = current_process->next;
        } else {
            current_process = head_of_process;
        }
    }

    // Minimal approach: if we wanted to skip processes that are "not ready," we'd check status_flag.
    // For part 1, it's usually enough just to pick the next node.
    // If your design uses status_flag == 1 for "ready," you can optionally check it:

    if (current_process->status_flag != 1) {
        return 0;  // e.g., no ready process
    }

    // Return the chosen process's stack pointer
    return current_process->sp;
}

// Debug helper function to display the process list
void display_process_list() {
    process_t *temp = head_of_process;
    while (temp != NULL) {
        printf("Process f=%p, SP=%u, status=%u, priority=%u\n",
               temp->f, temp->sp, temp->status_flag, temp->priority);
        temp = temp->next;
    }
}

// Debug helper function to display the current process
void display_stack() {
    if (current_process == NULL) {
        printf("No current process.\n");
        return;
    }
    unsigned int *stk = (unsigned int *) current_process->sp;
    printf("Stack Pointer: %u\nStack Top 10:\n", current_process->sp);
    for (int i = 0; i < 10; i++) {
        printf("%u ", stk[i]);
    }
    printf("\n");
}
