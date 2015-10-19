
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
 * Filename:	$RCSfile: string.c,v $
 * Createdate:	2004/07/08 18:09:00
 * FileId:	$Id: string.c,v 1.1.1.1 2005/08/09 05:25:25 devin Exp $
 * Last update:	$Date: 2005/08/09 05:25:25 $
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
#include <yios/types.h>

#include <yios/rtl/rtl.h>

EXTERN SIZE_T YICALL YIRtlStringLengthN(PCSTR pString, SIZE_T nLimit)
{
	SIZE_T nLen = 0;
	while(pString[nLen] != '\0' && nLimit>nLen)
		nLen++;

	return nLen;
}

EXTERN INT YICALL YIRtlStringCompare(PCSTR pStr1, PCSTR pStr2)
{
	while(*pStr1 != '\0' && *pStr1 == *pStr2)
	{
		pStr1++;
		pStr2++;
	}

	
	return (INT)(*(UCHAR*)pStr1 - *(UCHAR*)pStr2);
}

EXTERN PVOID YICALL YIRtlMemorySet(PVOID pDest, INT nSet, SIZE_T nSize)
{
	while(nSize)
	{
		*((PUINT8)pDest) = (UINT8)nSet;
		pDest ++;
		nSize --;
	}
	return pDest;
}

EXTERN PVOID YICALL YIRtlMemoryCopy(PVOID pDest, PVOID pSrc, SIZE_T nSize)
{
	while(nSize)
	{
		*(PUINT8)pDest = *(PUINT8)pSrc;
        pDest++;
        pSrc++;
		nSize --;
	}
	return pDest;
}


