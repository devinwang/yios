	
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
 * Filename:	$RCSfile: boot.S,v $
 * Createdate:	2004/04/19 16:19:00
 * FileId:	$Id: boot.S,v 1.1.1.1 2005/08/09 05:25:23 devin Exp $
 * Last update:	$Date: 2005/08/09 05:25:23 $
 * Creator:	Devin Wang <vhd@21cn.com>
 * Version:	$Revision: 1.1.1.1 $
 * Description: find oskernel.sys and load it into memory.
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


#define DISKDATASEG	0x9020
#define DISKDATAOFFSET	0x0000
#define DISKFATSEG	0x9040
#define DISKFATOFFSET	0x0000



.code16
.text


.global _start
_start:
	jmp	begin
	nop
fact_string:
	.ascii	"YiOS0.0\0"
disk_parm:
	.word	0x0200
	.byte	0x01
	.word	0x0001
	.byte	0x02
	.word	0x00e0
	.word	0x0b40
	.byte	0xf0
	.word	0x0009
	.word	0x0012
	.word	0x0002
	.long	0x0000
	.long	0x0000

.org	0x3e
begin:
	cli
	cld
	movw	$YI_BOOTSEG, %ax
	movw	%ax, %ds
	movw	$YI_BOOTHISEG, %ax
	movw	%ax, %es
	movw	$0x100, %cx
	xorw	%si, %si
	xorw	%di, %di
	rep
	movsw
	ljmp	$YI_BOOTHISEG, $rebegin

rebegin:
	movw	%ax, %ds
	movw	%ax, %ss
	movw	$YI_BOOTHISTKOF, %sp
	sti

# show loading...
	movw	$msg_loading, %ax
	movw	$7, %cx	
	call	show_msg

/*
# set display screen.
	movw	$0x0003, %ax
	int	$0x10		# change display mode.

	movw	$0x0500, %ax	# set current display page.
	int	$0x10

	movw	$0x0200, %ax	# set curs position.
	xorw	%dx, %dx
	xorw	%bx, %bx
	int	$0x10*/
/*
	movw	$2000, %cx	# set backgroud color.
	movw	$0x0920, %ax
	movw	$0x0011, %bx
	int	$0x10
*/

# calculate the disk parameters.
	movw	disk_parm+0x03, %ax		# save fat table position
	incw	%ax
	movw	%ax, fat_pos

	xorw	%cx, %cx
	movb	disk_parm+0x05, %cl
	movw	disk_parm+0x0b, %ax
	mulw	%cx				# get fat size

	addw	disk_parm+0x03, %ax		# plus boot sector size
	incw	%ax				# from 1 not 0
	movw	%ax, root_dir_pos		# save root directory position
	movw	%ax, data_sector_pos		# save data sector begin position (part one)
	
	xorw	%ax, %ax			# reset floppy disk
	xorw	%dx, %dx	
	int	$0x13

# find oskernel.sys
	movw	disk_parm+0x06, %ax
	movb	$0x10, %bl
	divb	%bl		# get root directory sector size


	xorb	%ah, %ah
	movw	%ax, %cx	# save root directory sector number.

	addw	%ax, data_sector_pos		# save data sector begin position (part two)

	movw	$oskernel_filename, %dx
	movw	%dx, %si

	movw	$DISKDATASEG, %dx
	movw	%dx, %es

begin_find:
	movw	root_dir_pos, %ax
	call	read_sector
	
	movw	$DISKDATAOFFSET, %dx
	movw	%dx, %di
	
	pushw	%cx
	movw	$0x10, %cx	# directory number per sector

find_per_item:
	call	cmp_filename
	jnz	oskernel_found
	
	addw	$0x20, %di
	decw	%cx
	jnz	find_per_item
	
	incw	root_dir_pos
	popw	%cx	
	decw	%cx
	jnz	begin_find

no_found:
	movw	$msg_nofind, %ax
	movw	$19, %cx	
	call	show_msg
	cli
	hlt

oskernel_found:
	addw	$0x2, %sp	# delete %cx in stack
# Now es:di is a point to osloader.sys directory item.
	addw	$0x1a, %di
	movw	%es:(%di), %ax	# get file sector in FAT
	pushw	%ax
# loader FAT table into memory
	movw	$DISKDATAOFFSET, %bx
	movw	%bx, %si
	movw	$DISKFATOFFSET, %bx
	movw	%bx, %di
	movw	fat_pos, %ax
	movw	disk_parm+0x0b, %cx
load_fat:
	call	read_sector
	pushw	%ds
	pushw	%es
	pushw	%cx

	movw	$DISKFATSEG, %bx
	movw	%bx, %es

	movw	$DISKDATASEG, %bx
	movw	%bx, %ds

	xorw	%si, %si

	movw	$0x100, %cx

	rep
	movsw

	popw	%cx
	popw	%es
	popw	%ds
	incw	%ax
	loop	load_fat

# load OSKERNEL.SYS into memory
	movw	$DISKFATSEG, %bx
	movw	%bx, %es
	movw	$YI_OSKERNELSYSSEG, %dx
	popw	%ax
	movw	%ax, %bx
	testw	$0x0001, %ax
	jz	r_even
r_odd:
	shlw	$4, %ax
r_even:

load_oskernel_sys:
	testw	$0x0001, %bx
	jz	even
odd:
	shrw	$4, %ax
even:
	andw	$0x0fff, %ax

	cmpw	$0x0fff, %ax
	je	load_oskernel_over

	push	%ax

	decw	%ax		# load current sector
	decw	%ax
	addw	data_sector_pos, %ax
	call	read_sector
# show process
#    movw    
#    int     $0x10

    pushw   %ax
    pushw   %cx
 	movw	$msg_dot, %ax
	movw	$1, %cx	
	call	show_msg
    popw    %cx
    popw    %ax


	pushw	%ds
	pushw	%es

	movw	%dx, %es
	addw	$0x0020, %dx	

	xorw	%di, %di

	movw	$DISKDATAOFFSET, %ax
	movw	%ax, %si

	movw	$DISKDATASEG, %ax
	movw	%ax, %ds

	movw	$0x100, %cx

	rep
	movsw

	popw	%es
	popw	%ds
	
	popw	%ax		# calculate next fat item
	subw	$0x2, %sp
	
	pushw	%dx
	
	movw	$0x03, %cx
	mulw	%cx
	movw	$0x02, %cx
	divw	%cx
	
	popw	%dx
	
	movw	%ax, %bx
	movw	%es:(%bx), %ax
	
	popw	%bx
	jmp	load_oskernel_sys
load_oskernel_over:
    xorw    %ax, %ax
    xorw    %dx, %dx
    int $0x13       # reset floppy

    movw    $0x3f2, %dx # shut down floppy motor.
    xorw    %ax, %ax
    outb    %al, %dx

	ljmp	$YI_OSKERNELSYSSEG, $0x0000


# show some message.
# ds:ax: message address
# cx:	string length
show_msg:
	pushaw

    movw    %ax, %bx
show_loop:
    movb    (%bx), %al
    movb    $0x0e, %ah
    int     $0x10
    incw    %bx
    loop    show_loop

	popaw
	ret

# read a physical sector into memory
# dl:	what disk? 0--disk A 80--hard disk
# dh:	what head
# cl:	sector number
# ch:	track number
# es:bx buffer
read_fsector:
	movw	$0x0201, %ax	# read 1 sector
	int	$0x13
	ret

# read a locgical sector into memory
# ax:	logical sector
read_sector:
	pushaw
	pushw	%es
	pushw	%ax
	
	decw	%ax
	divb	disk_parm+0x0d
	incb	%ah
	movb	%ah, %cl	# get physical sector

	movb	%al, %dh
	andb	$0x01, %dh	# get head

	xorb	%ah, %ah
	divb	disk_parm+0xf
	movb	%al, %ch	# get track number
	
	xorb	%dl, %dl	# floppy disk A

	movw	$DISKDATASEG, %ax	# set buffer
	movw	%ax, %es
	movw	$DISKDATAOFFSET, %bx

	call	read_fsector
	
	addw	$0x2, %sp
	popw	%es
	popaw	
	ret
	

# compare two file name
# ds:si	first name
# es:di second name
cmp_filename:
	pushaw
	movw	$(8+3), %cx	# 8+3 name length
begin_cmp:
	movb	(%si), %al
	cmpb	%al, %es:(%di)
	jne	no_equ
	incw	%si
	incw	%di
	loop	begin_cmp
	popaw
	movw	$0x01, %ax
	ret
no_equ:
	popaw
	xorw	%ax, %ax
	ret


fat_pos:
	.word	0x0000
root_dir_pos:
	.word	0x0000
data_sector_pos:
	.word	0x0000

msg_loading:
    .ascii  "Loading"

msg_dot:
    .ascii ">"

msg_nofind:
oskernel_filename:
	.ascii	"OSKERNELSYS"
msg_nofind_part:
	.ascii	" miss"


.org 510
boot_flag:	.word	0xAA55


