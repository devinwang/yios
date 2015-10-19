
#include <yios/config.h>
#include <yios/types.h>

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

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

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
