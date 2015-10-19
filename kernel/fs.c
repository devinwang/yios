
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
 * Filename:	$RCSfile: fs.c,v $
 * Createdate:	2005/01/08 17:50:00
 * FileId:	$Id: fs.c,v 1.1.1.1 2005/08/09 05:25:25 devin Exp $
 * Last update:	$Date: 2005/08/09 05:25:25 $
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


#include <yios/config.h>
#include <yios/types.h>
#include <yios/errno.h>

#include <yios/file.h>
#include <yios/stat.h>

#if CFG_TFS == 1
YI_FS_DIRECTORY g_stTFSRootDir;

NT_STATUS YITFSInitPhaseOne()
{
  YIRtlMemoryZero(&g_stTFSRootDir, sizeof(g_stTFSRootDir));

  return E_OK;
}

NT_STATUS YITFSInitPhaseTwo()
{
  
  return E_OK;
}

INT YITFSGetNewFd()
{
  return -1;
}

INT YITFSFileOpen(PCSTR pFile, INT nFlags, INT nMode)
{
  
}

#endif /* CFG_TFS */
