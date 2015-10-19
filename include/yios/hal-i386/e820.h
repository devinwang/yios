
/* ====================================================================
 * The YingOS project.
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
 *
 * ====================================================================
 *
 * Filename:	$RCSfile: memory.h,v $
 * Createdate:	2005/08/19 23:19:00
 * FileId:      $Id: memory.h,v 1.1.1.1 2005/08/09 05:25:23 devin Exp $
 * Last update:	$Date: 2005/08/09 05:25:23 $
 * Creator:     Devin Wang <vhd@21cn.com>
 * Version:     $Revision: 1.1.1.1 $
 * Description: 
 *
 * ====================================================================
 */

/* ====================================================================
 *  Revision:
 *
 * ====================================================================
 */


#ifndef _YIOS_HAL_E820_H_
#define _YIOS_HAL_E820_H_

#include <yios/config.h>
#include <yios/types.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */


#define YM_E820NR_MAX	32

/* e820 memory type. */
#define YM_E820T_RAM		1
#define YM_E820T_RESERVED	2
#define YM_E820T_ACPI		3
#define YM_E820T_ROM		4

#ifndef __ASSEMBLY__

typedef struct _YM_E820AREA_{
    UINT64  nStart;
    UINT64  nSize;
    UINT32  nType;
}YM_E820AREA;


typedef struct _YM_E820MAP_{
    UINT        nMapNum;
    YM_E820AREA nMaps[YM_E820NR_MAX];
}YM_E820MAP;

#endif /* __ASSEMBLY__ */

#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _YIOS_HAL_E820_H_ */

