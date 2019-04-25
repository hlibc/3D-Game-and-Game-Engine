#include <ccube.h>
#include <ctools.h>
#include <cheaders.h>



uchar *stripheader(uchar *b)
{
        char *s = strstr((char *)b, "\n\r\n");
        if(!s) s = strstr((char *)b, "\n\n");
        return s ? (uchar *)s : b;
}
