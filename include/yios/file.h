
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
 * Filename:	$RCSfile: file.h,v $
 * Createdate:	2004/08/22 21:02:00
 * FileId:	$Id: file.h,v 1.1.1.1 2005/08/09 05:25:23 devin Exp $
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


#ifndef _YIOS_FILE_H_
#define _YIOS_FILE_H_

#include <yios/config.h>
#include <yios/types.h>
#include <yios/list.h>

#include <yios/stat.h>


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus  */


  typedef struct _YI_FS_VFS_STRUCT 		*YI_PFS_VFS_STRUCT;
  typedef struct _YI_FS_INODE			*YI_PFS_INODE;
  typedef struct _YI_FS_FILE_DECS_STRUCT	*YI_PFS_FILE_DECS_STRUCT;
  typedef struct _YI_FS_DIRECTORY		*YI_PFS_DIRECTORY;
  typedef struct _YI_FS_FILE_HANDLE		*YI_PFS_FILE_HANDLE;

  /* inode struct. */
  typedef struct _YI_FS_INODE
  {
    INO_T               nInodeId;
    DEV_T   		    nMainId;
    DEV_T       	    nSecId;
    YI_PFS_VFS_STRUCT	pVfs;
    YI_PFS_DIRECTORY	pDir;
  } YI_FS_INODE;


  /* directory struct. */
  typedef struct _YI_FS_DIRECTORY
  {
#if CFG_FS_TFS == 1
    YI_FS_INODE arstFile[CFG_FS_TFS_MAX_FILE];
#else /* Not YI_FS_TFS */
    YI_LIST_HEAD stParent;
    YI_LIST_HEAD stSubdir;
#endif /* YI_FS_TFS */
  } YI_FS_DIRECTORY;



/* Virtual file system struct */
  typedef struct _YI_FS_VFS_STRUCT
  {
    INT (*FileOpen) (YI_PFS_FILE_DECS_STRUCT, YI_PFS_INODE);
    INT (*FileClose) (YI_PFS_FILE_DECS_STRUCT, YI_PFS_INODE);
    SIZE_T (*FileRead) (YI_PFS_FILE_DECS_STRUCT, PVOID, SIZE_T);
    SIZE_T (*FileWrite) (YI_PFS_FILE_DECS_STRUCT, PVOID, SIZE_T);
    INT (*FileIoctl) (YI_PFS_FILE_DECS_STRUCT, INT, INT);
    INT (*FileStat) (YI_PFS_FILE_DECS_STRUCT, YI_PFS_STAT);
  } YI_FS_VFS_STRUCT;


/* file descriptor struct */
  typedef struct _YI_FS_FILE_DECS_STRUCT
  {
    /* This file descriptor have be used ? */
    INT nUse;
    /* file id number */
    INT nId;
    /* file position point. */
    OFF_T nPos;
    /* */

    /* extra data. */
    PVOID pData;

  } YI_FS_FILE_DECS_STRUCT;


/* process file struct */

  typedef struct _YI_FS_FILE_HANDLE
  {
    INT nFileDescNum;		/* how many file descriptor be opened. */
    YI_FS_FILE_DECS_STRUCT arpFileDesc[CFG_FS_PROCESS_MAX_FD];
  } YI_FS_FILE_HANDLE;

#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _YIOS_FILE_H_ */
