
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
 * Filename:	$RCSfile: driver.h,v $
 * Createdate:	2005/01/05 09:05:00
 * FileId:	$Id: driver.h,v 1.1.1.1 2005/08/09 05:25:23 devin Exp $
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


#ifndef _YIOS_DRIVER_H_
#define _YIOS_DRIVER_H_

#include <yios/types.h>
#include <yios/file.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */

/* all driver use this struct to describe itself. */
typedef struct __YI_DRIVER{
  YI_FS_VFS_STRUCT	stFileOp;	/* driver operation method. */
  INO_T	       		nInode;
}YI_DRIVER, *YI_PDRIVER;


#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _YIOS_DRIVER_H_ */


