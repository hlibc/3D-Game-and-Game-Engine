//////////////////////////// pool ///////////////////////////

#include "tools.h"
#include <new>

pool::pool()
{
	blocks = 0;
	allocnext(POOLSIZE);
	for(int i = 0; i<MAXBUCKETS; i++) reuse[i] = NULL;
};

void *pool::alloc(size_t size)
{
	if(size>MAXREUSESIZE)
	{
		return malloc(size);
	}
	else
	{
		size = bucket(size);
		void **r = (void **)reuse[size];
		if(r)
		{
			reuse[size] = *r;
			return (void *)r;
		}
		else
		{
			size <<= PTRBITS;
			if(left<size) allocnext(POOLSIZE);
			char *r = p;
			p += size;
			left -= size;
			return r;
		};
	};
};

void pool::dealloc(void *p, size_t size)
{
	if(size>MAXREUSESIZE)
	{
		free(p);
	}
	else
	{
		size = bucket(size);
		if(size)	// only needed for 0-size free, are there any?
		{
			*((void **)p) = reuse[size];
			reuse[size] = p;
		};
	};
};

void *pool::realloc(void *p, size_t oldsize, size_t newsize)
{
	void *np = alloc(newsize);
	if(!oldsize) return np;
	memcpy(np, p, newsize>oldsize ? oldsize : newsize);
	dealloc(p, oldsize);
	return np;
};

void pool::dealloc_block(void *b)
{
	if(b)
	{
		dealloc_block(*((char **)b));
		free(b);
	};
}

void pool::allocnext(size_t allocsize)
{
	char *b = (char *)malloc(allocsize+PTRSIZE);
	*((char **)b) = blocks;
	blocks = b;
	p = b+PTRSIZE;
	left = allocsize;
};

char *pool::string(char *s, size_t l)
{
	char *b = (char *)alloc(l+1);
	strncpy(b,s,l);
	b[l] = 0;
	return b;  
};

pool *gp()  // useful for global buffers that need to be initialisation order independant
{
	static pool *p = NULL;
	return p ? p : (p = new pool());
};

