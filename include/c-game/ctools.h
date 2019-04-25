/* things that will sort of work with C .. */
#define true 1
#define false 0

#ifndef _CTOOLS_H
#define _CTOOLS_H 

#ifdef __GNUC__
#define gamma __gamma
#endif 
#include <math.h> 
#ifdef __GNUC__
#undef gamma
#endif 

/* c headers */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include <assert.h> 

/* get rid of these data type */
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

/* macros */
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define rnd(max) (rand()%(max))
#define rndreset() (srand(1))
#define rndtime() { loopi(lastmillis&0xF) rnd(i+1); }
#define loop(v,m) for(int v = 0; v<(m); v++)
#define loopi(m) loop(i,m)
#define loopj(m) loop(j,m)
#define loopk(m) loop(k,m)
#define loopl(m) loop(l,m)
#define __cdecl
#define _vsnprintf vsnprintf

#define loopv(v) for(int i = 0; i<(v).length(); i++)
#define loopvrev(v) for(int i = (v).length()-1; i>=0; i--)
#define enumerate(ht,t,e,b) loopi(ht->size) for(ht->enumc = ht->table[i]; ht->enumc; ht->enumc = ht->enumc->next) { t e = &ht->enumc->data; b; }
// easy safe strings

#define _MAXDEFSTR 260
typedef char string[_MAXDEFSTR]; 

inline void strn0cpy(char *d, const char *s, size_t m) { strncpy(d,s,m); d[(m)-1] = 0; };
inline void strcpy_s(char *d, const char *s) { strn0cpy(d,s,_MAXDEFSTR); };
inline void strcat_s(char *d, const char *s) { size_t n = strlen(d); strn0cpy(d+n,s,_MAXDEFSTR-n); };


inline void formatstring(char *d, const char *fmt, va_list v)
{
	_vsnprintf(d, _MAXDEFSTR, fmt, v);
	d[_MAXDEFSTR-1] = 0;
};
#define fast_f2nat(val) ((int)(val))


#endif
