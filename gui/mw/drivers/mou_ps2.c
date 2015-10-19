/*
 * Copyright (c) 1999 Greg Haerr <greg@censoft.com>
 *
 * NULL Mouse Driver
 */
#include <stdio.h>
#include "device.h"

#define	SCALE		3	/* default scaling factor for acceleration */
#define	THRESH		5	/* default threshhold for acceleration */

static int  	PS2M_Open(MOUSEDEVICE *pmd);
static void 	PS2M_Close(void);
static int  	PS2M_GetButtonInfo(void);
static void	    PS2M_GetDefaultAccel(int *pscale,int *pthresh);
static int  	PS2M_Read(MWCOORD *dx, MWCOORD *dy, MWCOORD *dz, int *bp);
static int  	PS2M_Poll(void);

MOUSEDEVICE mousedev = {
	PS2M_Open,
	PS2M_Close,
	PS2M_GetButtonInfo,
	PS2M_GetDefaultAccel,
	PS2M_Read,
	PS2M_Poll
};

/*
 * Poll for events
 */

static int
PS2M_Poll(void)
{
  return Ps2MicePoll();
  //return 1;
}

/*
 * Open up the mouse device.
 */
static int
PS2M_Open(MOUSEDEVICE *pmd)
{
	//return -2;	/* no mouse*/
    return 9;
}

/*
 * Close the mouse device.
 */
static void
PS2M_Close(void)
{
}

/*
 * Get mouse buttons supported
 */
static int
PS2M_GetButtonInfo(void)
{
    return MWBUTTON_L | MWBUTTON_R;
}

/*
 * Get default mouse acceleration settings
 */
static void
PS2M_GetDefaultAccel(int *pscale,int *pthresh)
{
	*pscale = SCALE;
	*pthresh = THRESH;
}
/*
#define SCREEN_WIDTH    800
#define SCREEN_HIGHT    600
*/
/*
 * Attempt to read bytes from the mouse and interpret them.
 * Returns -1 on error, 0 if either no bytes were read or not enough
 * was read for a complete state, or 1 if the new state was read.
 * When a new state is read, the current buttons and x and y deltas
 * are returned.  This routine does not block.
 */
static int
PS2M_Read(MWCOORD *dx, MWCOORD *dy, MWCOORD *dz, int *bp)
{
    INT vdx, vdy, vdz, vdb;
    INT buttons;

    if(Ps2MiceRead(&vdx, &vdy, &vdz, &vdb))
    {
        if(vdx <-10)
            vdx = -10;
        if(vdy < -10)
            vdy = -10;
        if(vdx >10)
            vdx = 10;
        if(vdy >10)
            vdy = 10;

        buttons = 0;
        if(vdb & 0x1)
            buttons |= MWBUTTON_L;
        if(vdb & 0x2)
            buttons |= MWBUTTON_R;
        *bp = buttons;
        
        *dx = vdy;
        *dy = -vdx;
        *dz = 0;
        /* YIRtlKernelMsg("<%2x,%2x,%2x>", vdx, vdy, vdb);*/
        return 1;
    }

    //YIRtlKernelMsg("[%d,%d]", x, y);
    /**dx = Rand()%(200);
    *dy = Rand()%(200);*/
    
	return 0;
}

