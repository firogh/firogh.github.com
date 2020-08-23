/* as -o g gas_section.s ;objdump -D ./g; */
/* details in source code gas elf_pseudo_table in gas/config/obj-elf.c */

2:      movl %edx,%ecx
3:      rep 
        movsb
        xorl %eax,%eax
        ret

        .section .fixup1,"ax"
12:     movl $1,%edx          
        .previous

        .section .fixup2,"ax"
16:     movl $2,%edx          

        .section .fixup3,"ax"
16:     movl $3,%edx          

        .section .fixup4,"ax"
16:     movl $4,%edx          
        .section .fixup5,"ax"
16:     movl $5,%edx          
        .section .fixup6,"ax"
16:     movl $6,%edx          
        .previous
        .previous
16:     movl $7,%edx          /* expect this instruction in .fixup4, however it ends up in .fixup6 */
