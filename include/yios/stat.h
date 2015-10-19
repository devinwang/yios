
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
 * Filename:	$RCSfile: stat.h,v $
 * Createdate:	2005/01/07 21:02:00
 * FileId:	$Id: stat.h,v 1.1.1.1 2005/08/09 05:25:23 devin Exp $
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


#ifndef _YIOS_STAT_H_
#define _YIOS_STAT_H_

#include <yios/config.h>
#include <yios/types.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus  */


/* Virtual file system struct */
typedef struct _YI_FS_STAT
{
  DEV_T		st_dev;		/* device */
  INO_T		st_ino;		/* inode */
  MODE_T	st_mode;	/* protection */
  NLINK_T	st_nlink;	/* number of hard links */
  UID_T		st_uid;		/* user id of owner */
  GID_T		st_gid;		/* group id of owner */
  DEV_T		st_rdev;	/* device type */
  OFF_T		st_size;	/* total size */
  TIME_T	st_atime;	/* time of last access */
  TIME_T	st_mtime;	/* time of last modification. */
  TIME_T	st_ctime;	/* time of last change. */
}YI_FS_STAT, *YI_PFS_STAT;


#define _IFMT		0170000		/* type of file */
#define _IFDIR		0040000		/* directory */
#define _IFCHR		0020000		/* character special */
#define _IFBLK		0060000		/* block special */
#define _IFREG		0100000		/* regular */
#define _IFLNK		0120000		/* symbolic link */
#define _IFSOCK		0140000		/* socket */
#define _IFIFO		0010000		/* fifo */

#define S_BLKSIZE	1024		/* size of a block */

#define S_ISUID		0004000		/* set user id on execution */
#define S_ISGID		0002000		/* set group id on execution */
#define S_ISVTX		0001000		/* save swapped text even after use */
#define S_IREAD		0000400		/* read permission, owner */
#define S_IWRITE	0000200		/* write permission, owner */
#define S_IEXEC		0000100		/* execute/search permission, owner */
#define S_ENFMT		0002000		/* enforcement-mode locking */

#define S_IFMT		_IFMT
#define S_IFDIR		_IFDIR
#define S_IFCHR		_IFCHR
#define S_IFBLK		_IFBLK
#define S_IFREG		_IFREG
#define S_IFLNK		_IFLNK
#define S_IFSOCK	_IFSOCK
#define S_IFIFO		_IFIFO


#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _YIOS_STAT_H_ */
