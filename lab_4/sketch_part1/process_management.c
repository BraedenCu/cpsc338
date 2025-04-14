#include "concurrency.h"
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>


static process_t *head_of_process = NULL; // head of the process list
process_t *current_process = NULL; // currently running process


int process_create(void (*f)(void), int n) {
    cli();  

    process_t *new_proc = (process_t *) process_malloc(sizeof(process_t));
    if (new_proc == NULL) {
        sei();  
        return -1; 
    }

    new_proc->sp          = 0;
    new_proc->next        = NULL;
    // new_proc->status_flag = 0;
    // new_proc->priority    = 0;
    // new_proc->f           = f;

    sei();  

    cli();
    unsigned int sp_init = process_init(f, n);
    sei();
    if (sp_init == 0) {
        cli();
        free(new_proc);
        sei();
        return -1;
    }

    new_proc->sp          = sp_init;
    // new_proc->status_flag = 1;  // 1 = ready
    // new_proc->priority    = 1;  // default priority

    cli();
    if (head_of_process == NULL) {
        head_of_process  = new_proc;
    } 
    else {
        process_t *temp = head_of_process;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_proc;
    }
    sei();

    return 0; // success
}

void process_start(void) {
    if (head_of_process == NULL) {
        // current_process = NULL;
        return;
    }

    // current_process = head_of_process;

    process_begin();

    // while (1) {
    //     // stay here forever
    // }
}

unsigned int process_select(unsigned int cursp) {
    if (head_of_process == NULL) {
        current_process = NULL;  
        return 0;  // no processes to run
    }

    if (current_process != NULL && cursp != 0) {
        current_process->sp = cursp;
    }

    // round robin approach
    if (current_process == NULL) {
        current_process = head_of_process;
    } 
    else {
        // move to next in list, wrap around for RR
        if (current_process->next != NULL) {
            current_process = current_process->next;
        } else {
            current_process = head_of_process;
        }
    }

    // if (current_process->status_flag != 1) {
    //     return 0;  // no ready process
    // }

    return current_process->sp; // return choosen process stack pointer
}

// void display_process_list() {
//     process_t *temp = head_of_process;
//     while (temp != NULL) {
//         printf("Process f=%p, SP=%u, status=%u, priority=%u\n",
//                temp->f, temp->sp, temp->status_flag, temp->priority);
//         temp = temp->next;
//     }
// }

// void display_stack() {
//     if (current_process == NULL) {
//         printf("No current process.\n");
//         return;
//     }
//     unsigned int *stk = (unsigned int *) current_process->sp;
//     printf("Stack Pointer: %u\nStack Top 10:\n", current_process->sp);
//     for (int i = 0; i < 10; i++) {
//         printf("%u ", stk[i]);
//     }
//     printf("\n");
// }
