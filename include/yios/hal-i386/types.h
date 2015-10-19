
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
 * Createdate:	2004/05/31 15:49:00
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


#ifndef _YIOS_HAL_TYPES_H_
#define _YIOS_HAL_TYPES_H_

#include <yios/config.h>
#include <yios/hal/compiler.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */


#ifndef NULL            
#  ifdef __cplusplus    
#    define NULL        0
#  else
#    define NULL        ((void*)0)
#  endif /* __cplusplus */      
#endif /* NULL */               

#ifndef FALSE
#define FALSE           0
#endif

#ifndef TRUE
#define TRUE            1
#endif

#define CONST           const

#define EXTERN		extern
#define STATIC		static

typedef unsigned char		__UINT8;
typedef signed char		__INT8;

typedef unsigned short int	__UINT16;
typedef	signed short 		__INT16;

typedef unsigned int		__UINT32;
typedef signed int		__INT32;

typedef unsigned long long int	__UINT64;
typedef signed long long int	__INT64;

typedef void			__VOID;

typedef float			__FP32;

typedef double			__FP64;

typedef unsigned char		__UCHAR;
typedef char			__CHAR;

typedef __UINT8*	__PUINT8;
typedef __UINT16*	__PUINT16;
typedef __UINT32*	__PUINT32;
typedef __UINT64*	__PUINT64;

typedef __INT8*		__PINT8;
typedef __INT16*	__PINT16;
typedef __INT32*	__PINT32;
typedef __INT64*	__PINT64;

typedef __VOID*		__PVOID;

typedef __FP32*		__PFP32;

typedef __FP64*		__PFP64;

typedef __CHAR		*__PCHAR;
typedef __CHAR		*__PSTR;
typedef CONST __CHAR	*__PCSTR;

/* 32bit platform integer (platform dependent integer) */
typedef __UINT32	__UINT;
typedef __PUINT32	__PUINT;
typedef	__INT32		__INT;
typedef __PINT32	__PINT;

typedef long		__TIME_T;
typedef __UINT32	__SIZE_T;
typedef long		__OFF_T;

typedef unsigned long           __FLAGS_T;

typedef __volatile__ long		__ATOM_T;

typedef int		__PID_T;  /* process id type. */
typedef int		__TID_T;  /* thread id type. */
typedef int		__GID_T;  /* group id type. */
typedef int		__UID_T;  /* user id type. */
typedef int		__DEV_T;  /* user id type. */
typedef int		__INO_T;
typedef int		__MODE_T; 
typedef int		__CADDR_T;
typedef unsigned short	__NLINK_T;


typedef unsigned long	__NRADDR;
typedef void*		__PADDR;


typedef __UINT		__YI_STATUS;

/*
typedef struct __YI_STATUS_STRUCT_{
			
}__YI_STATUS_STRUCT;

typedef union __YI_STATUS_{
	__UINT			nCode;
	__YI_STATUS_STRUCT	stStatus;
}__YI_STATUS;
*/


#define __UINT_SIZE	sizeof(__UINT)
#define __ARCH_SIZE	32



#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _YIOS_HAL_TYPES_H_ */



