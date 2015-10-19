
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

 * Please read this project's documents about the detail of myOS's source.
 *
 * ====================================================================
 *
 * Filename:	$RCSfile: segset.h,v $
 * Createdate:	2005/09/21 12:40:00
 * FileId:	$Id: segset.h,v 1.1.1.1 2005/08/09 05:25:23 devin Exp $
 * Last update:	$Date: 2005/08/09 05:25:23 $
 * Creator:	Devin Wang <vhd@21cn.com>
 * Version:	$Revision: 1.1.1.1 $
 * Description: bit opration come from linux.
 *
 * ====================================================================
 */

/* ====================================================================
 *  Revision:
 *
 * ====================================================================
 */

#ifndef _YIOS_HAL_BITOPT_H_
#define _YIOS_HAL_BITOPT_H_

/* Set bit and return old bit. */
STATIC INLINE INT YITestSetBit(VOLATILE NRADDR* pAddr, INT nPos)
{
    INT nOldPos;

    __asm__ __volatile__(
        "btsl %2, %1\n\t"
        "sbbl %0, %0\n\t"
        :"=r"(nOldPos), "=m"(*(VOLATILE NRADDR*)pAddr)
        :"Ir"(nPos): "memory"
    );

    return nOldPos;
}

/* clear bit and return old bit. */
STATIC INLINE INT YITestClearBit(VOLATILE NRADDR* pAddr, INT nPos)
{
    INT nOldPos;

    __asm__ __volatile__(
        "btrl %2, %1\n\t"
        "sbbl %0, %0\n\t"
        :"=r"(nOldPos), "=m"(*(VOLATILE NRADDR*)pAddr)
        :"Ir"(nPos): "memory"
    );

    return nOldPos;
}


#endif /* _YIOS_HAL_BITOPT_H_ */

