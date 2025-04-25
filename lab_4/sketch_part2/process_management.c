#include "concurrency.h"
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>


static process_t *head_of_process = NULL; // head of the process list
process_t *current_process = NULL; // currently running process


int process_create(void (*f)(void), int n) {
    cli();  // disable interrupts

    process_t *new_proc = (process_t *) process_malloc(sizeof(process_t));
    
    if (new_proc == NULL) 
    {
        sei();  
        return -1; 
    }

    new_proc->sp   = 0;
    new_proc->next = NULL;

    sei(); // renable interrupts

    cli(); // disable interrupts
    
    unsigned int sp_init = process_init(f, n);
    
    sei(); //renable interrupts
    
    if (sp_init == 0) 
    {
        cli(); // disable interrupts
        free(new_proc); // free mem
        sei(); // renable interrupts
        return -1;
    }

    new_proc->sp = sp_init;

    cli(); // disable interrupts
    
    if (head_of_process == NULL) 
    {
        head_of_process  = new_proc; // no head, process is now head 
    } 
    else 
    {
        process_t *temp = head_of_process;
        while (temp->next != NULL) 
        {
            temp = temp->next;
        }
        temp->next = new_proc; // store process at the end
    }
    
    sei(); // renable interrupts

    return 0; // success
}

void process_start(void) {
    if (head_of_process == NULL) 
    {
        return;
    }

    process_begin();
}

unsigned int process_select(unsigned int cursp) {
    if (head_of_process == NULL) 
    {
        current_process = NULL;  
        return 0;  // no processes to run
    }

    if (current_process != NULL && cursp != 0) 
    {
        current_process->sp = cursp;
    }

    // round robin approach
    if (current_process == NULL) 
    {
        current_process = head_of_process; // done if only one
    } 
    else 
    {
        // move to next in list, wrap around for RR
        if (current_process->next != NULL) 
        {
            current_process = current_process->next;
        } 
        else 
        {
            current_process = head_of_process;
        }
    }
    
    return current_process->sp; // return choosen process stack pointer
}



void lock_init(lock_t *lk) {
  lk->is_taken = false; // check if lock is taken
}

// atomic test-and-set
void lock_acquire(lock_t *lk) {
  while (1) 
  {
    cli();                         // start critical section
      if (!lk->is_taken) 
      {
        lk->is_taken = true;       // claim the lock
        sei();                     // end critical section
        return;
      }
    sei();                         // release critical section
    yield();                       // let other process run
  }
}

void lock_release(lock_t *lk) 
{
  cli();                          
  lk->is_taken = false;           // free the lock
  sei();
}
