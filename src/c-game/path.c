#include <ccube.h>
#include <ctools.h>
#include <cheaders.h>


char *path(char *s)
{
        for(char *t = s; (t = strpbrk(t, "/\\")); *t++ = '/');
        return s;
}

