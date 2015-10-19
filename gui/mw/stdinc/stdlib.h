
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


#ifndef _YIOS_MW_STDLIB_H_
#define _YIOS_MW_STDLIB_H_

#include <yios/config.h>
#include <yios/types.h>

#include <mwcfg.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */


int abs(int j);

void *malloc(size_t size);
void free(void *ptr);

void *realloc(void *ptr, size_t size);
void *calloc(size_t nmemb, size_t size);
# define alloca(size)   __builtin_alloca (size)

void qsort (void  *base,
            size_t nel,
            size_t width,
            int (*comp)(const void *, const void *));

void exit(int status);

long int random(void);

#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _YIOS_MW_STDLIB_H_ */

