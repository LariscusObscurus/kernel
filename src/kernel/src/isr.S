/*
 * isr.S
 *
 *  Created on: 02.05.2015
 *      Author: leonhardt
 */


.macro def_isr num
.global isr\num
isr\num:
	pushl $0
	pushl $\num
	jmp isr_common
.endm

.macro def_exception_error_code num
.global isr\num
isr\num:
	pushl $\num
	jmp isr_common
.endm

//http://www.lowlevel.eu/wiki/Exception#Liste_der_Exceptions
def_isr 0
def_isr 1
def_isr 2
def_isr 3
def_isr 4
def_isr 5
def_isr 6
def_isr 7
def_exception_error_code 8
def_isr 9
def_exception_error_code 10
def_exception_error_code 11
def_exception_error_code 12
def_exception_error_code 13
def_exception_error_code 14
def_isr 15
def_isr 16
def_exception_error_code 17
def_isr 18

//IRQ
def_isr 32
def_isr 33

def_isr 48

.extern temp_isr

isr_common:
	push %ebp
	push %edi
	push %esi
	push %edx
	push %ecx
	push %ebx
	push %eax

	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es

	push %esp
	call tmp_isr
	mov %eax, %esp

    mov $0x23, %ax
	mov %ax, %ds
	mov %ax, %es

	pop %eax
	pop %ebx
	pop %ecx
	pop %edx
	pop %esi
	pop %edi
	pop %ebp

	add $8, %esp

	iret
