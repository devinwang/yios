
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
 
 * ====================================================================
 *
 * Filename:	$RCSfile: bkernel.c,v $
 * Createdate:	2004/05/07 11:36:00
 * FileId:	$Id: bkernel.c,v 1.1.1.1 2005/08/09 05:25:23 devin Exp $
 * Last update:	$Date: 2005/08/09 05:25:23 $
 * Creator:	Devin Wang <vhd@21cn.com>
 * Version:	$Revision: 1.1.1.1 $
 * Description: build tools for oskernel.sys. 
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
#include <string.h>

#define HEADER_OFFSET	0x10
#define ALIGN_SIZE	0x0010
#define LOADER_OFFSET	0x1000

#define ALIGNSIZE(size)	do{(size) = ((((size)-1)>>4)+1)<<4;}while(0)

typedef struct _INIT16_INFORMATION_HEADER{
   unsigned int init32_start_addr;
}INIT16_INFORMATION_HEADER;

typedef struct _INIT32_INFORMATION_HEADER{
   unsigned int kernel32_position;
   unsigned int kernel32_size;
   unsigned int kernel32_pre_size;
   unsigned int kernel32_de_size;
}INIT32_INFORMATION_HEADER;




int main(int argc, char** argv)
{
   int fd[4];
   int size[4], total_size;
   struct stat st;
   void* buf;
   INIT16_INFORMATION_HEADER init16_header;
   INIT32_INFORMATION_HEADER init32_header;
   
   if(argc != 4)
   {
      printf("bkernel init16 init32 oskernel.gz\n");
      return 1;
   }
   
   if((fd[0] = open(argv[1], O_RDWR)) == -1)
   {
      perror("open argv[1] fault:");
      return 1;
   }
   fstat(fd[0], &st);
   size[0] = st.st_size;

   ALIGNSIZE(size[0]);
   total_size = size[0];
   

   if((fd[1] = open(argv[2], O_RDWR)) == -1)
   {
      perror("open argv[2] fault:");
      return 1;
   }
   fstat(fd[1], &st);
   size[1] = st.st_size;
   ALIGNSIZE(size[1]);
   total_size += size[1];



   if((fd[2] = open(argv[3], O_RDWR)) == -1)
   {
      perror("open argv[3] fault:");
      return 1;
   }
   fstat(fd[2], &st);
   size[2] = st.st_size;
/*   ALIGNSIZE(size[2]);*/
   total_size += size[2];

   if((buf = malloc(total_size))==NULL)
   {
      perror("malloc memory fault:");
      return 1;
   }
   
   if((read(fd[0], buf, size[0])) == -1)
   {
      perror("read fd[0] fault:");
      return 1;
   }
   
   init16_header.init32_start_addr = size[0]+LOADER_OFFSET;
   *(INIT16_INFORMATION_HEADER*)(buf + HEADER_OFFSET) = init16_header;
   

   if((read(fd[1], buf+size[0], size[1])) == -1)
   {
      perror("read fd[1] fault:");
      return 1;
   }

   bzero(&init32_header, sizeof(init32_header));
   init32_header.kernel32_position = size[0]+size[1]+LOADER_OFFSET;
   init32_header.kernel32_size = size[2];
/*   init32_header.loader_pre_size;
   init32_header.loader_de_size;
*/
   
   *(INIT32_INFORMATION_HEADER*)(buf + size[0] + HEADER_OFFSET) = init32_header;

   if((read(fd[2], buf+size[0]+size[1], size[2])) == -1)
   {
      perror("read fd[2] fault:");
      return 1;
   }


   if((fd[3] = open("oskernel.sys", O_CREAT|O_RDWR, S_IRWXU)) == -1)
   {
      perror("open oskernel.sys fault:");
      return 1;
   }

   if(write(fd[3], buf, total_size) == -1)
   {
      perror("write osloader.sys fualt:");
      return 1;
   }
   
   close(fd[0]);
   close(fd[1]);
   close(fd[2]);
   close(fd[3]);
   return 0;
}



