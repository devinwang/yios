
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
 * Filename:	$RCSfile: list.h,v $
 * Createdate:	2005/01/05 20:41:00
 * FileId:	$Id: list.h,v 1.1.1.1 2005/08/09 05:25:23 devin Exp $
 * Last update:	$Date: 2005/08/09 05:25:23 $
 * Creator:	Devin Wang <vhd@21cn.com>
 * Version:	$Revision: 1.1.1.1 $
 * Description: list-chain manipulation method come from linux. 
 *
 * ====================================================================
 */

/* ====================================================================
 *  Revision:
 *
 * ====================================================================
 */


#ifndef _YIOS_LIST_H_
#define _YIOS_LIST_H_

#include <yios/config.h>
#include <yios/types.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */


typedef struct LIST_HEAD{
	struct LIST_HEAD *pNext;
	struct LIST_HEAD *pPrev;
}LIST_HEAD, *PLIST_HEAD;

#define LIST_HEAD_INIT(__list)	{&(__list), &(__list)}

#define LIST_INIT(__plist)	do{ \
		(__plist)->pNext = (__plist);\
		(__plist)->prev = (__plist);}while(0);


#define LIST_ENTRY(__plist, __type, __member) \
	((__type*)((char*)(__plist)-(char*)(&((__type*)0)->__member))) 

#define LIST_FOR_EACH(__pHead, __pList) \
	for ((__pHead) = (__pList)->pNext ; (__pHead) != (__pList); \
	        	(__pHead) = (__pHead)->pNext)


STATIC INLINE VOID YIListInsert(PLIST_HEAD pPrev, PLIST_HEAD pNext, PLIST_HEAD pNew)
{
	pNext->pPrev = pNew;
	pNew->pNext = pNext;
	pNew->pPrev = pPrev;
	pPrev->pNext = pNew;
}

STATIC INLINE VOID YIListAdd(PLIST_HEAD pList, PLIST_HEAD pNew)
{
	YIListInsert(pList, pList->pNext, pNew);	
}

STATIC INLINE VOID YIListAddTail(PLIST_HEAD pList, PLIST_HEAD pNew)
{
	YIListInsert(pList->pPrev, pList, pNew);	
}

STATIC INLINE VOID YIListDelete(PLIST_HEAD pPrev, PLIST_HEAD pNext)
{
	pPrev->pNext = pNext;
	pNext->pPrev = pPrev;
}

STATIC INLINE VOID YIListDel(PLIST_HEAD pItem)
{
	YIListDelete(pItem->pPrev, pItem->pNext);
}

STATIC INLINE INT YIListEmpty(PLIST_HEAD pItem)
{
	return pItem->pNext == pItem;
}


#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _YIOS_LIST_H_ */



