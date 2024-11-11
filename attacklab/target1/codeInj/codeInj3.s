pushq  $0x4018fa
sub    $0x38,%rsp
movabsq $0x6166373939623935,%rax
movq   %rax,(%rsp)
movb   $0x0,0x8(%rsp)
mov    %rsp,%rdi
add    $0x38,%rsp
ret
