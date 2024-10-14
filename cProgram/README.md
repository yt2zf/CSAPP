## Environment
Server in Tecent Cloud: Ubuntu Server 24.04 LTS 64bit

The original tar file is `cprogramminglab-handout.tar`, 
`tar xvf cprogramminglab-handout.tar`

## Compile Error
`cd cprogramminglab-handout`
`make`
--> Compile Error:
```
/usr/bin/ld: /tmp/ccB6WZAh.o:/home/ubuntu/csapp/cProgram/cprogramminglab-handout/report.h:93: multiple definition of `peak_bytes'; /tmp/cccUXzcM.o:/home/ubuntu/csapp/cProgram/cprogramminglab-handout/report.h:93: first defined here
/usr/bin/ld: /tmp/ccB6WZAh.o:/home/ubuntu/csapp/cProgram/cprogramminglab-handout/report.h:96: multiple definition of `last_peak_bytes'; /tmp/cccUXzcM.o:/home/ubuntu/csapp/cProgram/cprogramminglab-handout/report.h:96: first defined here
/usr/bin/ld: /tmp/ccB6WZAh.o:/home/ubuntu/csapp/cProgram/cprogramminglab-handout/report.h:99: multiple definition of `current_bytes'; /tmp/cccUXzcM.o:/home/ubuntu/csapp/cProgram/cprogramminglab-handout/report.h:99: first defined here
......
```
To fix this, in report.h change "size_t peak_bytes; size_t last_peak_bytes; size_t current_bytes;" to 
"extern size_t peak_bytes; extern size_t last_peak_bytes; extern size_t current_bytes;",
in harness.h change "int fail_probability" to "extern int fail_probability".

References: https://stackoverflow.com/questions/69908418/multiple-definition-of-first-defined-here-on-gcc-10-2-1-but-not-gcc-8-3-0

## Testing
-To run `make test`, there're 2 options:
1. Install python2 in your environment.
2. find a python3 version of `driver.py`, and replace the original python2 version of `driver.py`.
I choose to get a python3 version of 'driver.py' since `sudo apt install python2` fails in my environment.

## Reflection
Some test cases are tricky. Here are my lessons:
1. When copying the first k characters of string to a char array, remember to set k-th index of char array to `\0`.    
2. When freeing a, if a.foo == NULL, remembers to free(a).
3. When a function relies on the success of a series of memory allocation, if one memory allocation fails, remember to free the previous allocated memory. 





