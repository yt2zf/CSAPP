
codeInj3.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <.text>:
   0:	68 fa 18 40 00       	push   $0x4018fa
   5:	48 83 ec 38          	sub    $0x38,%rsp
   9:	48 b8 35 39 62 39 39 	movabs $0x6166373939623935,%rax
  10:	37 66 61 
  13:	48 89 04 24          	mov    %rax,(%rsp)
  17:	c6 44 24 08 00       	movb   $0x0,0x8(%rsp)
  1c:	48 89 e7             	mov    %rsp,%rdi
  1f:	48 83 c4 38          	add    $0x38,%rsp
  23:	c3                   	ret
