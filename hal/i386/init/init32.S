	
/* ====================================================================
 * The YingOS project.
 * Copyright (C) 2004 Devin Wang

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

 * ====================================================================
 *
 * Filename:	$RCSfile: init32.S,v $
 * Createdate:	2004/04/20 09:16:00
 * FileId:	$Id: init32.S,v 1.1.1.1 2005/08/09 05:25:24 devin Exp $
 * Last update:	$Date: 2005/08/09 05:25:24 $
 * Creator:	Devin Wang <vhd@21cn.com>
 * Version:	$Revision: 1.1.1.1 $
 * Description: Decompress kernel32 into memory.
 *
 * ====================================================================
 */

/* ====================================================================
 *  Revision:
 *
 * ====================================================================
 */

#include <yios/config.h>
#include <yios/hal/init.h>


.code32
.text
.data
.bss

.text

.globl	init32
init32:
	cli
	cld
	jmp	restart
	nop

.org	0x10

# information for compressed kernel32.

kernel32_position:
	.long	0x0000
kernel32_size:
	.long	0x0000
kernel32_pre_size:
	.long	0x0000
kernel32_de_size:
	.long	0x0000

restart:
	movw	$YI_HAL_KE_DS_SELC, %ax
	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %fs
	movw	%ax, %gs

	movw	$YI_HAL_KE_SS_SELC, %ax
	movw	%ax, %ss
	
	movl	$YI_INIT32_UNZIP_STACK_PADDR,	%esp

# move	oskernel32.gz for generating .bss gap

	movl	$yi_g_nLdInit32BssEnd, %edi
	movl	kernel32_position, %esi
	movl	%edi, kernel32_position

	movl	kernel32_size, %ecx
	addl	%ecx, %edi
	addl	%ecx, %esi

	incl	%ecx
	incl	%ecx
	shrl	$0x2, %ecx
	incl	%ecx
	incl	%ecx

	std
	rep
	movsl

	cld

# memzero init32 .bss
	movl	$yi_g_nLdInit32BssStart, %ebx
	movl	$yi_g_nLdInit32BssEnd, %ecx
	subl	%ebx, %ecx
	shrl	$0x2, %ecx
	xorl	%eax, %eax
	xorl	%edx, %edx
bbsto0_loop:
	movl	%eax, (%ebx, %edx, 4)
	incl	%edx
	loop	bbsto0_loop
	
# decompress kernel32(It's a elf format file.)

	pushl	$kernel32_de_size	# output size
	pushl	$CFG_KERNEL_IMAGE_PADDR	# output buffer
	pushl	kernel32_size		# input size
	pushl	kernel32_position	# input buffer
	call	unzip

## if unzip loader fault, we halt system.
	movl	kernel32_de_size, %eax
	cmpl	$0x0, %eax	# if output size is zero, unzip error .
	jne	decom_ok

decom_fault:
	cli
	hlt

decom_ok:
# unzip OK!
# jump to kenrel start32.S
	movl	(CFG_KERNEL_IMAGE_PADDR + 0x18), %eax	# get elf file start address
	subl	$YI_KERNEL_OFFSET, %eax			# current use no page mode
#	movl	$(0x101000), %eax
	jmp	*%eax


.align 16
	nop


