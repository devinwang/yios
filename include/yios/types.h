
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
 * Filename:	$RCSfile: types.h,v $
 * Createdate:	2004/05/31 15:32:00
 * FileId:	$Id: types.h,v 1.1.1.1 2005/08/09 05:25:23 devin Exp $
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


#ifndef _YIOS_TYPES_H_
#define _YIOS_TYPES_H_

#include <yios/hal/types.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */

typedef __UINT8		UINT8;
typedef __UINT16	UINT16;
typedef __UINT32	UINT32;
typedef __UINT64	UINT64;

typedef __INT8		INT8;
typedef __INT16		INT16;
typedef __INT32		INT32;
typedef __INT64		INT64;

typedef __FP32		FP32;
typedef __FP64		FP64;

typedef __UCHAR		UCHAR;
typedef __CHAR		CHAR;
typedef __PCHAR		PCHAR;
typedef __PSTR		PSTR;
typedef __PCSTR		PCSTR;

typedef __VOID		VOID;

typedef __PUINT8	PUINT8;
typedef __PUINT16	PUINT16;
typedef __PUINT32	PUINT32;
typedef __PUINT64	PUINT64;


typedef __PINT8		PINT8;
typedef __PINT16	PINT16;
typedef __PINT32	PINT32;
typedef __PINT64	PINT64;


typedef __PVOID		PVOID;

typedef	__PFP32		PFP32;
typedef __PFP64		PFP64;


/* 32bit platform integer (platform dependent integer) */
typedef	__INT		INT;
typedef __PINT		PINT;
typedef __UINT		UINT;
typedef __PUINT		PUINT;


typedef __FLAGS_T   FLAGS_T;


typedef __PID_T		PID_T;	/* process id type. */
typedef __TID_T		TID_T;	/* thread id type. */
typedef __GID_T		GID_T;  /* group id type. */
typedef __UID_T		UID_T;	/* user id type. */
typedef __DEV_T		DEV_T;	/* user id type. */
typedef __INO_T		INO_T;  /* inode id type. */
typedef __MODE_T	MODE_T;
typedef __CADDR_T	CADDR_T;
typedef __NLINK_T	NLINK_T;	/* number of hard link. */


typedef __TIME_T	TIME_T;
typedef __SIZE_T	SIZE_T;
typedef __OFF_T		OFF_T;

typedef __NRADDR	NRADDR;
typedef __PADDR		PADDR;

typedef __YI_STATUS	YI_STATUS;


#define UINT_SIZE	__UINT_SIZE
#define ARCH_SIZE	__ARCH_SIZE


#define ALIGN_UP(__x__, __align__)	(((__x__) + (__align__) - 1) & ~((__align__) - 1))
#define ALIGN_DN(__x__, __align__)	((__x__) & (~((__align__) - 1)))



#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _YIOS_TYPES_H_ */



