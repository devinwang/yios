
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
 * Filename:	$RCSfile: list.h,v $
 * Createdate:	2005/01/05 20:41:00
 * FileId:	$Id: list.h,v 1.1.1.1 2005/08/09 05:25:23 devin Exp $
 * Last update:	$Date: 2006/03/19 14:02:00 $
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


#ifndef _YIOS_MW_STRING_H_
#define _YIOS_MW_STRING_H_

#include <yios/config.h>
#include <yios/types.h>

#include <mwcfg.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */

char *strncpy(char *s1, const char *s2, size_t n);

#define strcmpi strcasecmp
int strcasecmp(const char *s1,const char * s2);

/* size_t strlen(const char *s);*/

#define strlen  __builtin_strlen
#define strcmp  __builtin_strcmp
#define strcpy  __builtin_strcpy
#define strcat  __builtin_strcat

void *memcpy(void *dest, const void *src, size_t n);

void *memset(void *s, int c, size_t n);

#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _YIOS_MW_STRING_H_ */



