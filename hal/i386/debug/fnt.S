	
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
 * ====================================================================
 *
 * Filename:	$RCSfile: fnt.S,v $
 * Createdate:	2005/02/16 22:15:00
 * FileId:	$Id: fnt.S,v 1.1.1.1 2005/08/09 05:25:24 devin Exp $
 * Last update:	$Date: 2005/08/09 05:25:24 $
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

#include <config.h>
/*
#include <yios/hal/init.h>
*/
.code32

.data

.globl yi_g_Fnt8x16
yi_g_Fnt8x16:
    .incbin "asc16.fnt"
#if CFG_LANG_GB2312 == 1
.globl yi_g_HzFnt16x16
yi_g_HzFnt16x16:
    .incbin "hzk16.fnt"
.globl yi_g_szNovel
yi_g_szNovel:
    .byte '\n'
    .incbin "novel.txt"
#endif /* CFG_LANG_GB2312 */
