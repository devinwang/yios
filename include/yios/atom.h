
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
 * Createdate:	2005/02/12 11:08:00
 * FileId:	$Id: atom.h,v 1.1.1.1 2005/08/09 05:25:23 devin Exp $
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


#ifndef _YIOS_ATOM_H_
#define _YIOS_ATOM_H_

#include <yios/config.h>
#include <yios/types.h>

#include <yios/hal/atom.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */

#define YIKeAtomInit(__atom)              YIHalAtomInit((__atom))
#define YIKeAtomAssign(__atom, __var)     YIHalAtomAssign((__atom), (__var))
#define YIKeAtomRead(__atom)              YIHalAtomRead((__atom))

#define YIKeAtomInc(__atom)               YIHalAtomInc((__atom))
#define YIKeAtomDec(__atom)               YIHalAtomDec((__atom))
#define YIKeAtomAdd(__atom)               YIHalAtomAdd((__atom), (__var))
#define YIKeAtomSub(__atom)               YIHalAtomSub((__atom), (__var))

#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _YIOS_ATOM_H_ */

