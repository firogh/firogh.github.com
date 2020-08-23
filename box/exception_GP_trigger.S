.data
    .quad msg 

msg:
    .ascii "Hello, world!\n"
    len = . - msg 
saved_idt:
        .long 0,0 

.text
    .global _start

_start:
    movl  $4, %eax
    movl  $1, %ebx  
    movl  $msg, %ecx 
    sidt  saved_idt
    lidt  saved_idt  #===============> crashed at here
    movl  $len, %edx 
    int   $0x80
    
    movl  $1, %eax
    xorl  %ebx, %ebx 
    int   $0x80
