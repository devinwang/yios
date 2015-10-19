
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
 * Filename:	$RCSfile: stub16.S,v $
 * Createdate:	2004/06/14 09:52:00
 * FileId:	$Id: stub16.S,v 1.1.1.1 2005/08/09 05:25:24 devin Exp $
 * Last update:	$Date: 2005/08/09 05:25:24 $
 * Creator:	Devin Wang <vhd@21cn.com>
 * Version:	$Revision: 1.1.1.1 $
 * Description:  
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

.code32

.text
begin:
	nop
.align 16
stub_idtv:

stub_gdtv:

.align 16
stub_32to16:
bios_int_call:
	movw	%ax, %fs	# save ax
	xorl	%eax, %eax

	pushfl			# save flags
	cli	

	movw	%cs, %ax
	pushl	%eax
	
	movw	%ds, %ax
	pushl	%eax
	
	movw	%es, %ax
	pushl	%eax
	
	movw	%ss, %ax
	pushl	%eax

	movw	%fs, %ax
	pushl	%eax

	pushal
	
	

stub_16to32:
	cli

	
