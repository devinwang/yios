
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
 * Filename:	$RCSfile: vbe.c,v $
 * Createdate:	2004/06/07 17:21:00
 * FileId:	$Id: vbe.c,v 1.1.1.1 2005/08/09 05:25:24 devin Exp $
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

#include <yios/hal/init.h>

#if CFG_GUI == 1
#define ASCII_WIDTH     8
#define ASCII_HEIGHT    16
#if CFG_VBE == 1

#define GSCREEN_WIDTH   800
#define GSCREEN_HEIGHT  600
#define GSCREEN_DEEP    4

#else /* CFG_VBE == 0 */
#define GSCREEN_WIDTH   640
#define GSCREEN_HEIGHT  480

#endif /* CFG_VBE */
#define SCREEN_WIDTH    yi_g_nTtyWidth/* (GSCREEN_WIDTH/ASCII_WIDTH)*/
#define SCREEN_HEIGHT   yi_g_nTtyHeight/* (GSCREEN_HEIGHT/ASCII_HEIGHT)*/

#else /* CFG_GUI == 0 */
#define SCREEN_WIDTH	80
#define SCREEN_HEIGHT	25
#endif /* CFG_VBE */

#define BASECOLOR       0x00000000
/*
#define BASECOLOR       0x00475790
*/
STATIC INT32 g_cursor_x = 0, g_cursor_y = 0;

INT32 yi_g_nTtyWidth = (GSCREEN_WIDTH/ASCII_WIDTH);
INT32 yi_g_nTtyHeight = (GSCREEN_HEIGHT/ASCII_HEIGHT);
INT32 yi_g_nTtyBKColor = BASECOLOR;
INT32 yi_g_nTtyFTColor = 0x00ffffff;


EXTERN VOID YIDrawTextOutEn(CHAR nChar, INT x, INT y, UINT32 nColorFT, UINT32 nColorBK);
STATIC VOID INLINE YIDrawAnChar(CHAR nChar, INT x, INT y, UINT32 nColorFT, UINT32 nColorBK);


#define CL_R_SIZE   5
#define CL_G_SIZE   6
#define CL_B_SIZE   5

#define CL_R_MASK   0xF800
#define CL_G_MASK   0x07E0
#define CL_B_MASK   0x001F

#define CL24_R_MASK   0x00ff0000
#define CL24_G_MASK   0x0000ff00
#define CL24_B_MASK   0x000000ff


STATIC UINT16 YIDrawColor24To16(UINT32 nColor)
{
/*    return (((nColor>>16)>>(8-CL_R_SIZE))<<(CL_G_SIZE+CL_B_SIZE)) |
    ((((nColor>>8)&0x0000ff)>>(8-CL_G_SIZE))<<(CL_B_SIZE)) |
    ((nColor&0x0000ff)>>(8-CL_B_SIZE));*/
    return (((nColor & CL24_R_MASK) >> 8)&CL_R_MASK) |
        (((nColor & CL24_G_MASK)>>CL_R_SIZE)&CL_G_MASK) |
        ((nColor & CL24_B_MASK) >> (8-CL_B_SIZE));
}

STATIC UINT32 YIDrawColor16To24(UINT16 nColor)
{
    UINT32 nCl = nColor;
    return (nCl&CL_R_MASK)<<8 | (nCl&CL_G_MASK)<<CL_R_SIZE |
        (nCl&CL_B_MASK)<<(8-CL_B_SIZE);
}

STATIC VOID YICALL YIMoveScreen()
{

    PUINT32 pScreenAddress = (PUINT32)YI_KE_VBE_VADDR;
    UINT nCountX, nCountY;
    UINT16 nBKColor = YIDrawColor24To16(yi_g_nTtyBKColor);


    for(nCountY = 0; nCountY < (SCREEN_HEIGHT-1)*ASCII_HEIGHT; nCountY++)
    {
       pScreenAddress = (PUINT32)YI_KE_VBE_VADDR + nCountY * GSCREEN_WIDTH/2;
       for(nCountX = 0; nCountX < SCREEN_WIDTH*ASCII_WIDTH/2; nCountX++)
        {
           *pScreenAddress = *(pScreenAddress + GSCREEN_WIDTH*ASCII_HEIGHT/2);
            pScreenAddress++;
        }
    }
    for(; nCountY < GSCREEN_HEIGHT; nCountY++)
    {
       pScreenAddress = (PUINT32)YI_KE_VBE_VADDR + nCountY * GSCREEN_WIDTH/2;
       for(nCountX = 0; nCountX < SCREEN_WIDTH*ASCII_WIDTH/2; nCountX++)
        {
            *(PUINT16)pScreenAddress = nBKColor;
            *((PUINT16)pScreenAddress+1) = nBKColor;

            pScreenAddress++;
        }
    }

	return;
}

void update_cursor(int row, int col)
{
    YIDrawAnChar('_', col, row,  yi_g_nTtyFTColor, yi_g_nTtyBKColor);
}

EXTERN CHAR yi_g_Fnt8x16;

STATIC VOID INLINE YIDrawAnChar(CHAR nChar, INT x, INT y, UINT32 nColorFT, UINT32 nColorBK)
{
    YIDrawTextOutEn(nChar, x*ASCII_WIDTH, y*ASCII_HEIGHT, nColorFT, nColorBK);
}

EXTERN VOID YIDrawTextOutEn(CHAR nChar, INT x, INT y, UINT32 nColorFT, UINT32 nColorBK)
{
    PCHAR   pFntPos;
    UINT32  nPosX, nPosY;
    PUINT16 pScreenPos = (PUINT16)YI_KE_VBE_VADDR + y * GSCREEN_WIDTH + x;

    UINT16 nFTColor = YIDrawColor24To16(nColorFT);
    UINT16 nBKColor = YIDrawColor24To16(nColorBK);

    pFntPos = ((PCHAR)&yi_g_Fnt8x16) + nChar * 16;
    for(nPosY = 0; nPosY < ASCII_HEIGHT; nPosY++)
    {
        for(nPosX = 0; nPosX < ASCII_WIDTH; nPosX++)
        {
            if((*pFntPos) & 0x01<<(ASCII_WIDTH -1 - nPosX))
                *pScreenPos++ = nFTColor;
            else
                *pScreenPos++ = nBKColor;
        }
        pFntPos ++;
        pScreenPos += GSCREEN_WIDTH-ASCII_WIDTH;
    }
}

EXTERN CHAR yi_g_HzFnt16x16;
EXTERN VOID YIDrawTextOutHz(UINT8 nChar1, UINT8 nChar2, INT x, INT y, UINT32 nColorFT, UINT32 nColorBK)
{
    PUINT16 pScreenPos;
    UINT32 nPosX, nPosY;
    PCHAR pFntPos;
    UINT16 nFTColor = YIDrawColor24To16(nColorFT);
    UINT16 nBKColor = YIDrawColor24To16(nColorBK);


    /* convent interior code to QW code */
    nChar1 -= 0xA0;
    nChar2 -= 0xA0;
    
    /* GB2312-80 have 87 code area. */
    if(nChar1 > 87)
        return;   
 
    pScreenPos = (PUINT16)YI_KE_VBE_VADDR + y * GSCREEN_WIDTH + x ;

    pFntPos = ((PCHAR)&yi_g_HzFnt16x16)+ ((nChar1-1)*94 + (nChar2-1))* 32;

    for(nPosY = 0; nPosY < ASCII_HEIGHT; nPosY++)
    {
        for(nPosX = 0; nPosX < ASCII_WIDTH; nPosX++)
        {
            if((*pFntPos) & 0x01<<(ASCII_WIDTH -1 - nPosX))
                *pScreenPos++ = nFTColor;
            else
                *pScreenPos++ = nBKColor;
        }
        pFntPos ++;
        for(nPosX = 0; nPosX < ASCII_WIDTH; nPosX++)
        {
            if((*pFntPos) & 0x01<<(ASCII_WIDTH -1 - nPosX))
                *pScreenPos++ = nFTColor;
            else
                *pScreenPos++ = nBKColor;
        }
        pFntPos ++;

        pScreenPos += GSCREEN_WIDTH-2*ASCII_WIDTH;
    }
}
STATIC VOID INLINE YIDrawHzChar(UINT8 nChar1, UINT8 nChar2, INT x, INT y, UINT32 nColorFT, UINT32 nColorBK)
{
    YIDrawTextOutHz(nChar1, nChar2, x*ASCII_WIDTH , y * ASCII_HEIGHT , nColorFT, nColorBK);
}

EXTERN INT  YIDriverCom2OutChar(CHAR nChar);        
EXTERN UINT32 STDCALL LdDrawString(PCSTR pString, UINT8 color)
{
    /* INT nHzChar = 0;*/
	while(*pString)
	{
        /* send char to serial port. */
        YIDriverCom2OutChar(*pString);

		if(*pString == '\n')
		{
			YIDrawAnChar('\0', g_cursor_x, g_cursor_y, yi_g_nTtyFTColor, yi_g_nTtyBKColor);
            		g_cursor_x = 0;
            
			if(++g_cursor_y >= SCREEN_HEIGHT)
			{
				YIMoveScreen();
				g_cursor_y--;
			}
		}
		else
		{
            /* this is't hz code begin? */
            if((UINT8)*pString < 128 || (UINT8)*(pString+1) < 128)
            {
                YIDrawAnChar(*pString, g_cursor_x, g_cursor_y, yi_g_nTtyFTColor, yi_g_nTtyBKColor);

			    g_cursor_x++;
			    g_cursor_y = (g_cursor_y + (g_cursor_x / SCREEN_WIDTH));
			    if(g_cursor_y >= SCREEN_HEIGHT)
			    {
				    YIMoveScreen();
				    g_cursor_y--;
			    }
			    g_cursor_x = g_cursor_x % SCREEN_WIDTH;
            }else /* hz code display. */
            {
               /* half code ?*/
               if(g_cursor_x + 1 >= SCREEN_WIDTH)
               {
                    /* Show blank. */
                    YIDrawAnChar('\0', g_cursor_x, g_cursor_y, yi_g_nTtyFTColor, yi_g_nTtyBKColor);
                    g_cursor_x = 0;
                    g_cursor_y++;
			        if(g_cursor_y >= SCREEN_HEIGHT)
			        {
				        YIMoveScreen();
				        g_cursor_y--;
			        }
               }
                YIDrawHzChar(*pString, *(pString+1), g_cursor_x, g_cursor_y, yi_g_nTtyFTColor, yi_g_nTtyBKColor);
                g_cursor_x++;
                g_cursor_x++;
 			    g_cursor_y = (g_cursor_y + (g_cursor_x / SCREEN_WIDTH));
			    if(g_cursor_y >= SCREEN_HEIGHT)
			    {
				    YIMoveScreen();
				    g_cursor_y--;
			    }
			    g_cursor_x = g_cursor_x % SCREEN_WIDTH;
                pString++;
           }
		}
		pString++;
	}
	update_cursor(g_cursor_y, g_cursor_x);
	return 0;
}

EXTERN VOID YIDrawLineH(INT x1, INT x2, INT y, UINT32 color)
{
    PUINT16 pScreenPos = (PUINT16)YI_KE_VBE_VADDR;
    UINT    nOffset = x2-x1;

    UINT16 nColor = YIDrawColor24To16(color);


    if(nOffset >= 0)
    {
        pScreenPos += y *  GSCREEN_WIDTH + x1;
    }
    else
    {
        pScreenPos += y * GSCREEN_WIDTH + x2;
        nOffset = -nOffset; 
    }
    while(nOffset > 0)
    {
        *pScreenPos++ = nColor;
        nOffset--;
    }
}

EXTERN VOID YIDrawLineV(INT x, INT y1, INT y2, UINT32 color)
{
    PUINT16 pScreenPos = (PUINT16)YI_KE_VBE_VADDR;
    UINT    nOffset = y2-y1;
    UINT16 nColor = YIDrawColor24To16(color);

    if(nOffset >= 0)
    {
        pScreenPos += y1 *  GSCREEN_WIDTH + x;
    }
    else
    {
        pScreenPos += y2 * GSCREEN_WIDTH + x;
        nOffset = -nOffset; 
    }
    while(nOffset > 0)
    {
        *pScreenPos = nColor;
        pScreenPos += GSCREEN_WIDTH;
        nOffset--;
    }
}

EXTERN VOID YIDrawPoint(INT x, INT y, UINT32 color)
{
    PUINT16 pScreenPos = (PUINT16)YI_KE_VBE_VADDR; 
    UINT16 nColor = YIDrawColor24To16(color);

    pScreenPos += y * GSCREEN_WIDTH + x;
    *pScreenPos = nColor;
}

EXTERN UINT32 YIDrawReadPoint(INT x, INT y)
{
    PUINT16 pScreenPos = (PUINT16)YI_KE_VBE_VADDR;
    UINT16 nColor;

    pScreenPos += y * GSCREEN_WIDTH + x;
    
    nColor = *pScreenPos;
    /*
    return YIDrawColor16To24(nColor);
    */
    /*if(nColor)
    {
        UINT32 nColorOut;
        YIRtlKernelMsg("<In:%08x>", nColor);
        nColorOut = YIDrawColor16To24(nColor);
        YIRtlKernelMsg("<Out:%08x>", nColorOut);
        return nColorOut;
    }*/
 
    return YIDrawColor16To24(nColor);
}

EXTERN VOID YIDrawLine(INT iX, INT iY, INT iToX, INT iToY, UINT32 iColor)
{

   INT iCount;
   INT iOffsetX = iToX-iX;
   INT iOffsetY = iToY-iY;

/*   if(iOffsetY == 0)
   {
      YIDrawLineH(iX, iToX,iY, iColor);
      return ;
   }
   if(iOffsetX == 0)
   {
      YIDrawLineV(iX, iY, iToY, iColor);
      return ;
   }
*/

   if((iOffsetX>0?iOffsetX:-iOffsetX) > (iOffsetY>0?iOffsetY:-iOffsetY))
   {
      if(iOffsetX >0)
         for(iCount = iX; iCount <= iToX; iCount++)
            YIDrawPoint(iCount, iY+iOffsetY*(iCount-iX)/iOffsetX, iColor);
      else  
         for(iCount = iToX; iCount <= iX; iCount++)
            YIDrawPoint(iCount, iToY+iOffsetY*(iCount-iToX)/iOffsetX, iColor);
   }        
   else
   {
      
      if(iOffsetY >0)
         for(iCount = iY; iCount <= iToY; iCount++)
            YIDrawPoint(iX+iOffsetX*(iCount-iY)/iOffsetY, iCount, iColor);
      else  
         for(iCount = iToY; iCount <= iY; iCount++)
            YIDrawPoint(iToX+iOffsetX*(iCount-iToY)/iOffsetY, iCount, iColor);
   }        

   return;
}

EXTERN VOID YIDrawFillRect(INT x1, INT y1, INT x2, INT y2, UINT32 color)
{
    UINT16 nColor = YIDrawColor24To16(color);
    PUINT16 pScreenPos;
    INT nTemp, i, j;

    if(x1 > x2)
    {
        nTemp = x1;
        x1 = x2;
        x2 = nTemp;
    }
    if(y1 > y2)
    {
        nTemp = y1;
        y1 = y2;
        y2 = nTemp;
    }


    for(j = y1; j < y2; j++)
    {
        pScreenPos = (PUINT16)YI_KE_VBE_VADDR + j * GSCREEN_WIDTH + x1 ;
        for(i = x1; i < x2; i++)
        {
            *pScreenPos++ = nColor;
        }
    }
    

   return ;
}

EXTERN VOID YIDrawWindow(INT iX, INT iY, INT iToX, INT iToY, UINT32 dwStyle)
{
   YIDrawLineH(iX, iToX, iY, 0x00a8a8a8);
   YIDrawLineV(iX, iY, iToY, 0x00a8a8a8);
   
   YIDrawLineV(iToX, iY+1, iToY, 0x0);
   YIDrawLineH(iX+1, iToX, iToY, 0x0);
   
   YIDrawLineH(iX+1, iToX-1, iY+1, 0x00ffffff);
   YIDrawLineV(iX+1, iY+1, iToY-1, 0x00ffffff);

   YIDrawLineV(iToX-1, iY+1, iToY-1, 0x00545454);
   YIDrawLineH(iX+1, iToX-1, iToY-1, 0x00545454);

   if(dwStyle != 1)
   YIDrawFillRect(iX+2, iY+2, iToX-1, iToY-1, 0x007f7f7f);

   if(dwStyle == 2)
      return ;

   YIDrawFillRect(iX+2, iY+2, iToX-1, iY+3+18, 0x000058e6);

   YIDrawLineH(iToX-6-15, iToX-6, iY+5, 0x00ffffff);
   YIDrawLineV(iToX-6-15, iY+5, iY+5+14, 0x00ffffff);
   YIDrawLineH(iToX-6-15, iToX-6, iY+5+14, 0x0);
   YIDrawLineV(iToX-6, iY+5, iY+5+14, 0x0);

   YIDrawLineH(iToX-6-14, iToX-6-1, iY+5+13, 0x00545454);
   YIDrawLineV(iToX-6-1, iY+5+1, iY+5+13, 0x00545454);

   YIDrawFillRect(iToX-6-14, iY+5+1, iToX-6-1, iY+5+13, 0x007f7f7f);

   YIDrawLine(iToX-6-15+4, iY+5+3, iToX-6-4, iY+5+14-4, 0x0);
   YIDrawLine(iToX-6-15+4, iY+5+14-4, iToX-6-4, iY+5+3, 0x0);

   return ;
}
/*
EXTERN VOID YIDrawWindow(INT iX, INT iY, INT iToX, INT iToY, UINT32 dwStyle)
{
   YIDrawLineH(iX, iToX, iY, 0x00a8a8a8);
   YIDrawLineV(iX, iY, iToY, 0x00a8a8a8);
   
   YIDrawLineV(iToX, iY+1, iToY, 0x0);
   YIDrawLineH(iX+1, iToX, iToY, 0x0);
   
   YIDrawLineH(iX+1, iToX-1, iY+1, 0x00ffffff);
   YIDrawLineV(iX+1, iY+1, iToY-1, 0x00ffffff);

   YIDrawLineV(iToX-1, iY+1, iToY-1, 0x00545454);
   YIDrawLineH(iX+1, iToX-1, iToY-1, 0x00545454);

   if(dwStyle != 1)
   YIDrawFillRect(iX+2, iY+2, iToX-1, iToY-1, 0x007f7f7f);

   if(dwStyle == 2)
      return ;

   YIDrawFillRect(iX+3, iY+3, iToX-2, iY+3+18, 0x000058e6);

   YIDrawLineH(iToX-6-15, iToX-6, iY+5, 0x00ffffff);
   YIDrawLineV(iToX-6-15, iY+5, iY+5+14, 0x00ffffff);
   YIDrawLineH(iToX-6-15, iToX-6, iY+5+14, 0x0);
   YIDrawLineV(iToX-6, iY+5, iY+5+14, 0x0);

   YIDrawLineH(iToX-6-14, iToX-6-1, iY+5+13, 0x00545454);
   YIDrawLineV(iToX-6-1, iY+5+1, iY+5+13, 0x00545454);

   YIDrawFillRect(iToX-6-14, iY+5+1, iToX-6-1, iY+5+13, 0x007f7f7f);

   YIDrawLine(iToX-6-15+4, iY+5+3, iToX-6-4, iY+5+14-4, 0x0);
   YIDrawLine(iToX-6-15+4, iY+5+14-4, iToX-6-4, iY+5+3, 0x0);

   return ;
}
*/
