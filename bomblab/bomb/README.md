## Environment
Server in Tecent Cloud: Ubuntu Server 24.04 LTS 64bit

The original tar file is `bomb.tar`, `tar xvf bomb.tar`

Install GDB: `sudo apt install gdb`

## Useful GDB Commands
`x /s <addr>`: print string starting from given memory address
`x/NFU <addr>`: x is used to check memory data; N is the number of units, F is format(x:hex, d:decimal), 
U is unit size(b:1 byte, h:2 bytes, w:4 bytes, g:8 bytes).

## Reflection
To solve Phase5 and Phase6, converting assembly code to pseudo-code is necessary.

