	
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
 * Filename:	$RCSfile: start32.S,v $
 * Createdate:	2004/06/03 15:42:00
 * FileId:	$Id: start32.S,v 1.1.1.1 2005/08/09 05:25:24 devin Exp $
 * Last update:	$Date: 2005/08/09 05:25:24 $
 * Creator:	Devin Wang <vhd@21cn.com>
 * Version:	$Revision: 1.1.1.1 $
 * Description:  Switch to page mode and start system.
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

.globl	_yike_start
_yike_start:
	cli
	cld
    
	movw	$YI_HAL_KE_DS_SELC, %ax
	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %fs
	movw	%ax, %gs

# get  memory size.
#	movl	$(yi_g_nLdAllEnd-YI_KERNEL_OFFSET), %ebx
#	movl	$(PAGE_SIZE), %eax
#	movl	%eax, %edx
#	addl	%edx, %ebx
#	decl	%eax
#	notl	%eax
#	andl	%eax, %ebx	# free memory start.

#	movl	$0x55AA55AA, %eax

#memsize_loop:
#	movl	%eax, (%ebx)
#	cmpl	%eax, (%ebx)
#	jne	memsize_ok
#	addl	%edx, %ebx
#	jmp	memsize_loop
#memsize_ok:
#	movl	%ebx, (yi_g_nMemSystemHighestMemory-YI_KERNEL_OFFSET)

# prepare enter page mode
# fill null to PDT and PGT
	movl	$YI_KE_PDT_PADDR, %eax
	movl	$0x0, %edx
	movl	$((1+YI_KE_PGT_NUM)*(PAGE_SIZE>>2)), %ecx		# 1*PDT+8*PGT

fill_null:
	movl	%edx, -4(%eax, %ecx, 4)
	loop	fill_null

# set PDT item 0 for 0~3M
	movl	$(YI_KE_PGT_PADDR), %eax
	andl	$(YI_PAGE_ADDR_MASK), %eax	# get page address
	orl	$(YI_PAGE_PROP_PRESENT), %eax
	orl	$(YI_PAGE_PROP_WRITEABLE), %eax
	movl	%eax, YI_KE_PDT_PADDR

# set PDT item 512 for system high memory map in 2G ~ 2G+8M
	movl	$(YI_KE_PGT_PADDR+PAGE_SIZE), %eax
	andl	$(YI_PAGE_ADDR_MASK), %eax	# get page address
	orl	$(YI_PAGE_PROP_PRESENT), %eax
	orl	$(YI_PAGE_PROP_WRITEABLE), %eax
	movl	$YI_PAGE_ADDR_TO_PDT(YI_KERNEL_OFFSET), %edx	# first 4M
	movl	$(YI_KE_PDT_PADDR), %ebx
	movl	%eax, (%ebx, %edx, 4)
	
	incl	%edx			# second 4M
	addl	$PAGE_SIZE, %eax
	movl	%eax, (%ebx, %edx, 4)

# set PDT item 1023 for system stack in 4G-4k ~ 4G
	movl	$(YI_KE_PGT_PADDR+PAGE_SIZE*3), %eax
	andl	$(YI_PAGE_ADDR_MASK), %eax	# get page address
	orl	$(YI_PAGE_PROP_PRESENT), %eax
	orl	$(YI_PAGE_PROP_WRITEABLE), %eax
	movl	$YI_PAGE_ADDR_TO_PDT(YI_KE_STACK_TOP_VADDR), %edx
	movl	%eax, (%ebx, %edx, 4)


#if CFG_VBE == 1
# set PDT item 1022 for VBE in 4G-8M ~ 4G-4M
	movl	$(YI_KE_PGT_PADDR+PAGE_SIZE*4), %eax
	andl	$(YI_PAGE_ADDR_MASK), %eax	# get page address
	orl	$(YI_PAGE_PROP_PRESENT), %eax
	orl	$(YI_PAGE_PROP_WRITEABLE), %eax
	movl	$YI_PAGE_ADDR_TO_PDT(YI_KE_VBE_VADDR), %edx
	movl	%eax, (%ebx, %edx, 4)
#endif /* CFG_VBE */

#if CFG_MW == 1
# set PDT item 1016 for VBE in 4G-32M ~ 4G-28M
	movl	$(YI_KE_PGT_PADDR+PAGE_SIZE*5), %eax
	andl	$(YI_PAGE_ADDR_MASK), %eax	# get page address
	orl	$(YI_PAGE_PROP_PRESENT), %eax
	orl	$(YI_PAGE_PROP_WRITEABLE), %eax
	movl	$YI_PAGE_ADDR_TO_PDT(YI_KE_MW_SM_VADDR), %edx
	movl	%eax, (%ebx, %edx, 4)
#endif /* CFG_MW */

# set page table (PGT)
# map 0~4M to 0~4M
	movl	$0x0, %eax
	orl	$(YI_PAGE_PROP_PRESENT), %eax
	orl	$(YI_PAGE_PROP_WRITEABLE), %eax
	movl	$(YI_KE_PGT_PADDR), %ebx
	movl	$0x400, %ecx		# 1024 page in 4M
	movl	$0x0, %edx		# from first DWORD
fill_page0:
	movl	%eax, (%ebx, %edx, 4)
	addl	$PAGE_SIZE, %eax
	incl	%edx
	loop	fill_page0

# map 0~8M to 2G~2G+8M
	movl	$0x0, %eax
	orl	$(YI_PAGE_PROP_PRESENT), %eax
	orl	$(YI_PAGE_PROP_WRITEABLE), %eax
	movl	$(YI_KE_PGT_PADDR+PAGE_SIZE), %ebx
	movl	$0x800, %ecx		# 2048 page in 8M
	movl	$0x0, %edx		# from first DWORD
fill_page1to2:
	movl	%eax, (%ebx, %edx, 4)
	addl	$PAGE_SIZE, %eax
	incl	%edx
	loop	fill_page1to2


   

# map 4G-4k~4G to 1M~1M+4k for stack of loader
	movl	$(YI_KE_STACK_TOP_PADDR), %eax
	andl	$(YI_PAGE_ADDR_MASK), %eax # get page address
	orl	$(YI_PAGE_PROP_PRESENT), %eax
	orl	$(YI_PAGE_PROP_WRITEABLE), %eax
	movl	$(YI_KE_PGT_PADDR+PAGE_SIZE*3), %ebx
	movl	$YI_PAGE_ADDR_TO_PGT(YI_KE_STACK_TOP_VADDR), %edx
	movl	%eax, (%ebx, %edx, 4)

#if CFG_VBE == 1
# map vbe 4M buffer to high memory
    movl    YI_KE_VBE_PADDR+0x28, %eax      # get VBE PhysBasePtr
    andl    $(YI_PAGE_ADDR_MASK), %eax      # get page address
	orl     $(YI_PAGE_PROP_PRESENT), %eax
	orl     $(YI_PAGE_PROP_WRITEABLE), %eax
	movl	$(YI_KE_PGT_PADDR+PAGE_SIZE*4), %ebx
	movl	$YI_PAGE_ADDR_TO_PGT(YI_KE_VBE_VADDR), %edx
	movl	$0x400, %ecx		# 1024 page in 4M
fill_vbe_page0:
	movl	%eax, (%ebx, %edx, 4)
	addl	$PAGE_SIZE, %eax
	incl	%edx
	loop	fill_vbe_page0
#endif /* CFG_VBE */

#if CFG_MW == 1
# map microwindow stack 4M buffer to high memory
    movl    $(YI_KE_MW_SM_PADDR), %eax      # MW stack Phys address
    andl    $(YI_PAGE_ADDR_MASK), %eax      # get page address
	orl     $(YI_PAGE_PROP_PRESENT), %eax
	orl     $(YI_PAGE_PROP_WRITEABLE), %eax
	movl	$(YI_KE_PGT_PADDR+PAGE_SIZE*5), %ebx
	movl	$YI_PAGE_ADDR_TO_PGT(YI_KE_MW_SM_VADDR), %edx
	movl	$0x400, %ecx		# 1024 page in 4M
fill_mw_page0:
	movl	%eax, (%ebx, %edx, 4)
	addl	$PAGE_SIZE, %eax
	incl	%edx
	loop	fill_mw_page0
#endif /* CFG_MW */

# enter page mode.
	movl	$(YI_KE_PDT_PADDR), %eax
	andl	$(YI_PAGE_ADDR_MASK), %eax
	orl	$(YI_PAGE_PROP_PRESENT), %eax
	orl	$(YI_PAGE_PROP_WRITEABLE), %eax
	movl	%eax, %cr3

	movl	%cr0, %eax
	orl	$0x80000000, %eax
	movl	%eax, %cr0
	jmp	page_mode_pre
page_mode_pre:
	movl	$page_mode, %eax
	jmp	*%eax
page_mode:
	jmp	page_mode_pos
page_mode_pos:

# reset GDT
	lgdt	gdtv
# reset LDT
	lidt	idtv

	movw	$YI_HAL_KE_DS_SELC, %ax
	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %fs
	movw	%ax, %gs

# set stack of kernel
	movw	$YI_HAL_KE_DS_SELC, %ax	# set data selector
	movw	%ax, %ss
	movl	$(YI_KE_STACK_TOP_VADDR), %esp


# set null ISR
	movl	$null_int, %eax
	movw	%ax, null_idt_item
	shrl	$0x10, %eax
	movw	%ax, (null_idt_item+0x06)

	movl	null_idt_item, %eax
	movl	(null_idt_item+0x4), %edx

	movl	$(YI_P2V(YI_KE_IDT_PADDR)), %edi

	movl	$0xff, %ecx

fill_idt:
	movl	%eax, 0x0(%edi , %ecx, 8)
	movl	%edx, 0x4(%edi , %ecx, 8)
	loop	fill_idt

# umap 0~4M
	xorl	%eax, %eax
	movl	%eax, YI_P2V(YI_KE_PDT_PADDR)	# unmap 0~4M in PDT

	movl    $(YI_P2V(YI_KE_PGT_PADDR)), %ebx	# zero 0~4M PGT (no use)
	movl    $0xa0, %ecx         # 1024 page in 4M
	movl    $0x0, %edx
unmap_page0:
	movl    %eax, (%ebx, %edx, 4)
	incl    %edx
	loop    unmap_page0

# initialize 8259a chip

# set ICW1 mode.
	movb	$0x11, %al	# set ICW1
	outb	%al, $0x20

	call	delay

	outb	%al, $0xa0
	call	delay

# set ICW2 (map interrupt vector)
	movb	$0x40, %al
	outb	%al, $0x21
	call	delay

	movb	$0x48, %al
	outb	%al, $0xa1
	call	delay

# set ICW3 
	movb	$0x04, %al
	out	%al, $0x21
	call	delay

	movb	$0x02, %al
	outb	%al, $0xa1
	call	delay

# set ICW4	
	movb	$0x01, %al
	outb	%al, $0x21
	call	delay

	out	%al, $0xa1
	call	delay

# set OCW1
	movb	$0x00, %al
	outb	%al, $0x21
	call	delay

	movb	$0x00, %al
	out	%al, $0xa1
	call	delay

# memzero bss section
	movl	$yi_g_nLdBssStart, %ebx
	movl	$yi_g_nLdBssEnd, %ecx
	subl	%ebx, %ecx
	shrl	$0x2, %ecx
	xorl	%edx, %edx
	xorl	%eax, %eax
bbs_zero_loop:
	movl	%eax, (%ebx, %edx, 4)
	incl	%edx
	loop	bbs_zero_loop

	sti
# begin initialize system

	call 	KernelMain
	cli
	hlt


null_int:
	pushal

	movb	$0x20, %al
	outb	%al, $0x20		# int finish
	popal

	iret

delay:
	outb	%al, $0x80
	ret

gdtv:
        .word   0x0028
        .long   (YI_P2V(YI_KE_GDT_PADDR))

idtv:
	.word	0x07ff
	.long	(YI_P2V(YI_KE_IDT_PADDR))

null_idt_item:
	.word	0
	.word	YI_HAL_KE_CS_SELC
	.word	0x8E00
	.word	0

msg:
	.asciz "Hi, YingOS!\n\0"
msg2:
	.asciz "Int \n\0"

