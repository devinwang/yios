
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
 * Filename:	$RCSfile: dbg.h,v $
 * Createdate:	2005/02/09 21:51:00
 * FileId:	$Id: dbg.h,v 1.1.1.1 2005/08/09 05:25:23 devin Exp $
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


#ifndef _YIOS_DBG_H_
#define _YIOS_DBG_H_

#include <yios/config.h>
#include <yios/types.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */

#ifdef __FUNCTION__
#define BUG()	\
	YIRtlKernelMsg("\n<BUG>: n:"__FILE__" f:"__FUNCTION__" l:%d\n", __LINE__)
#else
#define BUG()	\
	YIRtlKernelMsg("\n<BUG>: File:"__FILE__"\nFunction:%s\nLine:%d\n", __FUNCTION__, __LINE__)
#endif /* __FUNCTION__ */

#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _YIOS_DBG_H_ */



