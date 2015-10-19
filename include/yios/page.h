
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
 * Filename:	$RCSfile: page.h,v $
 * Createdate:	2005/08/04 21:29:00
 * FileId:      $Id: page.h,v 1.1.1.1 2005/08/09 05:25:23 devin Exp $
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


#ifndef _YIOS_PAGE_H_
#define _YIOS_PAGE_H_

#include <yios/config.h>
#include <yios/types.h>
#include <yios/list.h>

#include <yios/mmzone.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */

#ifndef YM_PPAGE
typedef struct _YM_PAGE* YM_PPAGE;
#endif /* YM_PPAGE */

typedef struct _YM_PAGE{
	LIST_HEAD	ltNext;

	PADDR		pVirtual;
/*	YM_PMEMORY_ZONE	pZone;*/
}YM_PAGE;

#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _YIOS_PAGE_H_ */

