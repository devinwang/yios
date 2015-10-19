
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
 * Filename:	$RCSfile: pc_com.c,v $
 * Createdate:	2005/01/08 00:18:00
 * FileId:	$Id: pc_com.c,v 1.1.1.1 2005/08/09 05:25:25 devin Exp $
 * Last update:	$Date: 2005/08/09 05:25:25 $
 * Creator:	Devin Wang <vhd@21cn.com>
 * Version:	$Revision: 1.1.1.1 $
 * Description: serial drvier for pc
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
#include <yios/rtl/rtl.h>


#include <yios/sched.h>
#include <yios/driver.h>
#include <yios/int.h>

#include <yios/hal/int.h>
#include <yios/hal/io.h>


#define COM1_PORT	0x3f8
#define COM2_PORT	0x2f8

#define combase COM2_PORT


STATIC  YI_IRQ_HANDLE_STRUCT yi_g_stCom2IrqHandleSt;

UINT8 yi_g_Com2Buffer[24];
INT yi_g_Com2BufPosStart = 0;
INT yi_g_Com2BufPosEnd = 0;

STATIC VOID YIDriverCom2IntHandle(INT nIntNum, YI_HAL_CPU_REGS* pRegs, PVOID pData)
{
    YI_HAL_INT_FINISH_INT;

    while(YIIoReadPortByte(combase+5)&0x01)
    {
        if((yi_g_Com2BufPosStart+1)%24 != yi_g_Com2BufPosEnd)
        {   
            yi_g_Com2Buffer[yi_g_Com2BufPosStart] = YIIoReadPortByte(combase);
            yi_g_Com2BufPosStart++;
            yi_g_Com2BufPosStart %= 24;
        }
        else
            break;
    }
    if(yi_g_Com2BufPosStart != yi_g_Com2BufPosEnd)
        YIKeSRMResumeThread(3);
}

#define TERM_WIDTH  80
#define TERM_HEIGHT 25

STATIC INT YIDriverCom2OutByte(CHAR nChar)
{
    while (!(YIIoReadPortByte(combase + 5) & 0x20));
        YIIoWritePortByte(nChar, combase);
    return 0;
}

EXTERN INT YIDriverCom2OutChar(CHAR nChar)
{
    static INT x=0;

    if(nChar == '\n' || ++x >= TERM_WIDTH)
    {
        YIDriverCom2OutByte('\r');
        YIDriverCom2OutByte('\n');
        x = 0;
    }
    else
        YIDriverCom2OutByte(nChar);
    return 0;
}

EXTERN YI_STATUS YIDriverComInitialize(VOID)
{
   

   /* Initialize com2 interrupt chanel. */
    YIRtlMemoryZero(&yi_g_stCom2IrqHandleSt, sizeof(yi_g_stCom2IrqHandleSt));

    yi_g_stCom2IrqHandleSt.pHandle = YIDriverCom2IntHandle;
    yi_g_stCom2IrqHandleSt.nMask = YI_IRQM_SHARE;

    YIIntInstallHandle(YI_HAL_INT_CHANNEL_COM2, &yi_g_stCom2IrqHandleSt);

    /* Initialize serial port hardware. */
    /* set baud rate 9660*/ 
    YIIoWritePortByte(YIIoReadPortByte(combase+2)|0x80, combase+3);
    YIIoWritePortByte(0x0c, combase);
    YIIoWritePortByte(0, combase+1);
    YIIoWritePortByte(YIIoReadPortByte(combase+2)&0x7f, combase+3);

    /* 8 N 1 */
    YIIoWritePortByte(0x03, combase+3);

    /* enable interrupt.  */
    YIIoWritePortByte(0x08, combase+4);
    YIIoWritePortByte(0x01, combase+1);

    /* YIIoReadPortByte(combase+5);*/

    /*nCode = YIIoReadPortByte(KBPORT_DATA);
    YIRtlKernelMsg("(Key:%d,%c)", nCode, nCode);
*/
    return E_OK; 
}

#if 0

STATIC INT PccomFileOpen(YI_PFS_FILE_DECS_STRUCT pFd, YI_PFS_INODE pNode);
STATIC INT PccomFileClose(YI_PFS_FILE_DECS_STRUCT pFd, YI_PFS_INODE pNode);
STATIC SIZE_T PccomFileRead(YI_PFS_FILE_DECS_STRUCT pFd, PVOID pBuf, SIZE_T nSize);
STATIC SIZE_T PccomFileWrite(YI_PFS_FILE_DECS_STRUCT pFd, PVOID pBuf, SIZE_T nSize);
 
EXTERN YI_STATUS PccomDriverEntry(YI_PFS_INODE pInode)
{
  /* Init system file call function. */
  pInode->pVfs->FileOpen = PccomFileOpen;
  pInode->pVfs->FileClose = PccomFileClose;
  pInode->pVfs->FileRead = PccomFileRead;
  pInode->pVfs->FileWrite = PccomFileWrite;

  /* */

  return E_OK;
}


STATIC INT PccomFileOpen(YI_PFS_FILE_DECS_STRUCT pFd, YI_PFS_INODE pNode)
{
  pFd->nPos = 0;
  
  return E_OK;
}

STATIC INT PccomFileClose(YI_PFS_FILE_DECS_STRUCT pFd, YI_PFS_INODE pNode)
{
  pFd->nPos = 0;
  return E_OK;
}

STATIC SIZE_T PccomFileRead(YI_PFS_FILE_DECS_STRUCT pFd, PVOID pBuf, SIZE_T nSize)
{
  
  return nSize;
}

STATIC SIZE_T PccomFileWrite(YI_PFS_FILE_DECS_STRUCT pFd, PVOID pBuf, SIZE_T nSize)
{
  pFd->nPos += nSize;
  return nSize;
}
#endif /* 0 */
