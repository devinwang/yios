
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
 *
 * ====================================================================
 *
 * Filename:	$RCSfile: int.h,v $
 * Createdate:	2004/06/08 22:18:00
 * FileId:	$Id: int.h,v 1.1.1.1 2005/08/09 05:25:23 devin Exp $
 * Last update:	$Date: 2005/08/09 05:25:23 $
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


#ifndef _YIOS_HAL_INT_H_
#define _YIOS_HAL_INT_H_

#include <yios/config.h>
#include <yios/types.h>

#include <yios/hal/cpu.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */


#define YI_HAL_INT_MAX_NUM  		0xff
#define YI_HAL_INT_CHANNEL_START    0x40
#define YI_HAL_INT_CHANNEL_END      0x60

#define YI_HAL_INT_CHANNEL_TIME     0
#define YI_HAL_INT_CHANNEL_KEYBD    1
#define YI_HAL_INT_CHANNEL_CASCDE   2
#define YI_HAL_INT_CHANNEL_COM2     3
#define YI_HAL_INT_CHANNEL_COM1     4
#define YI_HAL_INT_CHANNEL_PS2MOU   12
#define YI_HAL_INT_CHANNEL_IDE0     14
#define YI_HAL_INT_CHANNEL_IDE1     15

#define YI_HAL_INT_VECTOR_DIVIDE    0x00
#define YI_HAL_INT_VECTOR_NMI       0x02
#define YI_HAL_INT_VECTOR_DBG       0x03
#define YI_HAL_INT_VECTOR_OVERFL    0x04
#define YI_HAL_INT_VECTOR_BUDEXC    0x05
#define YI_HAL_INT_VECTOR_INVCODE   0x06
#define YI_HAL_INT_VECTOR_DEVINV    0x07
#define YI_HAL_INT_VECTOR_DOUFUT    0x08
#define YI_HAL_INT_VECTOR_COPSEG    0x09
#define YI_HAL_INT_VECTOR_INVTSS    0x0a
#define YI_HAL_INT_VECTOR_INVSEG    0x0b
#define YI_HAL_INT_VECTOR_INVSS     0x0c
#define YI_HAL_INT_VECTOR_GPFUT     0x0d
#define YI_HAL_INT_VECTOR_PGFUT     0x0e
#define YI_HAL_INT_VECTOR_FPFUT     0x10

#define YI_HAL_INT_VECTOR_SYSCALL   0x80

#define YI_HAL_INT_SAVE_ALL_REGS		\
	"pushl %ds\n\t"			\
	"pushl %es\n\t"			\
	"pushl %eax\n\t"		\
	"pushl %ebx\n\t"		\
	"pushl %ecx\n\t"		\
	"pushl %edx\n\t"		\
	"pushl %esi\n\t"		\
	"pushl %edi\n\t"		\
	"pushl %ebp\n\t"

#define YI_HAL_INT_RESTORE_ALL_REGS	\
	"popl %ebp\n\t"			\
	"popl %edi\n\t"			\
	"popl %esi\n\t"			\
	"popl %edx\n\t"			\
	"popl %ecx\n\t"			\
	"popl %ebx\n\t"			\
	"popl %eax\n\t"			\
	"popl %es\n\t"			\
	"popl %ds\n\t"



#define YI_HAL_INT_FINISH_INT     \
	do{\
		__asm__ __volatile__(\
		"movb $0x20, %al\n\t"\
		"outb %al, $0x20\n\t"\
		"movb $0x20, %al\n\t"\
		"outb %al, $0xa0\n\t"\
	);}while(0)


/* Set i386 IDT item.
   Note: pFu isn't real function, is interrute handle address. 
    nDpl is only assigned 0 or 3.
*/
EXTERN VOID YIHalSetIntTblItem(INT iIntNum, PVOID pFn, UINT nType, UINT nDpl);


EXTERN YI_STATUS  YIHalIntInitialize(VOID);

#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _YIOS_HAL_INT_H_ */



