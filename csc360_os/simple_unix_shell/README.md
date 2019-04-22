## Simple UNIX Shell

CSC 360 Assignment 1 <br />
Summer 2018 <br />
Prof. Michael Zastre

### Assignment Specificications

C program implementing very simple UNIX shell
- prompts user for commands
- spawns child process to execute given commands
- up to 7 arguments accepted per commands
- Output redirection implemented: e.g. "OR ls -l -> out.txt
- Piping implemeneed: e.g. "PP ls -l -> wc -l"

### Key Concepts Covered
- spawning and terminating child processes
- executing commands using C command *execve()*

