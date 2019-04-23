## Readers-Writers Problem

CSC 360 Assignment 2a <br />
Summer 2018 <br />
Prof. Michael Zastre

### Assignment Specificications

C program implementing the solution to the readers-writers problem
- multiple readers can access the resource
- only one writer at a time can access the resource
- readers and writers cannot have simultaneous access

### Key Concepts Covered
- using mutexes (lock/release)
- using condition variables (thread waits until condition true)
