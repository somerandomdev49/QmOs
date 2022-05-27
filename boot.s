/* first code to be ran */
/* See https://www.gnu.org/software/grub/manual/multiboot/multiboot.html for further info */
.set ALIGN,    1<<0
.set MEMINFO,  1<<1
.set VIDEO_MODE, 1 << 2
.set INFO, (1 << 11)
.set FLAGS,    ALIGN | MEMINFO | VIDEO_MODE | INFO
.set MAGIC,    0x1BADB002 
.set CHECKSUM, -(MAGIC + FLAGS)
.set MODE_TYPE, 0 /* - linear graphics mode */
.set WIDTH, 1024 /* pixels */
.set HEIGHT, 768 /* pixels */
.set DEPTH, 32 /* bits per pixel */

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
.long 0,0,0,0,0 /* unused */
.long MODE_TYPE
.long WIDTH
.long HEIGHT
.long DEPTH

.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

.section .text
.global _start
.type _start, @function
_start:
	mov $stack_top, %esp
	push %ebx
	push %eax
	call kernel_main
	cli
1:	hlt
	jmp 1b

.size _start, . - _start
