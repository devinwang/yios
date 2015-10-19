/****************************************************************************

		THIS SOFTWARE IS NOT COPYRIGHTED

   HP offers the following for use in the public domain.  HP makes no
   warranty with regard to the software or it's performance and the
   user accepts the software "AS IS" with all faults.

   HP DISCLAIMS ANY WARRANTIES, EXPRESS OR IMPLIED, WITH REGARD
   TO THIS SOFTWARE INCLUDING BUT NOT LIMITED TO THE WARRANTIES
   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

****************************************************************************/

/****************************************************************************
 *  Header: remcom.c,v 1.34 91/03/09 12:29:49 glenne Exp $
 *
 *  Module name: remcom.c $
 *  Revision: 1.34 $
 *  Date: 91/03/09 12:29:49 $
 *  Contributor:     Lake Stevens Instrument Division$
 *
 *  Description:     low level support for gdb debugger. $
 *
 *  Considerations:  only works on target hardware $
 *
 *  Written by:      Glenn Engel $
 *  ModuleState:     Experimental $
 *
 *  NOTES:           See Below $
 *
 *  Modified for 386 by Jim Kingdon, Cygnus Support.
 *
 *  To enable debugger support, two things need to happen.  One, a
 *  call to set_debug_traps() is necessary in order to allow any breakpoints
 *  or error conditions to be properly intercepted and reported to gdb.
 *  Two, a breakpoint needs to be generated to begin communication.  This
 *  is most easily accomplished by a call to breakpoint().  Breakpoint()
 *  simulates a breakpoint by executing a trap #1.
 *
 *  The external function exceptionHandler() is
 *  used to attach a specific handler to a specific 386 vector number.
 *  It should use the same privilege level it runs at.  It should
 *  install it as an interrupt gate so that interrupts are masked
 *  while the handler runs.
 *
 *  Because gdb will sometimes write to the stack area to execute function
 *  calls, this program cannot rely on using the supervisor stack so it
 *  uses it's own stack area reserved in the int array remcomStack.
 *
 *************
 *
 *    The following gdb commands are supported:
 *
 * command          function                               Return value
 *
 *    g             return the value of the CPU registers  hex data or ENN
 *    G             set the value of the CPU registers     OK or ENN
 *
 *    mAA..AA,LLLL  Read LLLL bytes at address AA..AA      hex data or ENN
 *    MAA..AA,LLLL: Write LLLL bytes at address AA.AA      OK or ENN
 *
 *    c             Resume at current address              SNN   ( signal NN)
 *    cAA..AA       Continue at address AA..AA             SNN
 *
 *    s             Step one instruction                   SNN
 *    sAA..AA       Step one instruction from AA..AA       SNN
 *
 *    k             kill
 *
 *    ?             What was the last sigval ?             SNN   (signal NN)
 *
 * All commands and responses are sent with a packet which includes a
 * checksum.  A packet consists of
 *
 * $<packet info>#<checksum>.
 *
 * where
 * <packet info> :: <characters representing the command or response>
 * <checksum>    :: < two hex digits computed as modulo 256 sum of <packetinfo>>
 *
 * When a packet is received, it is first acknowledged with either '+' or '-'.
 * '+' indicates a successful transfer.  '-' indicates a failed transfer.
 *
 * Example:
 *
 * Host:                  Reply:
 * $m0,10#2a               +$00010203040506070809101112131415#42
 *
 ****************************************************************************/

#include <yios/config.h>
#include <yios/types.h>
#include <yios/errno.h>

#include <yios/hal/cpu.h>
#include <yios/hal/init.h>
#include <yios/hal/io.h>
#include <yios/hal/int.h>



/* BreakPoint type. */
#define BKT_HW		1	/* Hardware breakpoint */
#define BKT_WT		2	/* Write breakpoint */
#define BKT_RW		4	/* Access breakpoint */
static unsigned long yi_g_HwBreakPointAddr[4] = {0, 0, 0,0 };

static int SetHardwarePoint(unsigned long pAddr, unsigned long nType, unsigned long nSize)
{
    int nCount = 0;
    
    for(nCount = 0; nCount < 4; nCount++)
    {
        /* Have empty drx */
	if(yi_g_HwBreakPointAddr[nCount] == 0)
	{
	    unsigned long nSet;

	/*YIRtlKernelMsg("\ndbg pAddr:0x%0.8x, nType:0x%0.8x, nSize:0x%0.8x", pAddr, nType, nSize);*/
	    
	    yi_g_HwBreakPointAddr[nCount] = pAddr;

	    __asm__("movl %%dr7, %0": "=r"(nSet):);

	    /* clear break type and length. */
	    nSet &= ~(0xfUL << (16 + nCount*4));

	    /* set break type and length. */
	    nSet |= ((nType - 1) | ((nSize - 1) << 2)) << (16 + nCount*4);

	    /* set GL bit. */
	    nSet |= (0x3UL)<<(nCount*4);

	    /* set GE/LE bit. */
	    nSet |= (0x3UL)<<8;
	    
            __asm__(
            "movl %0, %%dr0\n\t"
            "movl %1, %%dr7\n\t"
            :
            :"r"(pAddr), "r"(nSet)
            );
	    return 0;
	}
    }
    return 1;
}

static int ClearHardwarePoint(unsigned long pAddr)
{
    int nCount = 0;
   
    for(nCount = 0; nCount < 4; nCount++)
    {
        /* Have empty drx */
	if(yi_g_HwBreakPointAddr[nCount] == pAddr)
	{
	    unsigned long nSet;
	    yi_g_HwBreakPointAddr[nCount] = 0;
	    pAddr = 0;

	    __asm__("movl %%dr7, %0": "=r"(nSet):);

	    /* clear break type and length. */
	    nSet &= ~(0xfUL << (16 + nCount*4));

	    /* clear GL bit. */
	    nSet &= ~((0x3UL)<<(nCount*4)); 
	    
	    /* if no breakpoint clear GE/LE bit. */
	    if(yi_g_HwBreakPointAddr[0] == 0 &&
	        yi_g_HwBreakPointAddr[1] == 0 &&
		yi_g_HwBreakPointAddr[2] == 0 &&
		yi_g_HwBreakPointAddr[3] == 0)
		nSet &= ~((0x3UL)<<8);

            __asm__(
            "movl %0, %%dr0\n\t"
            "movl %1, %%dr7\n\t"
            :
            :"r"(pAddr), "r"(nSet)
            );
	    return 0;
	}
    }
    return 1;
}
/*static void ShowEsp()
{
	unsigned int pESP =0;

  	__asm__ __volatile__("movl %%esp, %0":"=m"(pESP));
  
  	YIRtlKernelMsg("\nStackPtr:0x%0.8x", pESP);
}*/



static unsigned char* strcpy(unsigned char* to, unsigned char* from)
{
	do
	{
		*to = *from;
		to++;
		from++;
	}while(*from);
	return to;
}
#define	IntDisableExtInt()	do{__asm__("cli");}while(0)
#define	IntEnableExtInt()	do{__asm__("sti");}while(0)




/************************************************************************
 *
 * external low-level support routines
 */

static void putDebugChar();	/* write a single character      */
static int getDebugChar();	/* read and return a single char */
static  void exceptionHandler();	/* assign an exception handler   */

static  void breakpoint (void);


static void set_debug_traps (void);



#define com1 0x3f8 
#define com2 0x2f8

#define combase com1

#define outb	YIIoWritePortByte
#define inb	YIIoReadPortByte

static void init_serial(void)
{
#if 0
    outb(inb(combase + 3) | 0x80, combase + 3);
    outb(12, combase);                           /* 9600 bps, 8-N-1 */
    outb(0, combase+1);
    outb(inb(combase + 3) & 0x7f, combase + 3);
#else
    outb(0x80, combase+3);
    outb(0x0c, combase);
    outb(0, combase+1);
    outb(0x3a, combase+3);
    outb(0x03, combase+3);
    outb(inb(combase+5) &0xfe, combase+5);
    
#endif    

}

static int getDebugChar(void)
{
   while (!(inb(combase + 5) & 0x01));
   return inb(combase);
}

static void putDebugChar(int ch)
{
   while (!(inb(combase + 5) & 0x20));
   outb((char) ch, combase);
}


static  void exceptionHandler(int exc, void *addr)
{
	YIHalSetIntTblItem(exc, addr,YI_HAL_CPU_INTGATE, YI_HAL_CPU_SYSLEVEL);
}

/*static void flush_i_cache(void)
{
   __asm__ __volatile__ ("jmp 1f\n1:");
}*/

/*
void *memset(void *ptr, int val, unsigned int len)
{
   RtlFillMemory(ptr, len, val);
   return ptr;
}
*/

EXTERN YI_STATUS YICALL YIGdbstubInit()
{

   init_serial();

   set_debug_traps ();

   /* breakpoint ();*/

   return E_OK;
}


/************************************************************************/
/* BUFMAX defines the maximum number of characters in inbound/outbound buffers*/
/* at least NUMREGBYTES*2 are needed for register packets */
#define BUFMAX 400


static char initialized;  /* boolean flag. != 0 means we've been initialized */

static int     remote_debug;
/*  debug >  0 prints ill-formed commands in valid packets & checksum errors */

static const char hexchars[]="0123456789abcdef";

/* Number of registers.  */
#define NUMREGS	16

/* Number of bytes of registers.  */
#define NUMREGBYTES (NUMREGS * 4)

enum regnames {EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI,
	       PC /* also known as eip */,
	       PS /* also known as eflags */,
	       CS, SS, DS, ES, FS, GS};

/*
 * these should not be static cuz they can be used outside this module
 */
static int registers[NUMREGS];

#define STACKSIZE 1000
static int remcomStack[(STACKSIZE/sizeof(int))];
static int* stackPtr = &remcomStack[STACKSIZE/sizeof(int) - 4];


/***************************  ASSEMBLY CODE MACROS *************************/
/* 									   */

void
return_to_prog ();

/* Restore the program's registers (including the stack pointer, which
   means we get the right stack and don't have to worry about popping our
   return address and any stack frames and so on) and return.  */
__asm (".text");
__asm (".globl return_to_prog");
__asm ("return_to_prog:");
__asm ("        movw registers+44, %ss");
__asm ("        movl registers+16, %esp");
__asm ("        movl registers+4, %ecx");
__asm ("        movl registers+8, %edx");
__asm ("        movl registers+12, %ebx");
__asm ("        movl registers+20, %ebp");
__asm ("        movl registers+24, %esi");
__asm ("        movl registers+28, %edi");
__asm ("        movw registers+48, %ds");
__asm ("        movw registers+52, %es");
__asm ("        movw registers+56, %fs");
__asm ("        movw registers+60, %gs");
__asm ("        movl registers+36, %eax");
__asm ("        pushl %eax");  /* saved eflags */
__asm ("        movl registers+40, %eax");
__asm ("        pushl %eax");  /* saved cs */
__asm ("        movl registers+32, %eax");
__asm ("        pushl %eax");  /* saved eip */
__asm ("        movl registers, %eax");
/* use iret to restore pc and flags together so
   that trace flag works right.  */
__asm ("        iret");

/* #define BREAKPOINT() __asm ("   int $3");*/

/* Put the error code here just in case the user cares.  */
int gdb_i386errcode;
/* Likewise, the vector number here (since GDB only gets the signal
   number through the usual means, and that's not very specific).  */
static int gdb_i386vector = -1;
 
/* GDB stores segment registers in 32-bit words (that's just the way
   m-i386v.h is written).  So zero the appropriate areas in registers.  */
#define SAVE_REGISTERS1() \
  __asm ("movl %eax, registers");                                   	  \
  __asm ("movl %ecx, registers+4");			  		     \
  __asm ("movl %edx, registers+8");			  		     \
  __asm ("movl %ebx, registers+12");			  		     \
  __asm ("movl %ebp, registers+20");			  		     \
  __asm ("movl %esi, registers+24");			  		     \
  __asm ("movl %edi, registers+28");			  		     \
  __asm ("movw $0, %ax");							     \
  __asm ("movw %ds, registers+48");			  		     \
  __asm ("movw %ax, registers+50");					     \
  __asm ("movw %es, registers+52");			  		     \
  __asm ("movw %ax, registers+54");					     \
  __asm ("movw %fs, registers+56");			  		     \
  __asm ("movw %ax, registers+58");					     \
  __asm ("movw %gs, registers+60");			  		     \
  __asm ("movw %ax, registers+62");
#define SAVE_ERRCODE() \
  __asm ("popl %ebx");                                  \
  __asm ("movl %ebx, gdb_i386errcode");
#define SAVE_REGISTERS2() \
  __asm ("popl %ebx"); /* old eip */			  		     \
  __asm ("movl %ebx, registers+32");			  		     \
  __asm ("popl %ebx");	 /* old cs */			  		     \
  __asm ("movl %ebx, registers+40");			  		     \
  __asm ("movw %ax, registers+42");                                           \
  __asm ("popl %ebx");	 /* old eflags */		  		     \
  __asm ("movl %ebx, registers+36");			 		     \
  /* Now that we've done the pops, we can save the stack pointer.");  */   \
  __asm ("movw %ss, registers+44");					     \
  __asm ("movw %ax, registers+46");     	       	       	       	       	     \
  __asm ("movl %esp, registers+16");

/* See if mem_fault_routine is set, if so just IRET to that address.  */
#define CHECK_FAULT() \
  __asm ("cmpl $0, mem_fault_routine");					   \
  __asm ("jne mem_fault");

__asm (".text");
__asm ("mem_fault:");
/* OK to clobber temp registers; we're just going to end up in set_mem_err.  */
/* Pop error code from the stack and save it.  */
__asm ("     popl %eax");
__asm ("     movl %eax, gdb_i386errcode");

__asm ("     popl %eax"); /* eip */
/* We don't want to return there, we want to return to the function
   pointed to by mem_fault_routine instead.  */
__asm ("     movl mem_fault_routine, %eax");
__asm ("     popl %ecx"); /* cs (low 16 bits; junk in hi 16 bits).  */
__asm ("     popl %edx"); /* eflags */

/* Remove this stack frame; when we do the iret, we will be going to
   the start of a function, so we want the stack to look just like it
   would after a "call" instruction.  */
__asm ("     leave");

/* Push the stuff that iret wants.  */
__asm ("     pushl %edx"); /* eflags */
__asm ("     pushl %ecx"); /* cs */
__asm ("     pushl %eax"); /* eip */

/* Zero mem_fault_routine.  */
__asm ("     movl $0, %eax");
__asm ("     movl %eax, mem_fault_routine");

__asm ("iret");

#define CALL_HOOK() __asm ("call _remcomHandler");

/* This function is called when a i386 exception occurs.  It saves
 * all the cpu regs in the _registers array, munges the stack a bit,
 * and invokes an exception handler (remcom_handler).
 *
 * stack on entry:                       stack on exit:
 *   old eflags                          vector number
 *   old cs (zero-filled to 32 bits)
 *   old eip
 *
 */
void _catchException3();
__asm (".text");
__asm (".globl _catchException3");
__asm ("_catchException3:");
SAVE_REGISTERS1();
SAVE_REGISTERS2();
__asm ("pushl $3");
CALL_HOOK();

/* Same thing for exception 1.  */
void _catchException1();
__asm (".text");
__asm (".globl _catchException1");
__asm ("_catchException1:");
SAVE_REGISTERS1();
SAVE_REGISTERS2();
__asm ("pushl $1");
CALL_HOOK();

#if 0
/* Same thing for exception 0.  */
static void _catchException0();
__asm(".text");
__asm(".globl __catchException0");
__asm("__catchException0:");
SAVE_REGISTERS1();
SAVE_REGISTERS2();
__asm ("pushl $0");
CALL_HOOK();

/* Same thing for exception 4.  */
static void _catchException4();
__asm(".text");
__asm(".globl __catchException4");
__asm("__catchException4:");
SAVE_REGISTERS1();
SAVE_REGISTERS2();
__asm ("pushl $4");
CALL_HOOK();

/* Same thing for exception 5.  */
static void _catchException5();
__asm(".text");
__asm(".globl __catchException5");
__asm("__catchException5:");
SAVE_REGISTERS1();
SAVE_REGISTERS2();
__asm ("pushl $5");
CALL_HOOK();

/* Same thing for exception 6.  */
static void _catchException6();
__asm(".text");
__asm(".globl __catchException6");
__asm("__catchException6:");
SAVE_REGISTERS1();
SAVE_REGISTERS2();
__asm ("pushl $6");
CALL_HOOK();

/* Same thing for exception 7.  */
static void _catchException7();
__asm(".text");
__asm(".globl __catchException7");
__asm("__catchException7:");
SAVE_REGISTERS1();
SAVE_REGISTERS2();
__asm ("pushl $7");
CALL_HOOK();

/* Same thing for exception 8.  */
static void _catchException8();
__asm(".text");
__asm(".globl __catchException8");
__asm("__catchException8:");
SAVE_REGISTERS1();
SAVE_ERRCODE();
SAVE_REGISTERS2();
__asm ("pushl $8");
CALL_HOOK();

/* Same thing for exception 9.  */
static void _catchException9();
__asm(".text");
__asm(".globl __catchException9");
__asm("__catchException9:");
SAVE_REGISTERS1();
SAVE_REGISTERS2();
__asm ("pushl $9");
CALL_HOOK();

/* Same thing for exception 10.  */
static void _catchException10();
__asm(".text");
__asm(".globl __catchException10");
__asm("__catchException10:");
SAVE_REGISTERS1();
SAVE_ERRCODE();
SAVE_REGISTERS2();
__asm ("pushl $10");
CALL_HOOK();

/* Same thing for exception 12.  */
static void _catchException12();
__asm(".text");
__asm(".globl __catchException12");
__asm("__catchException12:");
SAVE_REGISTERS1();
SAVE_ERRCODE();
SAVE_REGISTERS2();
__asm ("pushl $12");
CALL_HOOK();

/* Same thing for exception 16.  */
static void _catchException16();
__asm(".text");
__asm(".globl __catchException16");
__asm("__catchException16:");
SAVE_REGISTERS1();
SAVE_REGISTERS2();
__asm ("pushl $16");
CALL_HOOK();

/* For 13, 11, and 14 we have to deal with the CHECK_FAULT stuff.  */

/* Same thing for exception 13.  */
static void _catchException13 ();
__asm (".text");
__asm (".globl __catchException13");
__asm ("__catchException13:");
CHECK_FAULT();
SAVE_REGISTERS1();
SAVE_ERRCODE();
SAVE_REGISTERS2();
__asm ("pushl $13");
CALL_HOOK();

/* Same thing for exception 11.  */
static void _catchException11 ();
__asm (".text");
__asm (".globl __catchException11");
__asm ("__catchException11:");
CHECK_FAULT();
SAVE_REGISTERS1();
SAVE_ERRCODE();
SAVE_REGISTERS2();
asm ("pushl $11");
CALL_HOOK();

/* Same thing for exception 14.  */
static void _catchException14 ();
__asm (".text");
__asm (".globl __catchException14");
__asm ("__catchException14:");
CHECK_FAULT();
SAVE_REGISTERS1();
SAVE_ERRCODE();
SAVE_REGISTERS2();
__asm ("pushl $14");
CALL_HOOK();
#endif /* 0 */
/*
 * remcomHandler is a front end for handle_exception.  It moves the
 * stack pointer into an area reserved for debugger use.
 */
__asm ("_remcomHandler:");
__asm ("           popl %eax");        /* pop off return address     */
__asm ("           popl %eax");      /* get the exception number   */
__asm ("		movl stackPtr, %esp"); /* move to remcom stack area  */
__asm ("		pushl %eax");	/* push exception onto stack  */
__asm ("		call  handle_exception");    /* this never returns */

static void
_returnFromException ()
{
  return_to_prog ();
}

static int
hex (ch)
     unsigned char ch;
{
  if ((ch >= 'a') && (ch <= 'f'))
    return (ch - 'a' + 10);
  if ((ch >= '0') && (ch <= '9'))
    return (ch - '0');
  if ((ch >= 'A') && (ch <= 'F'))
    return (ch - 'A' + 10);
  return (-1);
}

static unsigned char remcomInBuffer[BUFMAX];
static unsigned char remcomOutBuffer[BUFMAX];

/* scan for the sequence $<data>#<checksum>     */

static unsigned char *
getpacket (void)
{
  unsigned char *buffer = &remcomInBuffer[0];
  unsigned char checksum;
  unsigned char xmitcsum;
  int count;
  char ch;

  while (1)
    {
      /* wait around for the start character, ignore all other characters */
      while ((ch = getDebugChar ()) != '$')
	;

    retry:
      checksum = 0;
      xmitcsum = -1;
      count = 0;

      /* now, read until a # or end of buffer is found */
      while (count < BUFMAX)
	{
	  ch = getDebugChar ();
	  if (ch == '$')
	    goto retry;
	  if (ch == '#')
	    break;
	  checksum = checksum + ch;
	  buffer[count] = ch;
	  count = count + 1;
	}
      buffer[count] = 0;

      if (ch == '#')
	{
	  ch = getDebugChar ();
	  xmitcsum = hex (ch) << 4;
	  ch = getDebugChar ();
	  xmitcsum += hex (ch);

	  if (checksum != xmitcsum)
	    {
	      if (remote_debug)
		{
		  /*fprintf (stderr,
			   "bad checksum.  My count = 0x%x, sent=0x%x. buf=%s\n",
			   checksum, xmitcsum, buffer);*/
		}
	      putDebugChar ('-');	/* failed checksum */
	    }
	  else
	    {
	      putDebugChar ('+');	/* successful transfer */

	      /* if a sequence char is present, reply the sequence ID */
	      if (buffer[2] == ':')
		{
		  putDebugChar (buffer[0]);
		  putDebugChar (buffer[1]);

		  return &buffer[3];
		}

	      return &buffer[0];
	    }
	}
    }
}

/* send the packet in buffer.  */

static void
putpacket (unsigned char *buffer)
{
  unsigned char checksum;
  int count;
  char ch;

  /*  $<packet info>#<checksum>. */
  do
    {
      putDebugChar ('$');
      checksum = 0;
      count = 0;

      while ((ch = buffer[count]) != 0)
	{
	  putDebugChar (ch);
	  checksum += ch;
	  count += 1;
	}

      putDebugChar ('#');
      putDebugChar (hexchars[checksum >> 4]);
      putDebugChar (hexchars[checksum % 16]);
    }
  while (getDebugChar () != '+');
}

static void
debug_error (format, parm)
     char *format;
     char *parm;
{
  /*if (remote_debug)
    fprintf (stderr, format, parm);*/
}

/* Address of a routine to RTE to if we get a memory fault.  */
static void (*volatile mem_fault_routine) () = NULL;

/* Indicate to caller of mem2hex or hex2mem that there has been an
   error.  */
static volatile int mem_err = 0;

static void
set_mem_err (void)
{
  mem_err = 1;
}

/* These are separate functions so that they are so short and sweet
   that the compiler won't save any registers (if there is a fault
   to mem_fault, they won't get restored, so there better not be any
   saved).  */
static int
get_char(unsigned char *addr)
{
  return *addr;
}

static void
set_char(unsigned char *addr, int val)
{
  *addr = val;
}

/* convert the memory pointed to by mem into hex, placing result in buf */
/* return a pointer to the last char put in buf (null) */
/* If MAY_FAULT is non-zero, then we should set mem_err in response to
   a fault; if zero treat a fault like any other fault in the stub.  */
static unsigned char *
mem2hex (mem, buf, count, may_fault)
     unsigned char *mem;
     unsigned char *buf;
     int count;
     int may_fault;
{
  int i;
  unsigned char ch;

  if (may_fault)
    mem_fault_routine = set_mem_err;
  for (i = 0; i < count; i++)
    {
      ch = get_char (mem++);
      if (may_fault && mem_err)
	return (buf);
      *buf++ = hexchars[ch >> 4];
      *buf++ = hexchars[ch % 16];
    }
  *buf = 0;
  if (may_fault)
    mem_fault_routine =  NULL;
  return (buf);
}

/* convert the hex array pointed to by buf into binary to be placed in mem */
/* return a pointer to the character AFTER the last byte written */
static unsigned char *
hex2mem (buf, mem, count, may_fault)
     unsigned char *buf;
     unsigned char *mem;
     int count;
     int may_fault;
{
  int i;
  unsigned char ch;

  if (may_fault)
    mem_fault_routine = set_mem_err;
  for (i = 0; i < count; i++)
    {
      ch = hex (*buf++) << 4;
      ch = ch + hex (*buf++);
      set_char (mem++, ch);
      if (may_fault && mem_err)
	return (mem);
    }
  if (may_fault)
    mem_fault_routine = NULL;
  return (mem);
}

/* this function takes the 386 exception vector and attempts to
   translate this number into a unix compatible signal value */
static int
computeSignal (int exceptionVector)
{
  int sigval;
  switch (exceptionVector)
    {
    case 0:
      sigval = 8;
      break;			/* divide by zero */
    case 1:
      sigval = 5;
      break;			/* debug exception */
    case 3:
      sigval = 5;
      break;			/* breakpoint */
    case 4:
      sigval = 16;
      break;			/* into instruction (overflow) */
    case 5:
      sigval = 16;
      break;			/* bound instruction */
    case 6:
      sigval = 4;
      break;			/* Invalid opcode */
    case 7:
      sigval = 8;
      break;			/* coprocessor not available */
    case 8:
      sigval = 7;
      break;			/* double fault */
    case 9:
      sigval = 11;
      break;			/* coprocessor segment overrun */
    case 10:
      sigval = 11;
      break;			/* Invalid TSS */
    case 11:
      sigval = 11;
      break;			/* Segment not present */
    case 12:
      sigval = 11;
      break;			/* stack exception */
    case 13:
      sigval = 11;
      break;			/* general protection */
    case 14:
      sigval = 11;
      break;			/* page fault */
    case 16:
      sigval = 7;
      break;			/* coprocessor error */
    default:
      sigval = 7;		/* "software generated" */
    }
  return (sigval);
}

/**********************************************/
/* WHILE WE FIND NICE HEX CHARS, BUILD AN INT */
/* RETURN NUMBER OF CHARS PROCESSED           */
/**********************************************/
static int
hexToInt (unsigned char **ptr, int *intValue)
{
  int numChars = 0;
  int hexValue;

  *intValue = 0;

  while (**ptr)
    {
      hexValue = hex (**ptr);
      if (hexValue >= 0)
	{
	  *intValue = (*intValue << 4) | hexValue;
	  numChars++;
	}
      else
	break;

      (*ptr)++;
    }

  return (numChars);
}



/*
 * This function does all command procesing for interfacing to gdb.
 */
void
handle_exception (int exceptionVector)
{
  int sigval, stepping;
  int addr, length;
  unsigned char *ptr;
  int newPC;
 
  /* ShowEsp();*/
 
  gdb_i386vector = exceptionVector;

  if (remote_debug)
    {
      /*printf ("vector=%d, sr=0x%x, pc=0x%x\n",
	      exceptionVector, registers[PS], registers[PC]);*/
    }

  /* reply to host that an exception has occurred */
  sigval = computeSignal (exceptionVector);

  ptr = remcomOutBuffer;

  *ptr++ = 'T';			/* notify gdb with signo, PC, FP and SP */
  *ptr++ = hexchars[sigval >> 4];
  *ptr++ = hexchars[sigval & 0xf];

  *ptr++ = hexchars[ESP]; 
  *ptr++ = ':';
  ptr = mem2hex((unsigned char *)&registers[ESP], ptr, 4, 0);	/* SP */
  *ptr++ = ';';

  *ptr++ = hexchars[EBP]; 
  *ptr++ = ':';
  ptr = mem2hex((unsigned char *)&registers[EBP], ptr, 4, 0); 	/* FP */
  *ptr++ = ';';

  *ptr++ = hexchars[PC]; 
  *ptr++ = ':';
  ptr = mem2hex((unsigned char *)&registers[PC], ptr, 4, 0); 	/* PC */
  *ptr++ = ';';

  *ptr = '\0';

  putpacket (remcomOutBuffer);

  stepping = 0;

  while (1 == 1)
    {
      remcomOutBuffer[0] = 0;
      ptr = getpacket ();

      switch (*ptr++)
	{
	case '?':
	  remcomOutBuffer[0] = 'S';
	  remcomOutBuffer[1] = hexchars[sigval >> 4];
	  remcomOutBuffer[2] = hexchars[sigval % 16];
	  remcomOutBuffer[3] = 0;
	  break;
	case 'd':
	  remote_debug = !(remote_debug);	/* toggle debug flag */
	  break;
	case 'g':		/* return the value of the CPU registers */
	  mem2hex ((char *) registers, remcomOutBuffer, NUMREGBYTES, 0);
	  break;
	case 'G':		/* set the value of the CPU registers - return OK */
	  hex2mem (ptr, (char *) registers, NUMREGBYTES, 0);
          strcpy (remcomOutBuffer, (unsigned char*)"OK");          
	  break;
	case 'P':		/* set the value of a single CPU register - return OK */
	  {
	    int regno;

	    if (hexToInt (&ptr, &regno) && *ptr++ == '=')
	      if (regno >= 0 && regno < NUMREGS)
		{
		  hex2mem (ptr, (char *) &registers[regno], 4, 0);
		  strcpy (remcomOutBuffer, (unsigned char*)"OK");
		  break;
		}

	    strcpy (remcomOutBuffer, (unsigned char*)"E01");
	    break;
	  }

	  /* mAA..AA,LLLL  Read LLLL bytes at address AA..AA */
	case 'm':
	  /* TRY TO READ %x,%x.  IF SUCCEED, SET PTR = 0 */
	  if (hexToInt (&ptr, &addr))
	    if (*(ptr++) == ',')
	      if (hexToInt (&ptr, &length))
		{
		  ptr = 0;
		  mem_err = 0;
		  mem2hex ((char *) addr, remcomOutBuffer, length, 1);
		  if (mem_err)
		    {
		      strcpy (remcomOutBuffer, (unsigned char*)"E03");
		      debug_error ("memory fault");
		    }
		}

	  if (ptr)
	    {
	      strcpy (remcomOutBuffer, (unsigned char*)"E01");
	    }
	  break;

	  /* MAA..AA,LLLL: Write LLLL bytes at address AA.AA return OK */
	case 'M':
	  /* TRY TO READ '%x,%x:'.  IF SUCCEED, SET PTR = 0 */
	  if (hexToInt (&ptr, &addr))
	    if (*(ptr++) == ',')
	      if (hexToInt (&ptr, &length))
		if (*(ptr++) == ':')
		  {
		    mem_err = 0;
		    hex2mem (ptr, (char *) addr, length, 1);

		    if (mem_err)
		      {
			strcpy (remcomOutBuffer, (unsigned char*)"E03");
			debug_error ("memory fault");
		      }
		    else
		      {
			strcpy (remcomOutBuffer, (unsigned char*)"OK");
		      }

		    ptr = 0;
		  }
	  if (ptr)
	    {
	      strcpy (remcomOutBuffer, (unsigned char*)"E02");
	    }
	  break;

	  /* cAA..AA    Continue at address AA..AA(optional) */
	  /* sAA..AA   Step one instruction from AA..AA(optional) */
	case 's':
	  stepping = 1;
	case 'c':
	  /* try to read optional parameter, pc unchanged if no parm */
	  if (hexToInt (&ptr, &addr))
             registers[PC] = addr;

	  newPC = registers[PC];

	  /* clear the trace bit */
          registers[PS] &= 0xfffffeff;

	  /* set the trace bit if we're stepping */
	  if (stepping)
             registers[PS] |= 0x100;

	  _returnFromException ();	/* this is a jump */
	  break;

	  /* kill the program */
	case 'k':		/* do nothing */
	/*HLT; */
#if 0
	  /* Huh? This doesn't look like "nothing".
	     m68k-stub.c and sparc-stub.c don't have it.  */
	  BREAKPOINT ();
#endif
	  break;

    case 'Z':
        /* if this write watch break. */
     if(*ptr != '0')
     {
         unsigned long nType;

	 nType = (unsigned long)(*ptr - 0x30) ;
	 if(nType == 3)nType = BKT_RW;

	 ptr++;
	 
	 if (*(ptr++) == ',')
	 {
	     if(hexToInt(&ptr, &addr))
	     {
	     	if(*(ptr++) == ',')
		{
		    if (hexToInt (&ptr, &length))
		    {
			if(!SetHardwarePoint(addr, nType, length))
			   ptr = 0;
		    }
		}
	     }
	 }
	 if(ptr == 0)
	     strcpy (remcomOutBuffer, (unsigned char*)"OK");
	 else
	 {
		/* YIRtlKernelMsg("\nError\n");*/
	     strcpy (remcomOutBuffer, (unsigned char*)"E01");
	 }
    }
    break;

    case 'z':
     if(*ptr != '0')
     {
	 ptr++;
	 
	 if (*(ptr++) == ',')
	 {
	     if(hexToInt(&ptr, &addr))
	     {
	     	if(*(ptr++) == ',')
		{
		    if (hexToInt (&ptr, &length))
		    {
			if(!ClearHardwarePoint(addr))
			   ptr = 0;
		    }
		}
	     }
	 }
	 if(ptr == 0)
	     strcpy (remcomOutBuffer, (unsigned char*)"OK");
	 else
	     strcpy (remcomOutBuffer, (unsigned char*)"E01");
    }
    strcpy (remcomOutBuffer, (unsigned char*)"OK");
    break;

    case 'r':
        /* __asm__("cli; hlt");*/
	YIHLT();
      break;

	}			/* switch */

      /* reply to the request */
      putpacket (remcomOutBuffer);
    }
}

/* this function is used to set up exception handlers for tracing and
   breakpoints */
static void
set_debug_traps (void)
{
  stackPtr = &remcomStack[STACKSIZE / sizeof (int) - 4];

/*  exceptionHandler (0, _catchException0);*/
  exceptionHandler (1, _catchException1);
  exceptionHandler (3, _catchException3);
/*  exceptionHandler (4, _catchException4);
  exceptionHandler (5, _catchException5);
  exceptionHandler (6, _catchException6);
  exceptionHandler (7, _catchException7);
  exceptionHandler (8, _catchException8);
  exceptionHandler (9, _catchException9);
  exceptionHandler (10, _catchException10);
  exceptionHandler (11, _catchException11);
  exceptionHandler (12, _catchException12);
  exceptionHandler (13, _catchException13);
  exceptionHandler (14, _catchException14);
  exceptionHandler (16, _catchException16);*/

  putDebugChar('+');

  initialized = 1;
}

/* This function will generate a breakpoint exception.  It is used at the
   beginning of a program to sync up with a debugger and can be used
   otherwise as a quick means to stop program execution and "break" into
   the debugger. */

static void
breakpoint (void)
{
  if (initialized)
    YIDebugPoint ();
}
