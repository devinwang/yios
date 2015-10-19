
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
 * Filename:	$RCSfile: main.c,v $
 * Createdate:	2004/06/07 17:13:00
 * FileId:	$Id: main.c,v 1.1.1.1 2005/08/09 05:25:25 devin Exp $
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

#include <yios/list.h>

#include <yios/hal/io.h>

#include <yios/sched.h>
#include <yios/rtl/rtl.h>
#include <yios/time.h>
#include <yios/int.h>
#include <yios/sem.h>
#include <yios/memory.h>
#include <yios/hal/init.h>


/* external function declare. */
EXTERN YI_STATUS  YIDriverKBInitialize(VOID);
EXTERN YI_STATUS  YIDriverPs2MouInitialize(VOID);
EXTERN YI_STATUS YIDriverComInitialize(VOID);
EXTERN VOID YIDrawFillRect(INT x1, INT y1, INT x2, INT y2, UINT32 color);
EXTERN VOID YIDbgInitDebug();

/*STATIC*/ UINT INLINE Rand(); 

UINT g_st_rand_num;
EXTERN VOID TestInit();

EXTERN UINT YICALL KernelMain()
{
	/* struct aa test;
	YI_PLIST_HEAD ptest;
	ptest=&test.list;
	YI_LIST_ENTRY(ptest, struct aa, list)->list.pNext = 0;*/
#if CFG_VBE == 1
    YIDrawFillRect(0, 0, 800, 600, 0);
#endif /* CFG_VBE */
#if	(CFG_DEBUG == 1)
  	YIDbgInitDebug();
    /*YIDebugPoint();*/
#endif

	YIIntInitialize();

	/* __asm__("cli\n\thlt");*/
	YMInitialize();    

	YITimeInitialize();
    
    /* YIDebugPoint(); */

    YIDriverKBInitialize();
    YIDriverPs2MouInitialize();
    YIDriverComInitialize();
#if CFG_KERNEL_SCHED_METHOD_RM == 1
	YIKeSchedRMInit();
#endif /* CFG_KERNEL_SCHED_METHOD_RM */
#if CFG_KERNEL_SCHED_METHOD_TIMESLICE == 1
	YIKeSchedRMInit();
#endif /* CFG_KERNEL_SCHED_METHOD_TIMESLICE */


	/* YIRtlKernelMsg("Welcome to YingOS! Copyright(C) 2004-2005");*/
                                                                          
	YIRtlKernelMsg("\n    +-----------------------------------------+\n");
	YIRtlKernelMsg("    |Welcome to                Ver: 1.0.0     |\n");
	YIRtlKernelMsg("    |                                         |\n");
	YIRtlKernelMsg("    |__     __                  ____    _____ |\n");
	YIRtlKernelMsg("    |\\ \\   / /_                / __ \\  / ____||\n");
	YIRtlKernelMsg("    | \\ \\_/ /(_) _ __    __ _ | |  | || (___  |\n");
	YIRtlKernelMsg("    |  \\   / | || '_ \\  / _` || |  | | \\___ \\ |\n");
	YIRtlKernelMsg("    |   | |  | || | | || (_| || |__| | ____) ||\n");
	YIRtlKernelMsg("    |   |_|  |_||_| |_| \\__, | \\____/ |_____/ |\n");
	YIRtlKernelMsg("    |                   |___/                 |\n");
	YIRtlKernelMsg("    |-----------------------------------------|\n");
	YIRtlKernelMsg("    |    Copyright (2004-2005) Devin Wang     |\n");
	YIRtlKernelMsg("    +-----------------------------------------+\n");

#if CFG_GUI > 0
	YIRtlKernelMsg("   ÄúºÃ,»¶Ó­Ê¹ÓÃYingOS!\n");
#endif /* CFG_GUI */

    YIDrawFillRect(100, 100, 200, 200, 0x0000ff00); 
    /* YIDebugPoint();*/
	TestInit();
    /*YIHLT(); */
    /* YIDebugPoint(); */
	/* YIHalExitCritical();	*/
	YIHalReSched();
    /*asm("sti");
    for(;;)
    {
        asm("hlt");
    }*/
	
		
  return 0;
}

STATIC YI_SEMAPHORE g_stTestSem;

#define ALLOC_NUM   30
STATIC UINT VOLATILE  yi_g_nThread5Circle = 0;
STATIC YI_STATUS YICALL TestThread(PVOID data)
{
	UINT nData = (UINT)data;
	UINT nCount = 0;
	for(;;)
	{
        while(nData == 5 && yi_g_nThread5Circle == 1)
        {
            /* unsigned int i,j, k ;
            void* p[ALLOC_NUM];
            
            i = Rand();
            i = i%ALLOC_NUM;
            k = i;
            
            while(i)
            {
                j = Rand()%(1024*200);
                if(j)
                {
                    p[i] = malloc(j);
                    if(p[i] == NULL)
                        YIRtlKernelMsg("malloc error <%d>, <%d>, <%d>\n", k, i, j);
                }else{p[i] = 0; }
                // memset(p[i], 0, j);
                //YIRtlKernelMsg("malloc ok <%d>, <%d>, <%d>\n", k, i, j);
                i--;
            }
            i = k;
            while(i)
            {
                free(p[i]);
                i--;
            }*/
            main(0, NULL);
           
        }

        YIKeSemLock(&g_stTestSem);
		YIRtlKernelMsg("Thread[%d]:%d ", nData, nCount++);
        YIKeSemUnLock(&g_stTestSem);
		YIKeSRMWait(yi_g_nKeCurrentThreadId, 10*nData, YI_WAIT_WITH_TIMEOUT_UNINTERRUPTED);
	}
	return E_OK;
}

STATIC CHAR stack[8][1024];

STATIC YI_STATUS YICALL ShellThread(PVOID data);
#define com1 0x3f8 
#define com2 0x2f8
#define combase com2

#include "demo.c"

EXTERN VOID TestInit()
{
    YIKeSemInit(&g_stTestSem, 1);

	YIKeSRMCreateThread(3, ShellThread, &stack[4][1024-4], (PVOID)9, 0); /*YI_CREATE_SUSPENDED);*/
	YIKeSRMCreateThread(4, TestThread, &stack[0][1024-4], (PVOID)4, YI_CREATE_SUSPENDED);
	YIKeSRMCreateThread(5, TestThread, &stack[1][1024-4], (PVOID)5, YI_CREATE_SUSPENDED);
	YIKeSRMCreateThread(6, TestThread, &stack[2][1024-4], (PVOID)6, YI_CREATE_SUSPENDED);
	YIKeSRMCreateThread(7, TestThread, &stack[3][1024-4], (PVOID)7, YI_CREATE_SUSPENDED);
#if CFG_GUI > 0
	YIKeSRMCreateThread(8, ColorThread, &stack[5][1024-4], NULL, YI_CREATE_SUSPENDED);
	YIKeSRMCreateThread(2, GlShowThread, &stack[6][1024-4], NULL, YI_CREATE_SUSPENDED);
	YIKeSRMCreateThread(9, NovelShowThread, &stack[7][1024-4], NULL, YI_CREATE_SUSPENDED);
#endif /* CFG_GUI */
}

/*static int getChar(void)
{
   while (!(YIIoReadPortByte(combase + 5) & 0x01));
   return YIIoReadPortByte(combase);
}

static void putChar(int ch)
{
   while (!(YIIoReadPortByte(combase + 5) & 0x20));
   YIIoWritePortByte((char) ch, combase);
}*/

EXTERN UINT8 yi_g_NKBBuffer[];
EXTERN INT yi_g_NKBBufPosStart;
EXTERN INT yi_g_NKBBufPosEnd;
EXTERN INT yi_g_NKBShiftDown;


EXTERN UINT8 yi_g_Com2Buffer[];
EXTERN INT yi_g_Com2BufPosStart;
EXTERN INT yi_g_Com2BufPosEnd;

static int g_st_pos;
static char g_st_cmd[1024];
static VOID DoCommand(PCHAR pCmd);
/* Create one shell for ttyS1 */
STATIC YI_STATUS YICALL ShellThread(PVOID data)
{
	YIRtlKernelMsg("YingOS sample shell v1.0\ncommand 'help' for help information.\n>");
	
	YIRtlMemoryZero(g_st_cmd, sizeof(g_st_cmd));
	g_st_pos = 0;

	/* set rand seed. */	
	g_st_rand_num = 12;/* *(PUINT)0x46c;*/
	char hz[3];
    UINT nHz = 0 ;
	for(;;)
	{
		char input;
	    while(yi_g_Com2BufPosStart != yi_g_Com2BufPosEnd || yi_g_NKBBufPosStart != yi_g_NKBBufPosEnd)
		{
            if(yi_g_NKBBufPosStart != yi_g_NKBBufPosEnd)
            {
                input = yi_g_NKBBuffer[yi_g_NKBBufPosEnd];
                yi_g_NKBBufPosEnd++;
                yi_g_NKBBufPosEnd %= 12;
            }
            else
	        {
                input = yi_g_Com2Buffer[yi_g_Com2BufPosEnd];
                yi_g_Com2BufPosEnd++;
                yi_g_Com2BufPosEnd %= 24;
            }
                

			if(input == '\r' || input == '\n')
			{
				
				YIRtlKernelMsg("\n");
				if(g_st_pos!=0)
				{
					g_st_cmd[g_st_pos] = '\0';
					DoCommand(g_st_cmd);
				}
				YIRtlKernelMsg(">");
				g_st_pos = 0;
                nHz = 0;
			}
#if CFG_GUI > 0
           else if(input == '{' || (input == '[' && yi_g_NKBShiftDown))
            {
                SNMovePos(-((NSC_ENDY-NSC_STARTY)/ASCII_HEIGHT-1));
                YIKeSemUnLock(&g_stSNSem);
            }
            else if(input == '}' || (input == ']' && yi_g_NKBShiftDown))
            {
                SNMovePos((NSC_ENDY-NSC_STARTY)/ASCII_HEIGHT-1);
                YIKeSemUnLock(&g_stSNSem);
            }
            else if(input == '[')
            {
                SNMovePos(-1);
                YIKeSemUnLock(&g_stSNSem);
            }
            else if(input == ']')
            {
                SNMovePos(1);
                YIKeSemUnLock(&g_stSNSem);
            }
#endif /* CFG_GUI */
            else if((unsigned char)input > 127)
            {
               if(nHz == 0)
                {
                    nHz = 1;
                    hz[0] = input;
                } 
                else
                {
                   nHz = 0;
                   hz[1] = input;
                   hz[2] = '\0';
                   YIRtlKernelMsg("%s", hz);
                }
                g_st_cmd[g_st_pos++] = input;
            }
			else
			{
				YIRtlKernelMsg("%c", input);
				g_st_cmd[g_st_pos++] = input;
                nHz = 0;
			}
		
		}		
		g_st_rand_num = (g_st_rand_num*123+59);
		/* YIKeSRMWait(2, 2, YI_WAIT_WITH_TIMEOUT_UNINTERRUPTED);*/

		YIKeSRMSuspendThread(3);
		YIHalReSched();
	}

	return E_OK;
}

EXTERN INT32 yi_g_nTtyWidth;
EXTERN INT32 yi_g_nTtyHeight;

EXTERN UINT32 yi_g_nTtyBKColor;
EXTERN UINT32 yi_g_nTtyFTColor;

/* section record. */
EXTERN VOID yi_g_nLdAllStart, yi_g_nLdAllEnd, yi_g_nLdTextStart, yi_g_nLdTextEnd;

#include <yios/dbg.h>

static VOID DoCommand(PCHAR pCmd)
{
  if(YIRtlStringCompare("help", pCmd) == 0)
    {
      YIRtlKernelMsg("YingOS sample shell Version 1.0\n");
      YIRtlKernelMsg("info:  show system information.\n");
      YIRtlKernelMsg("run:   run one kernel thread. \n");
      YIRtlKernelMsg("kill:  stop one kernel thread. \n");
      YIRtlKernelMsg("lock:  lock semaphore. \n");
      YIRtlKernelMsg("unlock:  unlock semaphore. \n");
      YIRtlKernelMsg("circle:  let thread 5 to circle. \n");
      YIRtlKernelMsg("uncir:  let thread 5 not to circle. \n");
      YIRtlKernelMsg("trap:  halt system for debug. \n");
#if CFG_GUI > 0
      YIRtlKernelMsg("mode:  change screen mode. \n");
      YIRtlKernelMsg("bkc:   change back color. \n");
      YIRtlKernelMsg("ftc:   change front color. \n"); 
#endif /* CFG_GUI */
      YIRtlKernelMsg("help:  show this information. \n"); 
    }
  else if(pCmd[0] == 'r' && pCmd[1] == 'u' && pCmd[2] == 'n')
    {
      pCmd += 4;
      YIKeSRMResumeThread(*pCmd-48);
      YIRtlKernelMsg("resume thread %s \n", pCmd);
    }
  else if(pCmd[0] == 'k' && pCmd[1] == 'i' && pCmd[2] == 'l')
    {
      pCmd += 5;
      YIKeSRMSuspendThread(*pCmd-48);
      yi_g_arKeThreadTbl[*pCmd-48].nDelayTime = 0;
      YIRtlKernelMsg("suspend thread %s \n", pCmd);
    }
  else if(pCmd[0] == 'c' && pCmd[1] == 'r' && pCmd[2] == 'a')
    {
      int* p = (int*)g_st_rand_num;
      *p = 1;
	BUG();
    }
  else if(pCmd[0] == 'i' && pCmd[1] == 'n' && pCmd[2] == 'f')
    {
      YIRtlKernelMsg("Clock Tick count: %d\n", yi_g_nTimeTickCount);
      YIRtlKernelMsg("Interrupt count:  %d\n", yi_g_nInterruptCount);
      YIRtlKernelMsg("Alls:0x%0.8x, Alle:0x%0.8x\nTexts:0x%0.8x, Texte:0x%0.8x\n",
		(NRADDR)&yi_g_nLdAllStart, (NRADDR)&yi_g_nLdAllEnd,
		(NRADDR)&yi_g_nLdTextStart, (NRADDR)&yi_g_nLdTextEnd);
    }
  else if(pCmd[0] == 'l' && pCmd[1] == 'o' && pCmd[2] == 'c')
    {
      YIKeSemLock(&g_stTestSem);
      YIRtlKernelMsg("Semaphore locked: %d\n", YIKeAtomRead(&g_stTestSem.stAtomCount));
    }
  else if(pCmd[0] == 'u' && pCmd[1] == 'n' && pCmd[2] == 'l')
    {
      YIKeSemUnLock(&g_stTestSem);
      YIRtlKernelMsg("Semaphore unlocked: %d\n", YIKeAtomRead(&g_stTestSem.stAtomCount));
    }
  else if(pCmd[0] == 'c' && pCmd[1] == 'i' && pCmd[2] == 'r')
    {
      yi_g_nThread5Circle = 1;
      YIRtlKernelMsg("Thread 5 circle: \n");
    }
  else if(pCmd[0] == 'u' && pCmd[1] == 'n' && pCmd[2] == 'c')
    {
      yi_g_nThread5Circle = 0;
      YIRtlKernelMsg("Thread 5 uncircle\n");
    }
  else if(pCmd[0] == 't' && pCmd[1] == 'r' && pCmd[2] == 'a')
    {
    	YIDebugPoint();
    }
#if CFG_GUI > 0
  else if(pCmd[0] == 'm' && pCmd[1] == 'o' && pCmd[2] == 'd')
    {
        /* !!! microwindows test. */
        /* test();*/
        if(yi_g_nTtyWidth == 60)
        {
            yi_g_nTtyWidth = 100;
            YIDrawFillRect(60*8, 0, GSCREEN_WIDTH, GSCREEN_HEIGHT, yi_g_nTtyBKColor);
        }
        else
        {
            yi_g_nTtyWidth = 60;
            YIDrawFillRect(yi_g_nTtyWidth*8, 0, GSCREEN_WIDTH, GSCREEN_HEIGHT, 0x00475790);
        }
        YIRtlKernelMsg("Screen width set to : %d\n", yi_g_nTtyWidth);
    }
  else if(pCmd[0] == 'b' && pCmd[1] == 'k' && pCmd[2] == 'c')
    {
        yi_g_nTtyBKColor = (UINT32)g_st_rand_num; 
        YIRtlKernelMsg("Back color set to : %8x\n", yi_g_nTtyBKColor);
    }
  else if(pCmd[0] == 'f' && pCmd[1] == 't' && pCmd[2] == 'c')
    {
        yi_g_nTtyFTColor = (UINT32)g_st_rand_num; 
        YIRtlKernelMsg("Front color set to : %8x\n", yi_g_nTtyFTColor);
    }
   else if(pCmd[0] == 'r' && pCmd[1] == 'e' && pCmd[2] == 's')
    {
        __asm__(
        "movl $0x9e00, %%edi\n\t"
        "movl $ent_16, %%esi\n\t"
        "movl $64, %%ecx\n\t"
        "rep        \n\t"
        "movsl       \n\t"
        "jmp  $0x10,  $0x9e00\n\t"
        "ent_16:\n\t"
        "xorl %%eax, %%eax\n\t"
        "movl %%eax, %%cr0\n\t"
        ".code16\n\t"
        /* ".byte 0x66\n\t"*/
        "ljmp $0xffff, $0x0000\n\t"
        ".code32\n\t"
        :
        :
        );    
    }
#endif /* CFG_GUI */
  else YIRtlKernelMsg("command not found\n");

}

