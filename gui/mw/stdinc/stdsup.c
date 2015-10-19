
#include <yios/config.h>
#include <yios/types.h>

#include <yios/list.h>
#include <yios/hal/init.h>
#include <yios/dbg.h>

#include <yios/sched.h>
#include <yios/time.h>

#include <mwcfg.h>

#include <assert.h>

#include <ctype.h>
#include <fcntl.h>
#include <mwcfg.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>

int tolower(int c)
{
    if(c >= 'A' && c <= 'Z')
        c += 0x20;

    return c;
}

int toupper(int c)
{
    if(c >= 'a' && c <= 'z')
        c -= 0x20;

    return c;
}


/****************************************************************/
/* string function support */
/****************************************************************/
char *strncpy(char *s1, const char *s2, size_t n)
{
  register char c;
  char *s = s1;

  --s1;

  if (n >= 4)
    {
      size_t n4 = n >> 2;

      for (;;)
	{
	  c = *s2++;
	  *++s1 = c;
	  if (c == '\0')
	    break;
	  c = *s2++;
	  *++s1 = c;
	  if (c == '\0')
	    break;
	  c = *s2++;
	  *++s1 = c;
	  if (c == '\0')
	    break;
	  c = *s2++;
	  *++s1 = c;
	  if (c == '\0')
	    break;
	  if (--n4 == 0)
	    goto last_chars;
	}
      n = n - (s1 - s) - 1;
      if (n == 0)
	return s;
      goto zero_fill;
    }

 last_chars:
  n &= 3;
  if (n == 0)
    return s;

  do
    {
      c = *s2++;
      *++s1 = c;
      if (--n == 0)
	return s;
    }
  while (c != '\0');

 zero_fill:
  do
    *++s1 = '\0';
  while (--n > 0);

  return s;

}

char *strdup(const char *s)
{
    char* p;
    size_t n;

    n = strlen(s);
    
    if(!n)
        return NULL;
 
    p =  malloc(n);

    if(!p)
        return NULL;

    memcpy(p, s, n);

    return p;
}

int strcasecmp(const char *s1,const char * s2)
{
    int result;
    const unsigned char *p1 = (const unsigned char *) s1;
    const unsigned char *p2 = (const unsigned char *) s2; 
    if (p1 == p2)
        return 0;
    while ((result = tolower(*p1) - tolower (*p2++)) == 0)
        if (*p1++ == '\0')
            break;
     return result;
}


void *memcpy(void *dest, const void *src, size_t n)
{
    return YIRtlMemoryCopy(dest, (void*) src, n);
}

void *memset(void *s, int c, size_t n)
{
    return YIRtlMemorySet(s, c, n);
}


long int random(void)
{
    return Rand();
}
/****************************************************************/
/* memory support */
/****************************************************************/
#define MEM_ALLOC_ALIGN_SIZE        8
#define MW_ALIGN(__x__)             ALIGN_UP((__x__), MEM_ALLOC_ALIGN_SIZE) 

typedef struct __MEM_ALLOC_MANAGER__{
    LIST_HEAD   list;
    PADDR       pMem;
    NRADDR      nSize;      /* Aligned all size (include manager self )*/
    SIZE_T      nOrgSize;   /* origin require size */
}MEM_ALLOC_MANAGER;


static MEM_ALLOC_MANAGER yi_g_ltMwAllocMan = 
    {LIST_HEAD_INIT(yi_g_ltMwAllocMan.list), NULL, 0, 0};
//static PADDR yi_g_pMwStackBeginPoint = YI_KE_MW_SM_VADDR;

void *malloc(size_t size)
{
    LIST_HEAD* pList;
    SIZE_T  nOrgSize = size;
    MEM_ALLOC_MANAGER* pManLast = &yi_g_ltMwAllocMan;

    if(!size)
        return NULL;

    size = MW_ALIGN(size);
    size += sizeof(MEM_ALLOC_MANAGER);
    size = MW_ALIGN(size);
    //PADDR = YI_KE_MW_SM_VADDR;

    LIST_FOR_EACH(pList, &yi_g_ltMwAllocMan.list)
    {
        MEM_ALLOC_MANAGER* pMan, *pManPrev;
        int nFirst = 0;

        pMan = LIST_ENTRY(pList, MEM_ALLOC_MANAGER, list);
        pManPrev = LIST_ENTRY(pList->pPrev, MEM_ALLOC_MANAGER, list);

        /* This't the first memory block. */
        if(pManPrev == &yi_g_ltMwAllocMan)
            nFirst = 1;

        //YIRtlKernelMsg("<%d>", aa);
        /* get a memory gap for allocate */
        if((NRADDR)pMan - (nFirst?
            YI_KE_MW_SM_VADDR:(NRADDR)((NRADDR)pManPrev + pManPrev->nSize)) >= size)
        {
            MEM_ALLOC_MANAGER* pManCur;

            /* Alloc new memory in gap. */
            pManCur = (PADDR)(nFirst? (NRADDR)YI_KE_MW_SM_VADDR: ((NRADDR)pManPrev + pManPrev->nSize));
            pManCur->nSize = size;
            pManCur->nOrgSize = nOrgSize;
            pManCur->pMem = (PADDR)MW_ALIGN((NRADDR)pManCur+sizeof(MEM_ALLOC_MANAGER));
            YIListAdd(&pManPrev->list, &pManCur->list);
            return pManCur->pMem;
        }
       pManLast = pMan;
    }
    { 
        MEM_ALLOC_MANAGER* pManCur;

        /* if empty */
        if(pManLast == &yi_g_ltMwAllocMan)
        {
            /* if size too big */
            if(YI_KE_MW_SM_SIZE < size)
                return NULL;
            /* Alloc new memory empty memory. */
            pManCur =  (PADDR) YI_KE_MW_SM_VADDR;
        }
        else /* no gap and no empty. */
        {
            /* if size too big */
            if((NRADDR)(YI_KE_MW_SM_VADDR+YI_KE_MW_SM_SIZE) - 
                    (NRADDR)((NRADDR)pManLast + pManLast->nSize) < size)
                return NULL;
            /* Alloc new memory behind last memory block. */
            pManCur = (PADDR)((NRADDR)pManLast + pManLast->nSize);
        }
        pManCur->nSize = size;
        pManCur->nOrgSize = nOrgSize;
        pManCur->pMem = (PADDR)MW_ALIGN((NRADDR)pManCur+sizeof(MEM_ALLOC_MANAGER));
        YIListAdd(&pManLast->list, &pManCur->list);
        return pManCur->pMem;
    }
}

void *realloc(void *ptr, size_t size)
{
    void* newptr;
    LIST_HEAD* pList;
    MEM_ALLOC_MANAGER* pOldMan = NULL, *pNewMan = NULL;

    if(!ptr)
        return malloc(size);
    if(!size)
    {
        free(ptr);
        return malloc(size);
    }
    
    /* A bad realloc arithmetic. */
    newptr = malloc(size);
    if(!newptr)
        return NULL;

    LIST_FOR_EACH(pList, &yi_g_ltMwAllocMan.list)
    {
        MEM_ALLOC_MANAGER* pMan;

        pMan = LIST_ENTRY(pList, MEM_ALLOC_MANAGER, list);
        
        if(pMan->pMem == ptr)
        {
            pOldMan = pMan;
            break;
        }
        else if(pMan->pMem == newptr)
        {
            pNewMan = pMan;
            break;
        }
    }
    if(pNewMan == NULL || pOldMan == NULL)
        BUG();
    
    memcpy(newptr, ptr, (pOldMan->nOrgSize> pNewMan->nOrgSize)?
                pNewMan->nOrgSize:pOldMan->nOrgSize );
    free(ptr);

    return newptr;
}

void free(void *ptr)
{
    LIST_HEAD*  pList;
    if(ptr == NULL)
        return;

    LIST_FOR_EACH(pList, &yi_g_ltMwAllocMan.list)
    {
        MEM_ALLOC_MANAGER* pMan;

        pMan = LIST_ENTRY(pList, MEM_ALLOC_MANAGER, list);
        
        if(pMan->pMem == ptr)
        {
            YIListDel(&pMan->list);
            return ;
        }
    }

    /* wrong ptr for free */
    BUG();
}

void *calloc(size_t nmemb, size_t size)
{
    void* ret;
    ret = malloc(nmemb*size);
    if(ret)
    {
        memset(ret, 0, nmemb*size);
    }
    return ret;
}

/****************************************************************/
/*  */
/****************************************************************/
/* This code is derived from a public domain shell sort routine by
 * Ray Gardner and found in Bob Stout's snippets collection.  The
 * original code is included below in an #if 0/#endif block.
 *
 * I modified it to avoid the possibility of overflow in the wgap
 * calculation, as well as to reduce the generated code size with
 * bcc and gcc. */

void qsort (void  *base,
            size_t nel,
            size_t width,
            int (*comp)(const void *, const void *))
{
	size_t wgap, i, j, k;
	char tmp;

	if ((nel > 1) && (width > 0)) {
		assert( nel <= ((size_t)(-1)) / width ); /* check for overflow */
		wgap = 0;
		do {
			wgap = 3 * wgap + 1;
		} while (wgap < (nel-1)/3);
		/* From the above, we know that either wgap == 1 < nel or */
		/* ((wgap-1)/3 < (int) ((nel-1)/3) <= (nel-1)/3 ==> wgap <  nel. */
		wgap *= width;			/* So this can not overflow if wnel doesn't. */
		nel *= width;			/* Convert nel to 'wnel' */
		do {
			i = wgap;
			do {
				j = i;
				do {
					register char *a;
					register char *b;

					j -= wgap;
					a = j + ((char *)base);
					b = a + wgap;
					if ( (*comp)(a, b) <= 0 ) {
						break;
					}
					k = width;
					do {
						tmp = *a;
						*a++ = *b;
						*b++ = tmp;
					} while ( --k );
				} while (j >= wgap);
				i += width;
			} while (i < nel);
			wgap = (wgap - width)/3;
		} while (wgap);
	}
}


/****************************************************************/
/* file functions  */
/****************************************************************/
void setbuf(FILE *stream, char *buf)
{

}

int close(int fd)
{
    return 0;
}

ssize_t write(int fd, const void *buf, size_t count)
{
    if(fd == 1 && fd == 2)
    {   //YI
     }
     BUG();
     return 0;
}

void exit(int status)
{
    return ;
}

sighandler_t signal(int signum, sighandler_t handler)
{
    return handler;
}

void usleep(unsigned long usec)
{
    //usec /= 1000;
    YIKeSRMWait(YIKeGetCurrentThreadId(),
        usec/(1000/CFG_KERNEL_SCHED_HZ), YI_WAIT_WITH_TIMEOUT_INTERRUPTED);
    return; 
}

int gettimeofday(struct timeval *tv, struct timezone *tz)
{
    tv->tv_sec = yi_g_nTimeTickCount/CFG_KERNEL_SCHED_HZ;
    tv->tv_usec = (yi_g_nTimeTickCount%CFG_KERNEL_SCHED_HZ)*(1000/CFG_KERNEL_SCHED_HZ);
    return 0;
}

int fprintf(FILE *stream, const char *format, ...)
{
    /* if(stream == stdout || stream == stdin)
        YIRtlMessage(format);*/
    return 0;
}
