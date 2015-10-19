/*
 * Copyright (c) 1999, 2000 Greg Haerr <greg@censoft.com>
 *
 * EGA/VGA 16 color 4 planes Screen Driver, direct hardware or bios
 * 	If HWINIT 1 is #defined, the file vgainit.c is
 * 	used to provide non-bios direct hw initialization of the VGA
 * 	chipset.  Otherwise, this driver uses int10 bios to 
 * 	set/reset graphics/text modes.
 *
 * 	If ROMFONT is #defined, the file romfont.c is used to
 * 	draw all fonts, afte the bios is called to
 * 	get the address of the ROM character font which
 * 	is used for the character bitmaps.  Otherwise, the
 * 	file genfont.c is used to draw linked in fonts, without
 * 	consulting the bios.
 *
 * 	All other access to the hardware is controlled through this driver.
 *
 * 	Blitting enabled with #define HAVEBLIT in vgaplan4.h
 *
 * 	This driver links with one of two other files, vgaplan4.c,
 * 	the portable VGA 4 planes 16 color driver, or asmplan4.s, which
 * 	is 8086 assembly language for speed.  This file itself
 * 	doesn't know about any planar or packed arrangement, relying soley
 * 	on the following external routines for all graphics drawing:
 * 		ega_init, ega_drawpixel, ega_readpixel,
 * 		ega_drawhorzline, ega_drawvertline
 *
 * 	All text/font drawing code is based on the above routines and
 * 	the included entry points for getting the ROM bitmap data.
 *
 * 	If the environment variable EGAMODE is set, the driver implements
 *	the EGA 640x350 (mode 10h) resolution, otherwise 640x480 (mode 12h)
 *	graphics mode is set.
 *
 *	The environment variable CHARHEIGHT if set will set the assumed rom
 *	font character height, which defaults to 14.
 *  For YingOS SVGA screen drvier.
 *
 */

#include "mwtypes.h"
#include "device.h"
#include "genfont.h"


/* VBE 800x600x16 define */
#define GSCREEN_WIDTH   800
#define GSCREEN_HEIGHT  600
EXTERN VOID YIDrawLineH(INT x1, INT x2, INT y, UINT32 color);
EXTERN VOID YIDrawLineV(INT x, INT y1, INT y2, UINT32 color);
EXTERN VOID YIDrawPoint(INT x, INT y, UINT32 color);
EXTERN UINT32 YIDrawReadPoint(INT x, INT y);

EXTERN VOID YIDrawFillRect(INT x1, INT y1, INT x2, INT y2, UINT32 color);


/* VGA driver entry points*/
static PSD  SVGA_open(PSD psd);
static void SVGA_close(PSD psd);
static void SVGA_getscreeninfo(PSD psd,PMWSCREENINFO psi);

static void SVGA_setpalette(PSD psd,int first,int count,MWPALENTRY *pal);

static void SVGA_drawpixel(PSD psd, MWCOORD x, MWCOORD y, MWPIXELVAL c);
static MWPIXELVAL SVGA_readpixel(PSD psd, MWCOORD x, MWCOORD y);
static void SVGA_drawhline(PSD psd, MWCOORD x1, MWCOORD x2, MWCOORD y, MWPIXELVAL c);
static void SVGA_drawvline(PSD psd, MWCOORD x, MWCOORD y1, MWCOORD y2, MWPIXELVAL c);
static void SVGA_fillrect(PSD psd, MWCOORD x1, MWCOORD y1, MWCOORD x2, MWCOORD y2, MWPIXELVAL c); 

/* static MWBOOL SVGA_mapmemgc(PSD mempsd,MWCOORD w,MWCOORD h,int planes,int bpp,
		int linelen,int size,void *addr);*/

static void SVGA_NULL_blit(PSD dstpsd, MWCOORD dstx, MWCOORD dsty, MWCOORD w,
		MWCOORD h, PSD srcpsd, MWCOORD srcx, MWCOORD srcy, long op) {}

static PSD  SVGA_NULL_allocatememgc(PSD psd) { return NULL; }

static MWBOOL SVGA_NULL_mapmemgc(PSD mempsd,MWCOORD w,MWCOORD h,int planes,int bpp,
			int linelen,int size,void *addr) { return 0; }

static void SVGA_NULL_freememgc(PSD mempsd) {}

SCREENDEVICE	scrdev = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL,
	SVGA_open,
	SVGA_close,
	SVGA_getscreeninfo,
	SVGA_setpalette,
	SVGA_drawpixel,
	SVGA_readpixel,
	SVGA_drawhline,
	SVGA_drawvline,
	SVGA_fillrect,
	gen_fonts,
	SVGA_NULL_blit,		/* Blit*/
	NULL,			/* PreSelect*/
	NULL,			/* DrawArea*/
	NULL,			/* SetIOPermissions*/
	SVGA_NULL_allocatememgc,
	SVGA_NULL_mapmemgc,
	SVGA_NULL_freememgc
};

static PSD
SVGA_open(PSD psd)
{
	/* enter graphics mode*/
	/* ega_hwinit();*/
    
	/* init driver variables depending on ega/vga mode*/
	psd->xres = psd->xvirtres = GSCREEN_WIDTH;
	psd->yres = psd->yvirtres = GSCREEN_HEIGHT;
	psd->planes = 1;
	psd->bpp = 16;
	psd->ncolors = 65536;
	psd->pixtype =  MWPF_TRUECOLOR565;
#if HAVEBLIT
	psd->flags = PSF_SCREEN | PSF_HAVEBLIT;
#else
	psd->flags = PSF_SCREEN;
#endif

	/* init planes driver (sets psd->addr and psd->linelen)*/
	//ega_init(psd);

	return psd;
}

static void
SVGA_close(PSD psd)
{
    
}

static void
SVGA_getscreeninfo(PSD psd,PMWSCREENINFO psi)
{
	psi->rows = psd->yvirtres;
	psi->cols = psd->xvirtres;
	psi->planes = psd->planes;
	psi->bpp = psd->bpp;
	psi->ncolors = psd->ncolors;
	psi->pixtype = psd->pixtype;
	psi->fonts = NUMBER_FONTS;
	psi->xdpcm = 33;		/* assumes screen width of 24 cm*/
	psi->ydpcm = 33;		/* assumes screen height of 18 cm*/
#if 0
	if(VGAMODE) {
		/* VGA 640x480*/
		psi->xdpcm = 27;	/* assumes screen width of 24 cm*/
		psi->ydpcm = 27;	/* assumes screen height of 18 cm*/
	} else {
		/* EGA 640x350*/
		psi->xdpcm = 27;	/* assumes screen width of 24 cm*/
		psi->ydpcm = 19;	/* assumes screen height of 18 cm*/
	}

#if ETA4000
	/* SVGA 800x600*/
	psi->xdpcm = 33;		/* assumes screen width of 24 cm*/
	psi->ydpcm = 33;		/* assumes screen height of 18 cm*/
#endif
#endif /* 0 */
}

static void
SVGA_setpalette(PSD psd,int first,int count,MWPALENTRY *pal)
{
	/* not yet implemented, std 16 color palette assumed*/
}

#if HAVEBLIT
/* initialize memory device with passed parms, and select suitable fb driver*/
static MWBOOL
SVGA_mapmemgc(PSD mempsd,MWCOORD w,MWCOORD h,int planes,int bpp,int linelen,
	int size,void *addr)
{
	extern SUBDRIVER memplan4;

	/* initialize mem screen driver*/
	initmemgc(mempsd, w, h, planes, bpp, linelen, size, addr);

	/* set and initialize subdriver into mem screen driver*/
	if (!set_subdriver(mempsd, &memplan4, TRUE))
		return 0;

	return 1;
}
#endif


static void SVGA_drawpixel(PSD psd, MWCOORD x, MWCOORD y, MWPIXELVAL c)
{
    YIDrawPoint(x, y, c);
}

static MWPIXELVAL SVGA_readpixel(PSD psd, MWCOORD x, MWCOORD y)
{
    return YIDrawReadPoint(x, y);
}

static void SVGA_drawhline(PSD psd, MWCOORD x1, MWCOORD x2, MWCOORD y, MWPIXELVAL c)
{
    YIDrawLineH(x1, x2, y, c);
}

static void SVGA_drawvline(PSD psd, MWCOORD x, MWCOORD y1, MWCOORD y2, MWPIXELVAL c)
{
    YIDrawLineV(x, y1, y2, c);
}

static void SVGA_fillrect(PSD psd, MWCOORD x1, MWCOORD y1, MWCOORD x2, MWCOORD y2, MWPIXELVAL c)
{
    YIDrawFillRect(x1, y1, x2, y2, c);
}

