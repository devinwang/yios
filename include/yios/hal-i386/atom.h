
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
 * Filename:	$RCSfile: atom.h,v $
 * Createdate:  2004/06/17 00:40:00
 * FileId:      $Id: atom.h,v 1.1.1.1 2005/08/09 05:25:23 devin Exp $
 * Last update: $Date: 2005/08/09 05:25:23 $
 * Creator:     Devin Wang <vhd@21cn.com>
 * Version:     $Revision: 1.1.1.1 $
 * Description:  Atomic operation function come from linux.
 *
 * ====================================================================
 */

/* ====================================================================
 *  Revision:
 *
 * ====================================================================
 */


#ifndef _YIOS_HAL_ATOM_H_
#define _YIOS_HAL_ATOM_H_

#include <yios/config.h>
#include <yios/types.h>

#include <yios/hal/segset.h>
/* #include <yios/thread.h>*/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */

typedef struct _YI_ATOM{
    VOLATILE    INT nAtom;
}YI_ATOM, *YI_PATOM;

STATIC INLINE VOID YIHalAtomInit(YI_PATOM pAtom)
{

}

STATIC INLINE VOID YIHalAtomAssign(YI_PATOM pAtom, INT n)
{
    pAtom->nAtom = n;
}

STATIC INLINE INT YIHalAtomRead(YI_PATOM pAtom)
{
    return pAtom->nAtom;
}

STATIC INLINE VOID YIHalAtomInc(YI_PATOM pAtom)
{
    __asm__ __volatile__(
        "incl %0"
        :"=m"(pAtom->nAtom)
        :"m"(pAtom->nAtom)
    );
}

STATIC INLINE VOID YIHalAtomDec(YI_PATOM pAtom)
{
    __asm__ __volatile__(
        "decl %0"
        :"=m"(pAtom->nAtom)
        :"m"(pAtom->nAtom)
    );
}

STATIC INLINE VOID YIHalAtomAdd(YI_PATOM pAtom, INT n)
{
    __asm__ __volatile__(
        "addl %1, %0"
        :"=m"(pAtom->nAtom)
        :"ir"(n), "m" (pAtom->nAtom)
    );
}

STATIC INLINE VOID YIHalAtomSub(YI_PATOM pAtom, INT n)
{
    __asm__ __volatile__(
        "subl %1, %0"
        :"=m"(pAtom->nAtom)
        :"ir"(n), "m"(pAtom->nAtom)
    );
}

#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _YIOS_HAL_ATOM_H_ */

