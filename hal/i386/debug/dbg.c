
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
 * Filename:	$RCSfile: dbg.c,v $
 * Createdate:	2004/06/07 17:21:00
 * FileId:	$Id: dbg.c,v 1.1.1.1 2005/08/09 05:25:24 devin Exp $
 * Last update:	$Date: 2005/08/09 05:25:24 $
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
#include <yios/hal/io.h>

#define SCREEN_WIDTH	80
#define SCREEN_HEIGHT	25

#if CFG_VBE == 0
STATIC INT32 g_cursor_x = 0, g_cursor_y = 0;
#endif /* CFG_VBE */

#if CFG_DEBUG_METHOD_SERIAL_PORT == 1
EXTERN YI_STATUS YICALL YIGdbstubInit();
#endif /* CFG_DEBUG_METHOD_SERIAL_PORT */

/* STATIC VOID STDCALL LdDbgScreenInit(); */

EXTERN VOID YIDbgInitDebug()
{
#if CFG_DEBUG_METHOD_SERIAL_PORT == 1
	YIGdbstubInit();
#endif /* CFG_DEBUG_METHOD_SERIAL_PORT */
}

#if CFG_VBE == 0
STATIC VOID YICALL YIMoveScreen()
{
	PUINT pScreenAddress = (PUINT)0xB8000;

	for(; pScreenAddress < (PUINT)0xB8000+960; pScreenAddress ++ )
		*pScreenAddress = *(pScreenAddress + 40);
	
	for(; pScreenAddress < (PUINT)0xB8000+1000; pScreenAddress++)
		*pScreenAddress = 0x3f003f00;

	return;
}
#endif /* CFG_VBE */
/* void update_cursor(int row, int col)
         * by Dark Fiber
         */
#if CFG_VBE == 0
void update_cursor(int row, int col)
{
	unsigned short position=(row*80) + col;

	// cursor LOW port to vga INDEX register
	YIIoWritePortByte(0x0F, 0x3D4);
	YIIoWritePortByte((unsigned char)(position&0xFF), 0x3D5);
	// cursor HIGH port to vga INDEX register
	YIIoWritePortByte(0x0E, 0x3D4);
	YIIoWritePortByte((unsigned char )((position>>8)&0xFF), 0x3D5);
}
#endif /* CFG_VBE */

#if CFG_VBE == 0
EXTERN INT  YIDriverCom2OutChar(CHAR nChar);
EXTERN UINT32 STDCALL LdDrawString(PCSTR pString, UINT8 color)
{
	PCHAR	pScreenAddress = (PCHAR)YI_P2V(0xB8000);
	while(*pString)
	{
        /* send char to serial port. */
        YIDriverCom2OutChar(*pString);

		if(*pString == '\n')
		{
			g_cursor_x = 0;
			if(++g_cursor_y >= SCREEN_HEIGHT)
			{
				YIMoveScreen();
				g_cursor_y--;
			}
		}
		else
		{
			*(pScreenAddress + (g_cursor_y * SCREEN_WIDTH + g_cursor_x)*2) = *pString;
			*(pScreenAddress + (g_cursor_y * SCREEN_WIDTH + g_cursor_x)*2 + 1) = color;
			g_cursor_x++;
			g_cursor_y = (g_cursor_y + (g_cursor_x / SCREEN_WIDTH));
			if(g_cursor_y >= SCREEN_HEIGHT)
			{
				YIMoveScreen();
				g_cursor_y--;
			}
			g_cursor_x = g_cursor_x % SCREEN_WIDTH;
		}
		pString++;
	}

	update_cursor(g_cursor_y, g_cursor_x);
	return 0;
}

#endif /* CFG_VBE */
