## Part 1

In Part 1, we use the struct `process_t`, which has two elements: `sp` (the process's stack pointer) and `next`, which is used to implement a linked list of processes.
We then implemented three functions: `process_create`, `process_start`, and `process_select` in `sketch_part1/process_management.c`. 
`process_create` allocates memory for a new `process_t` and its stack, initializes its variables, and adds it to the end of a linked list that starts at `head_of_process`.
`process_start` starts the processes running. It does this by simply calling `process_begin` after checking that the linked list is not empty.
`process_select` finds the next available process in the linked list. It then sets `current_process` to the next available process that can run. This also wraps around to the beginning of the linked list if it reaches the end.

To test the functionality of our implementation, we created four processes in `sketch_part1.ino`.  The first process loops through the characters `a` to `z` and prints them to the serial port. The second process is a counter that prints the numbers starting at 0 to the serial port. The third process is similar to p2, but it prints in increments of 1000. Process 4 blinks the blue LED 5 times every 1000 ms. Process 5 blinks the build in LED 10 times every 20 ms.
We tested our implementation by running different combinations of the five processes and saw how they appeared to run at the same time and interleaved. Videos of some of these tests can be seen in the folder sketch_part1.


## Part 2

In Part 2, we created the struct `lock_t` to act as a lock. It has two variables: a boolean `is_taken` (true if the lock is currently being used by a process) and 'p' (the process using the lock).
We implemented three functions: `lock_init` (initializes the lock), `lock_acquire` (the current process takes the lock), and `lock_release` (the process releases the lock). These functions are in `sketch_part2/process_management.c'.
Before any process's critical section, it must first wait until the lock is not taken. Then, it must acquire the lock. This will block any other process from accessing their critical section, thereby ensuring mutual exclusion. After the process is done with a critical section, it must release the lock to allow another process to enter their critical section.

We made three processes to test this functionality (in `sketch_part2.ino`). Process 1 blinks the green LED every 1000 ms. Process 2 blinks the green LED 10 times every 4500 ms. Process 3 blinks the blue LED 5 times every 2000 ms.
We again tested different combinations of these processes. For this, we noted the mutual exclusion was working correctly because the LED would not blink multiple colors at the same time, always being either red, green, or blue and not a combination. We also saw that no process's critical section would get blocked from running indefinitely.
Videos of these tests can be seen in `part2_test1.mov` and `part2_test2.mov`
