
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
 * Filename:	$RCSfile: init16.S,v $
 * Createdate:	2004/04/20 09:16:00
 * FileId:	$Id: init16.S,v 1.1.1.1 2005/08/09 05:25:24 devin Exp $
 * Last update:	$Date: 2005/08/09 05:25:24 $
 * Creator:	Devin Wang <vhd@21cn.com>
 * Version:	$Revision: 1.1.1.1 $
 * Description:  switch to protection mode.
 *
 * ====================================================================
 */

/* ====================================================================
 *  Revision:
 *
 * ====================================================================
*/


	
#include <yios/config.h>
#include <yios/hal/boot.h>
#include <yios/hal/init.h>

.code16

.text

# kernel32.sys start from here.
.globl _start
_start:
	jmp	restart
	nop
.org 	0x10

init_addr:
	.long	0x0		# init32 start address (bload set the address.)

restart:
	cli
	movw	$YI_OSKERNELSYSSEG, %ax
	movw	%ax, %ds
	movw	%ax, %es

#if CFG_GUI == 0
# set display mode
        movw    $0x0003, %ax
        int     $0x10           # change display mode.

        movw    $0x0500, %ax    # set current display page.
        int     $0x10

        movw    $0x0200, %ax    # set curs position.
        xorw    %dx, %dx
        xorw    %bx, %bx
        int     $0x10

        movw    $2000, %cx      # set backgroud color.
        movw    $0x0900, %ax
        movw    $0x003f, %bx
        int     $0x10
#else /* CFG_GUI != 1 */

#if CFG_VBE == 0
        movw    $0x0012, %ax    # change display mode to 640*480*4
        int     $0x10
#else /* CFG_VBE ==1 */
        call    vesa_init       # use vbe
#endif /* CFG_VBE */

#endif /* CFG_GUI */


# set bios data 0x40:0x15(main memory size) to 640K-64K for RDATA
	xorw	%ax, %ax
	movw	%ax, %es
	movw	$0x0258, %ax
	movw	%ax, %es:(0x415)

# call e820 to get system memory map information
#define SMAP	0x534d4150
	pushw	%es
	movw	$(YI_KE_E820_MAPS_SEG), %ax
	pushw	%ax

	
	xorl	%eax, %eax
	popw	%es
	pushw	%es
	movl	%eax, %es:(YI_KE_E820_NR_SEGOFF)

	movw	$(YI_KE_E820_MAPS_SEGOFF), %di
	xorl	%ebx, %ebx
e820_begin:
	movl	$0x0000e820, %eax
	movl	$20, %ecx
	movl	$SMAP, %edx
	popw	%es
	pushw	%es
	int	$0x15
	jc	test

	cmpl	$SMAP, %eax
	jne	test
good820:
	movb	%es:(YI_KE_E820_NR_SEGOFF), %al
	cmpb	$32, %al
	jnl	test

	incb	%es:(YI_KE_E820_NR_SEGOFF)
	movw	%di, %ax
	addw	$20, %ax
	movw	%ax, %di
again820:
	cmpl	$0, %ebx
	jne	e820_begin
	popw	%es
	popw	%es

# set init32 start address
	movl	init_addr, %eax
	movl	%eax, code32
	xorl	%eax, %eax

# move gdt into RDATA as GDT
	movw	$gdt, %ax
	movw	%ax, %si
	movw	$YI_KE_GDT_SEG, %ax
	movw	%ax, %es
	movw	$(gdt_end-gdt), %cx
	xorw	%di, %di
	cld
	rep
	movsb

# begin to enter protect mode.
	cli
	movw	%ds, %bx
	movw	$0x6000, %ax
	movw	%ax, %ds
	movw	%bx, %ds
    
	lidt	idtv
	lgdt	gdtv
	
	inb	$0x92, %al	# open A20 gate
	orb	$0x02, %al
	outb	%al, $0x92

	movl	%cr0, %eax
	orl	$0x00000001, %eax
	movl	%eax, %cr0	# enter protect mode.

	.byte	0x66, 0xea	# jump to code32
code32:
	.long	0x0		    # entry address
	.word	0x0010		# code segment selector

code16:

test:
	cli
	hlt

		
.align 16

gdt:
	.word	0, 0, 0, 0
	.word	0, 0, 0, 0

	.word	0xffff		# kernel code segment
	.word	0
	.word	0x9a00
	.word	0x00cf

	.word	0xffff		# kernel data segment
	.word	0
	.word	0x9200
	.word	0x00cf

	.word	0xffff		# user code segment
	.word	0
	.word	0xfa00
	.word	0x00cf

	.word	0xffff		# user data segment
	.word	0
	.word	0xf200
	.word	0x00cf


	.word	0xffff		# 16 bit data segment
	.word	0x0000
	.word	0x0009
		
gdt_end:


gdtv:
	.word	0x0028
	.long	YI_KE_GDT_PADDR

idtv:
	.word	0x07ff
	.long	YI_KE_GDT_PADDR

