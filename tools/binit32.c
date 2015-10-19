
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
 * Filename:	$RCSfile: binit32.c,v $
 * Createdate:	2004/05/07 10:37:00
 * FileId:	$Id: binit32.c,v 1.1.1.1 2005/08/09 05:25:23 devin Exp $
 * Last update:	$Date: 2005/08/09 05:25:23 $
 * Creator:	Devin Wang <vhd@21cn.com>
 * Version:	$Revision: 1.1.1.1 $
 * Description: build tools for osloader.sys. 
 *
 * ====================================================================
 */

/* ====================================================================
 *  Revision:
 *
 * ====================================================================
 */


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <limits.h>

#include <yios/config.h>
#include <yios/hal/boot.h>

#define HEADER_OFFSET	0x10
#define ALIGN_SIZE	0x0010

/* bootsect load oskernel.sys into 0x1000 */

#define ALIGNSIZE(size)	do{(size) = ((((size)-1)>>4)+1)<<4;}while(0)


int main(int argc, char* argv[])
{
   int fd;
   int text_start_addr/*, data_start_addr, init32_size*/;
   struct stat st;
   char buf[PATH_MAX*4];
   
   if(argc != 5)
     {
       printf("binit32 $(LD) $(srcdir)/init32.lds init32.o decom.o");
       exit(1);
     }

   if((fd = open("init16", O_RDWR)) == -1)
   {
        perror("open init16 fault:");
	exit(1);
   }
   fstat(fd, &st);
   close(fd);
   
   text_start_addr = st.st_size;
   ALIGNSIZE(text_start_addr);
   text_start_addr += CFG_KERNEL_OSKERNELSYS_PADDR;
   
	   
   /*   sprintf(buf, "%s -T %s/init32.lds -Ttext 0x%x -o init32 init32.o decom.o\n",
	getenv("LD"), getenv("srcdir"), text_start_addr);*/
   sprintf(buf, "%s -T %s -Ttext 0x%x -o init32 %s %s\n",
	   argv[1], argv[2], text_start_addr, argv[3], argv[4]);
   printf(buf);
   return system(buf);      
   
   return 0;
}



