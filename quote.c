#include "error.h"
#include "oflops.h"
#include "stralloc.h"
#include "str.h"
#include "quote.h"

/*
quote() encodes a box as per rfc 821 and rfc 822,
while trying to do as little quoting as possible.
no, 821 and 822 don't have the same encoding. they're not even close.
no special encoding here for bytes above 127.
*/

static char ok[128] = {
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
,0,7,0,7,7,7,7,7,0,0,7,7,0,7,7,7 ,7,7,7,7,7,7,7,7,7,7,0,0,0,7,0,7
,0,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7 ,7,7,7,7,7,7,7,7,7,7,7,0,0,0,7,7
,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7 ,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,0
} ;

static int doit(saout,sain)
stralloc *saout;
stralloc *sain;
{
 char ch;
 int i;
 int j;
 unsigned int nlen;

 /* make sure the size calculation below does not overflow */
 if (__builtin_mul_overflow(sain->len, 2, &nlen) ||
     __builtin_add_overflow(nlen, 2, &nlen)) {
   errno = error_nomem;
   return 0;
 }
 if (!stralloc_ready(saout,nlen)) return 0;
 j = 0;
 saout->s[j++] = '"';
 for (i = 0;i < sain->len;++i)
  {
   ch = sain->s[i];
   if ((ch == '\r') || (ch == '\n') || (ch == '"') || (ch == '\\'))
     saout->s[j++] = '\\';
   saout->s[j++] = ch;
  }
 saout->s[j++] = '"';
 saout->len = j;
 return 1;
}

int quote_need(s,n)
char *s;
unsigned int n;
{
 unsigned char uch;
 int i;
 if (!n) return 1;
 for (i = 0;i < n;++i)
  {
   uch = s[i];
   if (uch >= 128) return 1;
   if (!ok[uch]) return 1;
  }
 if (s[0] == '.') return 1;
 if (s[n - 1] == '.') return 1;
 for (i = 0;i < n - 1;++i) if (s[i] == '.') if (s[i + 1] == '.') return 1;
 return 0;
}

int quote(saout,sain)
stralloc *saout;
stralloc *sain;
{
 if (quote_need(sain->s,sain->len)) return doit(saout,sain);
 return stralloc_copy(saout,sain);
}

static stralloc foo = {0};

int quote2(sa,s)
stralloc *sa;
char *s;
{
 char *at;
 if (!*s) return stralloc_copys(sa,s);
    at = strrchr(s,'@');
 if (!stralloc_copys(&foo,s)) return 0;
 if (!at) return quote(sa,&foo);
 foo.len = at - s;
 if (!quote(sa,&foo)) return 0;
 return stralloc_cats(sa,at);
}
