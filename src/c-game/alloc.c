#include <ccube.h>
#include <ctools.h>
#include <cheaders.h>


void *alloc(int s)                        // for some big chunks... most other allocs use the memory pool
{
        void *b = calloc(1,s);
        if(!b)
		write(2, "Out of memory\n", 15);
                //fatal("out of memory!");
        return b;
}


