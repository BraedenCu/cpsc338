#include "./concurrency/concurrency.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// implement process_create
/*
This function creates a process that starts at function f(), with an initial stack size that can hold n bytes. This function should return -1 on an error, and 0 on success. The implementation of this function may require that you allocate memory for a process_t structure, and you can use malloc() for this purpose. Remember that malloc() returns NULL if it could not allocate the requested memory. To help you with stack manipulation, we provide function process_init (detailed below).

/* Note: should think about your memory map and make sure you know how spaced is used for each proc and have a utility to /* review this part of memory before too far aloZng.
*/
int process_create(void (*f)(void), int n) {
    asm volatile ("cli"); // disable interrupts

    process_t *new_process = process_malloc(sizeof(process_t)); // allocate memory for new process

    asm volatile ("sei"); // reenable interrupts

    if (new_process == NULL) {
        return -1; // memory allocation failed
    }
    new_process->sp = 0; // initialize stack pointer
    new_process->status_flag = 0; // initialize status flag
    new_process->f = f; // set function to execute
    new_process->next = NULL; // initialize next pointer
    new_process->priority = 0; // initialize priority

    unsigned int stack_pointer = process_init(f, n); // initialize stack pointer

    if (stack_pointer == 0) {
        asm volatile ("cli"); // disable interrupts
        free(new_process); // free memory if initialization fails
        asm volatile ("sei"); // reenable interrupts
        return -1; // initialization failed
    }
    new_process->sp = stack_pointer; // set the stack pointer in the process structure
    new_process->status_flag = 1; // set status flag to ready
    new_process->priority = 1; // set priority (default to 1)

    // add the new process to the process list
    if (current_process == NULL) {
        current_process = new_process; // head
    } 
    else {
        process_t *temp = current_process;
        while (temp->next != NULL) {
            temp = temp->next; // traverse to end of the list
        }
        temp->next = new_process; // append to the end
    }

    process_begin(); // start the first process

    return 0; // return 0 on success
}

// implement process_start
/*
This function starts the execution of concurrent processes. This function must initialize any data structures you need to implement concurrency, and start the execution of the first ready process. You can start the execution of the first process by calling the function process_begin () that we have provided. Note that process_begin() will call a function that you have to implement called process_select (detailed below).
*/
void process_start(void) {
    if(current_process == NULL) {
        return -1; // no processes to start
    }
    process_t *temp = current_process;
    while (temp != NULL) {
        temp->f(); // execute the function
        temp = temp->next; // move to the next process
    }
}

// implement process_select
/*
The code we provide will call process_select() with interrupts disabled. Called a "critical section" 
The argument to process_select() is the value of the stack pointer for the currently running process. If there are no currently running processes, the argument will be 0. If the currently running process has terminated, the argument to process_select() will also be 0. If there is no process ready to execute, process_select() should return 0; if there is a ready process, process_select() must return the value of the stack pointer for the next ready process.
*/
unsigned int process_select(unsigned int cursp) {
    if (current_process == NULL) {
        return 0; // no processes to select
    }
    process_t *temp = current_process;
    while (temp != NULL) {
        if (temp->sp == cursp) {
            return temp->sp; // return the stack pointer of the next process
        }
        temp = temp->next; // move to the next process
    }
    return 0; // no matching process found
}