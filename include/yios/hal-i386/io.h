
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
 * Filename:	$RCSfile: io.h,v $
 * Createdate:	2004/06/08 22:18:00
 * FileId:	$Id: io.h,v 1.1.1.1 2005/08/09 05:25:23 devin Exp $
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


#ifndef _YIOS_HAL_IO_H_
#define _YIOS_HAL_IO_H_

#include <yios/config.h>
#include <yios/types.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */

#define YIIoWritePortByte(data, port)	__YIIoWritePortByte(data, port)

static VOID __inline__ __YIIoWritePortByte(UINT8 nData, UINT16 nPort)
{	
	__asm__ __volatile__(
		"outb %0, %1\n\t"
		:
		:"a"(nData), "d"(nPort)
		);
}

#define YIIoReadPortByte(port)	__YIIoReadPortByte(port)

static UINT8 __inline__ __YIIoReadPortByte(UINT16 nPort)
{
	UINT8 nRet;
   	__asm__ __volatile__(
      		"inb %%dx, %%al	\n\t"
      		:"=a"(nRet)
      		:"d"(nPort)
	);
   return nRet;
}


#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _YIOS_HAL_IO_H_ */



