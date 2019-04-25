#include <ccube.h>
#include <ctools.h>
#include <cheaders.h>


int listlen(char *a)
{
        if(!*a) return 0;
        int n = 0;
        while(*a) if(*a++==' ') n++;
        return n+1;
}

