#include <ccube.h>
#include <ctools.h>
#include <cheaders.h>


void endianswap(void *memory, int stride, int length)   // little indians as storage format
{
        if(*((char *)&stride)) return;
        loop(w, length) loop(i, stride/2)
        {
                uchar *p = (uchar *)memory+w*stride;
                uchar t = p[i];
                p[i] = p[stride-i-1];
                p[stride-i-1] = t;
        };
}

