#include "./concurrency/concurrency.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// define process data structure
typedef struct process_t {
    unsigned int sp; // stack pointer
    unsigned int id; // process ID
    unsigned int status_flag; // status flag
    void (*f)(void); // function to execute
    struct process_t *next; // pointer to next process
}

// implement process_create


// implement process_start