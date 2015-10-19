
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
 * Filename:	$RCSfile: nkb.c,v $
 * Createdate:	2005/01/08 00:18:00
 * FileId:	$Id: nkb.c,v 1.1.1.1 2005/08/09 05:25:25 devin Exp $
 * Last update:	$Date: 2005/08/09 05:25:25 $
 * Creator:	Devin Wang <vhd@21cn.com>
 * Version:	$Revision: 1.1.1.1 $
 * Description: ps2 drvier for pc
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

#include <yios/sched.h>
#include <yios/driver.h>
#include <yios/sem.h>

#include <yios/int.h>
#include <yios/rtl/rtl.h>

#include <yios/hal/io.h>
/*
#define KBPORT_DATA     0x60
#define KBPORT_CMD      0x64
#define KBDATC_RESET    0xff
*/
#define PS2MICE_CTIMEOUT	250
#define PS2MICE_DTIMEOUT	1000	

#define PS2MICE_KBD_INT_NUM	0x41
#define PS2MICE_MICE_INT_NUM	0x4c


#define PS2MICE_DATA_PORT	0x60
#define PS2MICE_CTRL_PORT	0x64

#define PS2MICE_CTRL_WRITEMODE		0x60
#define PS2MICE_CTRL_DISABLEMICE	0xa7
#define PS2MICE_CTRL_ENABLEMICE		0xa8
#define PS2MICE_CTRL_SELECTMOUSE	0xd4

#define PS2MICE_MODE_KBD_INT		0x01
#define PS2MICE_MODE_MICE_INT		0x02
#define PS2MICE_MODE_SYS		0x04
#define PS2MICE_MODE_NO_KEYLOCK		0x08
#define PS2MICE_MODE_DISABLE_KBD	0x10
#define PS2MICE_MODE_DISABLE_MICE	0x20
#define PS2MICE_MODE_KCC		0x40
#define PS2MICE_MODE_RFU		0x80

#define PS2MICE_MODE_OFF		\
   (PS2MICE_MODE_KCC | PS2MICE_MODE_DISABLE_MICE |\
   PS2MICE_MODE_SYS | PS2MICE_MODE_KBD_INT)
   
#define PS2MICE_MODE_ON			\
   (PS2MICE_MODE_KCC | PS2MICE_MODE_SYS |\
   PS2MICE_MODE_MICE_INT | PS2MICE_MODE_KBD_INT)
   

#define PS2MICE_CMD_BUFFFULL		0x00
#define PS2MICE_CMD_SET_RES		0xe8
#define PS2MICE_CMD_SET_SCALE11		0xe6
#define PS2MICE_CMD_SET_SCALE21		0xe7
#define PS2MICE_CMD_GET_SCALE		0xe9
#define PS2MICE_CMD_SET_STREAM		0xea
#define PS2MICE_CMD_RECONNECT		0xaa
#define PS2MICE_CMD_GET_ID		0xf2
#define PS2MICE_CMD_SET_SAMPLE		0xf3
#define PS2MICE_CMD_ENABLE_DEV		0xf4
#define PS2MICE_CMD_DISABLE_DEV		0xf5
#define PS2MICE_CMD_RESET		0xff
#define PS2MICE_CMD_ACK			0xfa


#define PS2MICE_STAT_RDBUFABLE	0x01
#define PS2MICE_STAT_WTBUFFULL	0x02
#define PS2MICE_STAT_POSTSUC	0x04
#define PS2MICE_STAT_PREVCTRL	0x08
#define PS2MICE_STAT_KBENABLE	0x10
#define PS2MICE_STAT_MICEBUF	0x20
#define PS2MICE_STAT_TIMEOUT	0x40
#define PS2MICE_STAT_SERROR	0x80

#define PS2MICE_TYPE_NORMAL	0x00
#define PS2MICE_TYPE_MSSCROL	0x03
#define PS2MICE_TYPE_5BUTTON	0x04

#define PS2MICE_BUTTON_L        0x01
#define PS2MICE_BUTTON_R        0x02
#define PS2MICE_BUTTON_M        0x03

#define PS2MICE_SELECTMOUSE	YIIoWritePortByte(0xd4, PS2MICE_CTRL_PORT)
#define Ps2MiceReadData()	YIIoReadPortByte(PS2MICE_DATA_PORT)
#define Ps2MiceReadStatus()	YIIoReadPortByte(PS2MICE_CTRL_PORT)
#define Ps2MiceWriteData(x)	YIIoWritePortByte((x), PS2MICE_DATA_PORT)
#define Ps2MiceWriteCtrl(x)	YIIoWritePortByte((x), PS2MICE_CTRL_PORT)

#define Ps2MiceWriteCtrlWait(x)	\
   do{\
      Ps2MiceWaitBufDrain();\
      Ps2MiceWriteCtrl((x));\
   }while(0)

#define Ps2MiceWriteDataWait(x)	\
   do{\
      Ps2MiceWaitBufDrain();\
      Ps2MiceWriteData((x));\
   }while(0)
   
#define Ps2MiceWriteAck(x)		\
   do{\
      Ps2MiceWaitBufDrain();\
      Ps2MiceWriteCtrl(PS2MICE_CTRL_SELECTMOUSE);\
      Ps2MiceWaitBufDrain();\
      Ps2MiceWriteData((x));\
      Ps2MiceWaitBufDrain();\
      }while(0)

#define Ps2MiceWriteCmd(x)		\
   do{\
      Ps2MiceWaitBufDrain();\
      Ps2MiceWriteCtrl(PS2MICE_CTRL_WRITEMODE);\
      Ps2MiceWaitBufDrain();\
      Ps2MiceWriteData((x));\
      }while(0)

STATIC YI_SEMAPHORE g_stPs2MouSem;

static INT g_bMiceDataAck = FALSE;
static INT g_bMiceDataReSend = FALSE;
static INT g_bMiceType;
static INT g_nWaitMiceAck = 0;
static INT g_nInputBufferBegin = 0;
static INT g_nInputBufferEnd=0;

#define PS2MICE_BUFFER_SIZE (128*3)
static UINT8 g_arInputBuffer[PS2MICE_BUFFER_SIZE];


EXTERN UINT CDECLCALL YIRtlKernelMsg(PCSTR fmt, ...);

STATIC  YI_IRQ_HANDLE_STRUCT yi_g_stPs2MouIrqHandleSt;
/*
UINT8 yi_g_NKBBuffer[12];
INT yi_g_NKBBufPosStart = 0;
INT yi_g_NKBBufPosEnd = 0;
INT yi_g_NKBShiftDown = 0;
*/

static UINT8 Ps2MiceHandleEvent(VOID);

EXTERN INT Ps2MicePoll()
{
    //YIRtlKernelMsg("<2>");
    if((g_nInputBufferBegin + PS2MICE_BUFFER_SIZE - g_nInputBufferEnd)%
        PS2MICE_BUFFER_SIZE >=3)
        return 1;
    return 0;
}

EXTERN INT Ps2MiceRead(INT* x, INT* y, INT* z, INT* button)
{
    INT8 Buf[3];

    YIKeSemLock(&g_stPs2MouSem);
    
    if(g_nInputBufferBegin == g_nInputBufferEnd)
        return 0;

    if((g_nInputBufferBegin + PS2MICE_BUFFER_SIZE - g_nInputBufferEnd)%
        PS2MICE_BUFFER_SIZE >=3)
    {
        Buf[0] = g_arInputBuffer[g_nInputBufferEnd++];
        g_nInputBufferEnd %= PS2MICE_BUFFER_SIZE;
        Buf[1] = g_arInputBuffer[g_nInputBufferEnd++];
        g_nInputBufferEnd %= PS2MICE_BUFFER_SIZE;
        Buf[2] = g_arInputBuffer[g_nInputBufferEnd++];
        g_nInputBufferEnd %= PS2MICE_BUFFER_SIZE;
        
        //YIRtlKernelMsg("<%02x,%02x,%02x>", 0x0000ff&Buf[0], 0x0000ff& Buf[1], 0x0000ff&Buf[2]);
        *x = (INT)Buf[0];
        *y = (INT)Buf[2];
        //*button = (0x07 & Buf[1]);
        *button = 0xff &(INT)Buf[1];
        *z = 0;
        YIKeSemUnLock(&g_stPs2MouSem);
        return 1;
    }

        //YIRtlKernelMsg("<2>");
   
    YIKeSemUnLock(&g_stPs2MouSem);
    return 0;
}

static VOID Ps2MiceWaitBufDrain(VOID)
{
   /* LONG lTimeOut = PS2MICE_CTIMEOUT;*/
   UINT64 lTimeOut = 50;
   do
   {
      UINT8 byStatus = Ps2MiceHandleEvent();
      if(!(byStatus & PS2MICE_STAT_WTBUFFULL))
         return;
      /* Delay(10);*/
      lTimeOut--;
   }while(lTimeOut);
}

static VOID Ps2MiceHandleMiceEvent(UINT8 nCode)
{
   //MyOSCrash();
   if(g_nWaitMiceAck)
   {
      if(nCode == PS2MICE_CMD_ACK)
      {
         g_nWaitMiceAck--;
         return;
      }
      g_nWaitMiceAck = 0;
   }
   else if(nCode == PS2MICE_CMD_RECONNECT)
   {
      Ps2MiceWriteAck(PS2MICE_CMD_ENABLE_DEV);
      return;
   }
   /*g_aMiceInputBuffer[g_iInputBufferPoint] = byCode;
   g_iInputBufferPoint++;*/
   if(((g_nInputBufferBegin + PS2MICE_BUFFER_SIZE - g_nInputBufferEnd)
        %PS2MICE_BUFFER_SIZE) +3 < (PS2MICE_BUFFER_SIZE-1))
   {
        g_arInputBuffer[g_nInputBufferBegin] = nCode;
        g_nInputBufferBegin ++;
        g_nInputBufferBegin %= PS2MICE_BUFFER_SIZE;
   }
} 

STATIC VOID YIDriverPs2MouIntHandle(INT nIntNum, YI_HAL_CPU_REGS* pRegs, PVOID pData)
{
    UINT8 nStatus;
    UINT  nWork = 10000;

    nStatus = Ps2MiceReadStatus();
    while((--nWork >0) && (nStatus & PS2MICE_STAT_RDBUFABLE))
    {
        UINT8 nScanCode;
        nScanCode = Ps2MiceReadData();

        if(!(nStatus & (PS2MICE_STAT_TIMEOUT | PS2MICE_STAT_SERROR)))
        {
            Ps2MiceHandleMiceEvent(nScanCode); 
            /*if(((g_nInputBufferBegin + PS2MICE_BUFFER_SIZE - g_nInputBufferEnd)
                    %PS2MICE_BUFFER_SIZE) +3 < (PS2MICE_BUFFER_SIZE-1))
            {
                g_arInputBuffer[g_nInputBufferBegin] = nCode;
                g_nInputBufferBegin ++;
                g_nInputBufferBegin %= PS2MICE_BUFFER_SIZE;
            }
            
        }else if(nCode == PS2MICE_CMD_RECONNECT)
        {
            Ps2MiceWaitBufDrain();
            Ps2MiceWriteData(PS2MICE_CMD_ENABLE_DEV);
            Ps2MiceWaitBufDrain();
        }*/
        } 
        nStatus = Ps2MiceReadStatus();
        /* YIRtlKernelMsg("<%02x,%02x>", nCode, nStatus);*/
    }//while(nStatus & PS2MICE_STAT_RDBUFABLE);
    //YIIoWritePortByte()
    
    
    YI_HAL_INT_FINISH_INT;

    /* YIRtlKernelMsg("<%02x,%02x>", nCode, nStatus);*/
#if 0
    nCode = YIIoReadPortByte(KBPORT_DATA);
    nStatus = YIIoReadPortByte(KBPORT_CMD);
    /*YIRtlKernelMsg("(Key:%d,%c)", nCode, nCode);
*/

    YI_HAL_INT_FINISH_INT;
    /* YIRtlKernelMsg("(Key:%2x,%c)", dd, dd);*/
    /* dd = YIIoReadPortByte(KBPORT_CMD);*/


    /*YIRtlKernelMsg("(Stat:%2x,Code:%2x)", nStatus, yi_g_arNKBMapTbl[nCode]);*/
    if(nCode == 0x2a || nCode == 0x36)
        yi_g_NKBShiftDown = 1;
    if(nCode == 0xaa || nCode == 0xb6)
        yi_g_NKBShiftDown = 0;


    if(nCode < 0x80)
    {
        if((yi_g_NKBBufPosStart+1)%12 != yi_g_NKBBufPosEnd)
        {   
            yi_g_NKBBuffer[yi_g_NKBBufPosStart] = yi_g_arNKBMapTbl[nCode];
            yi_g_NKBBufPosStart++;
            yi_g_NKBBufPosStart %= 12;
        }
            YIKeSRMResumeThread(3);
//    YIRtlKernelMsg("%c", yi_g_arNKBMapTbl[nCode]);
//    YIRtlKernelMsg("(%s,%d)", yi_g_NKBBuffer, yi_g_NKBBufPos);
    }

    
    //if(nCode )
    

    /* if(nCode == 0)
        return;
    */
    /* YIKeTimerRoutine(0);*/
#endif
    /* YIHLT();*/
}

EXTERN YI_STATUS YIDriverPs2MouInitialize(VOID)
{
    YIKeSemInit(&g_stPs2MouSem, 1);
    /* Initialize ps2 mouse. */
   Ps2MiceWriteCtrlWait(PS2MICE_CTRL_ENABLEMICE);
/*
   Ps2MiceWriteAck(PS2MICE_CMD_SET_SAMPLE);
   Ps2MiceWriteAck(100);
   Ps2MiceWriteAck(PS2MICE_CMD_SET_RES);
   Ps2MiceWriteAck(3);
   Ps2MiceWriteAck(PS2MICE_CMD_SET_SCALE21);
*/   
   
   
   /* Get mouse type. */
/*   Ps2MiceWriteAck(PS2MICE_CMD_GET_ID);
   Delay(20);
   

   Ps2MiceReadData();*/
   /* g_bMiceType = Ps2MiceReadData();*/
/*   Ps2MiceReadData();*/
   /* g_bMiceType = PS2MICE_TYPE_NORMAL;*/

  /* if(g_bMiceType == 0)//PS2MICE_CMD_ACK)
     {
  g_bMiceType = PS2MICE_TYPE_MSSCROL;
  }*/

   Ps2MiceWriteCtrlWait(PS2MICE_CTRL_DISABLEMICE);
   Ps2MiceWriteCmd(PS2MICE_MODE_OFF);
   
   /* hook keybord interrupt function. */
   /* IntSetIntFunction(PS2MICE_KBD_INT_NUM, IntPs2MiceIntFun);*/

   /* hook ps2 int function. */
   /* IntSetIntFunction(PS2MICE_MICE_INT_NUM, IntPs2MiceIntFun);*/
   
   Ps2MiceWriteCtrlWait(PS2MICE_CTRL_ENABLEMICE);

   Ps2MiceWriteAck(PS2MICE_CMD_ENABLE_DEV);
   Ps2MiceWriteCmd(PS2MICE_MODE_ON);
//   Delay(20);

   //Ps2MiceSendData(PS2MICE_CMD_ENABLE_DEV);

   //return TRUE;
  
    /* YIRtlMemoryZero(yi_g_NKBBuffer, sizeof(yi_g_NKBBuffer));*/

   /* Initialize ps2 mouse interrupt chanel. */
    YIRtlMemoryZero(&yi_g_stPs2MouIrqHandleSt, sizeof(yi_g_stPs2MouIrqHandleSt));

    yi_g_stPs2MouIrqHandleSt.pHandle = YIDriverPs2MouIntHandle;
    yi_g_stPs2MouIrqHandleSt.nMask = YI_IRQM_SHARE;

    YIIntInstallHandle(YI_HAL_INT_CHANNEL_PS2MOU, &yi_g_stPs2MouIrqHandleSt);


    /*nCode = YIIoReadPortByte(KBPORT_DATA);
    YIRtlKernelMsg("(Key:%d,%c)", nCode, nCode);
*/
 
    /* Initialize keybord hardware. */
    //YIIoReadPortByte(0x21)
    //YIIoWritePortByte(KBDATC_RESET, KBPORT_DATA);
    return E_OK;
}


/*
static VOID Ps2MiceHandleKeybordEvent(BYTE byCode)
{
   //MyOSCrash();
   if(g_iWaitMiceAck)
   {
      if(byCode == PS2MICE_CMD_ACK)
      {
         g_iWaitMiceAck--;
         return;
      }
      g_iWaitMiceAck = 0;
   }
   else if(byCode == PS2MICE_CMD_RECONNECT)
   {
      Ps2MiceWaitBufDrain();
      Ps2MiceWriteData(PS2MICE_CMD_ENABLE_DEV);
      Ps2MiceWaitBufDrain();
      return;
   }

}
*/
#if 0
static VOID Ps2MiceHandleMiceEvent(UINT8 byCode)
{
   //MyOSCrash();
   if(g_nWaitMiceAck)
   {
      if(byCode == PS2MICE_CMD_ACK)
      {
         g_nWaitMiceAck--;
         return;
      }
      g_nWaitMiceAck = 0;
   }
   else if(byCode == PS2MICE_CMD_RECONNECT)
   {
      Ps2MiceWriteAck(PS2MICE_CMD_ENABLE_DEV);
      return;
   }
   /* g_aMiceInputBuffer[g_iInputBufferPoint] = byCode;
   g_iInputBufferPoint++;*/

}
#endif /* 0 */

static UINT8 Ps2MiceHandleEvent(VOID)
{
   UINT8 byStatus;
   UINT32 iWork = 10000;
   byStatus = Ps2MiceReadStatus();
   while((--iWork > 0) && (byStatus & PS2MICE_STAT_RDBUFABLE))
   {
      UINT8 byScanCode;
      byScanCode = Ps2MiceReadData();
      if(!(byStatus & (PS2MICE_STAT_TIMEOUT | PS2MICE_STAT_SERROR)))
      {
//         if(byStatus & PS2MICE_STAT_MICEBUF)
 //        {
            /* Mouse send Data. */
            Ps2MiceHandleMiceEvent(byScanCode);
//         }
//         else
//         {
            /* keybord send Data. */
//            Ps2MiceHandleKeybordEvent(byScanCode);
 //        }
      }
      byStatus = Ps2MiceReadStatus();
   }
#if 0
   if(g_bMiceType == PS2MICE_TYPE_NORMAL)
   {
      if(g_iInputBufferPoint >=3)
      {
         /* MoveMouse(g_aMiceInputBuffer);*/
         g_iInputBufferPoint = 0;
      }
   }
   else
   {
         if(g_iInputBufferPoint >=4)
      {
         /* MoveMouse(g_aMiceInputBuffer);*/
         g_iInputBufferPoint = 0;
      }
   }
#endif
   return byStatus;
}


