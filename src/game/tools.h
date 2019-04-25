// generic useful stuff for any C++ program

#ifndef _TOOLS_H
#define _TOOLS_H 
#include <new>

#include "../../include/c-game/ctools.h"

struct sprintf_s_f
{
	char *d;
	sprintf_s_f(char *str): d(str) {};
	void operator()(const char* fmt, ...)
	{
		va_list v;
		va_start(v, fmt);
		_vsnprintf(d, _MAXDEFSTR, fmt, v);
		va_end(v);
		d[_MAXDEFSTR-1] = 0;
	};
};


#define sprintf_s(d) sprintf_s_f((char *)d)
#define sprintf_sd(d) string d; sprintf_s(d)
#define sprintf_sdlv(d,last,fmt) string d; { va_list ap; va_start(ap, last); formatstring(d, fmt, ap); va_end(ap); }
#define sprintf_sdv(d,fmt) sprintf_sdlv(d,fmt,fmt)

#define fast_f2nat(val) ((int)(val))

// memory pool that uses buckets and linear allocation for small objects
// VERY fast, and reasonably good memory reuse 

struct pool
{
	enum { POOLSIZE = 4096 };   // can be absolutely anything
	enum { PTRSIZE = sizeof(char *) };
	enum { MAXBUCKETS = 65 };   // meaning up to size 256 on 32bit pointer systems
	enum { MAXREUSESIZE = MAXBUCKETS*PTRSIZE-PTRSIZE };
	inline size_t bucket(size_t s) { return (s+PTRSIZE-1)>>PTRBITS; };
	enum { PTRBITS = PTRSIZE==2 ? 1 : PTRSIZE==4 ? 2 : 3 };

	char *p;
	size_t left;
	char *blocks;
	void *reuse[MAXBUCKETS];

	pool();
	~pool() { dealloc_block(blocks); };
	

	void *alloc(size_t size);
	void dealloc(void *p, size_t size);
	void *realloc(void *p, size_t oldsize, size_t newsize);

	char *string(char *s, size_t l);
	char *string(char *s) { return string(s, strlen(s)); };
	void deallocstr(char *s) { dealloc(s, strlen(s)+1); }; 
	char *stringbuf(char *s) { return string(s, _MAXDEFSTR-1); }; 

	void dealloc_block(void *b);
	void allocnext(size_t allocsize);
	
};

pool *gp();
template <class T> struct vector
{
	T *buf;
	int alen;
	int ulen;
	pool *p;

	vector()
	{
		this->p = gp();
		alen = 8;
		buf = (T *)p->alloc(alen*sizeof(T));
		ulen = 0;
	};
	
	~vector() { setsize(0); p->dealloc(buf, alen*sizeof(T)); };
	
	vector(vector<T> &v);
	void operator=(vector<T> &v);

	T &add(const T &x)
	{
		if(ulen==alen) realloc();
		new (&buf[ulen]) T(x);
		return buf[ulen++];
	};

	T &add()
	{
		if(ulen==alen) realloc();
		new (&buf[ulen]) T;
		return buf[ulen++];
	};

	T &pop() { return buf[--ulen]; };
	T &last() { return buf[ulen-1]; };
	bool empty() { return ulen==0; };

	int length() { return ulen; };
	T &operator[](int i) { assert(i>=0 && i<ulen); return buf[i]; };
	void setsize(int i) { for(; ulen>i; ulen--) buf[ulen-1].~T(); };
	T *getbuf() { return buf; };
	
	void sort(void *cf) { qsort(buf, ulen, sizeof(T), (int (__cdecl *)(const void *,const void *))cf); };

	void realloc()
	{
		int olen = alen;
		buf = (T *)p->realloc(buf, olen*sizeof(T), (alen *= 2)*sizeof(T));
	};

	T remove(int i)
	{
		T e = buf[i];
		for(int p = i+1; p<ulen; p++) buf[p-1] = buf[p];
		ulen--;
		return e;
	};

	T &insert(int i, const T &e)
	{
		add(T());
		for(int p = ulen-1; p>i; p--) buf[p] = buf[p-1];
		buf[i] = e;
		return buf[i];
	};
};


template <class T> struct hashtable
{
	struct chain { chain *next; char *key; T data; };

	int size;
	int numelems;
	chain **table;
	pool *parent;
	chain *enumc;

	hashtable()
	{
		this->size = 1<<10;
		this->parent = gp();
		numelems = 0;
		table = (chain **)parent->alloc(size*sizeof(T));
		for(int i = 0; i<size; i++) table[i] = NULL;
	};

	hashtable(hashtable<T> &v);
	void operator=(hashtable<T> &v);

	T *access(const char *key, T *data = NULL)
	{
		unsigned int h = 5381;
		for(int i = 0, k; (k = key[i]); i++) h = ((h<<5)+h)^k;	// bernstein k=33 xor
		h = h&(size-1);										 // primes not much of an advantage
		for(chain *c = table[h]; c; c = c->next)
		{
			char ch;
			for(const char *p1 = key, *p2 = c->key; (ch = *p1++)==*p2++; ) if(!ch)	//if(strcmp(key,c->key)==0)
			{
				T *d = &c->data;
				if(data) c->data = *data;
				return d;
			};
		};
		if(data)
		{
			chain *c = (chain *)parent->alloc(sizeof(chain));
			c->data = *data;
			c->key = (char*)key;
			c->next = table[h];
			table[h] = c;
			numelems++;
		};
		return NULL;
	};
};

inline char *newstring(char *s)		{ return gp()->string(s);	};
inline char *newstring(char *s, size_t l) { return gp()->string(s, l); };
inline char *newstringbuf(char *s)	 { return gp()->stringbuf(s); };

#endif

