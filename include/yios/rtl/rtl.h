
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
 * Filename:	$RCSfile: rtl.h,v $
 * Createdate:	2004/07/08 19:16:00
 * FileId:	$Id: rtl.h,v 1.1.1.1 2005/08/09 05:25:23 devin Exp $
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

#ifndef _YIOS_RTL_RTL_H_
#define _YIOS_RTL_RTL_H_

#include <yios/config.h>
#include <yios/types.h>

typedef char *va_list;
#define  _AUPBND         (sizeof (unsigned int) - 1)
#define  _ADNBND         (sizeof (unsigned int) - 1)

#define _bnd(X, bnd)    (((sizeof (X)) + (bnd)) & (~(bnd)))
#define va_arg(ap, T)   (*(T *)(((ap) += (_bnd (T, _AUPBND))) - (_bnd (T,_ADNBND))))
#define va_end(ap)      (void) 0
#define va_start(ap, A) (void) ((ap) = (((char *) &(A)) + (_bnd (A,_AUPBND))))


extern int vsnprintf(char *buf, SIZE_T size, const char *fmt, va_list args);

#define vsprintf(buf, fmt, args)  vsnprintf((buf), (SIZE_T)-1, (fmt), (args))


#define YIRtlMemoryZero(dest, size) YIRtlMemorySet(dest, 0, size)


EXTERN SIZE_T YICALL YIRtlStringLengthN(PCSTR pString, SIZE_T nLimit);
EXTERN INT YICALL YIRtlStringCompare(PCSTR pStr1, PCSTR pStr2);

EXTERN PVOID YICALL YIRtlMemorySet(PVOID pDest, INT nSet, SIZE_T nSize); 
EXTERN PVOID YICALL YIRtlMemoryCopy(PVOID pDest, PVOID pSrc, SIZE_T nSize);

EXTERN UINT CDECLCALL YIRtlKernelMsg(PCSTR fmt, ...);

#endif /* _YIOS_RTL_RTL_H_ */

