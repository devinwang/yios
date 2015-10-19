/* ====================================================================
 * YingOS project.
 * Copyright (C) 2005 Devin Wang

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

 * Please read this project's documents about the detail of myOS's source.
 *
 * ====================================================================
 *
 * Filename:	$RCSfile: vesa16.S,v $
 * Createdate:	2005/02/16 16:38:00
 * FileId:	$Id: vesa16.S,v 1.1.1.1 2005/08/09 05:25:24 devin Exp $
 * Last update:	$Date: 2005/08/09 05:25:24 $
 * Creator:	Deng Wang <vhd@21cn.com>
 * Version:	$Revision: 1.1.1.1 $
 * Description:  VESA VBE support.
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


#define VESASEG     YI_KE_VBE_SEG

#define OFF_MEMMODE  0x27

.code16

.global	vesa_init

vesa_init:
# get VBE controller imformation.
	movw	$VESASEG, %ax
	movw	%ax, %es
	xorw	%di, %di
	movw	$0x4f00, %ax
	int	$0x10
	call	vesa_ret_status

# get mode information.
	movw	$VESASEG, %ax
	movw	%ax, %es
	xorw	%di, %di
	movw	$0x4f01, %ax
	movw	$CFG_VBE_MODE, %cx
	int	$0x10
    cmpb    $0x4f, %al      # not support?
    jne     mode_0x140
    cmpb    $0x00, %ah      # call failed?
    jne     mode_0x140

# set Direct color mode.
    movb    $0x06, %al
    movb    %al, %es:OFF_MEMMODE

# set current mode to CFG_VBE_MODE
	movw	$VESASEG, %ax
	movw	%ax, %es
	xorw	%di, %di
	movw	$0x4f02, %ax
	movw	$(0x4000|CFG_VBE_MODE), %bx
	int	$0x10		# set VBE mode.
	call	vesa_ret_status
    ret

# try mode 0x140 (800*600*32)
mode_0x140:
	# get mode information.
	movw	$VESASEG, %ax
	movw	%ax, %es
	xorw	%di, %di
	movw	$0x4f01, %ax
	movw	$0x140, %cx
	int	$0x10
    cmpb    $0x4f, %al      # not support?
    jne     mode_0x140
    cmpb    $0x00, %ah      # call failed?
    jne     mode_0x140

# set current mode to CFG_VBE_MODE
	movw	$VESASEG, %ax
	movw	%ax, %es
	xorw	%di, %di
	movw	$0x4f02, %ax
	movw	$(0x4000|0x140), %bx
	int	$0x10		# set VBE mode.
	call	vesa_ret_status
    ret

vesa_ret_status:
	pusha
	cmpb	$0x4f, %al
	jne	1f
	cmpb	$0x00, %ah
	jne	2f
	xorw	%ax, %ax
	jmp	3f
1:
	movw	$YI_OSKERNELSYSSEG, %ax	# show on support.
	movw	%ax, %es
	movw	$0x1301, %ax
	movw	$no_support_msg, %bp
	movw	$31, %cx
	movw	$0x001f, %bx
	xorw	%dx, %dx
	int	$0x10
	cli
	hlt
2:
	movw	$YI_OSKERNELSYSSEG, %ax	# show call failt.
	movw	%ax, %es
	movw	$0x1301, %ax
	movw	$call_failed_msg, %bp
	movw	$26, %cx
	movw	$0x001f, %bx
	xorw	%dx, %dx
	int	$0x10
	cli
	hlt
3:
	popa
	ret

	
no_support_msg:
	.string	"VESA function is not supported!"
call_failed_msg:
	.string	"VESA function call failed!"

