#include <ccube.h>
#include <ctools.h>
#include <cheaders.h>

void conoutf(const char *s, ...)
{
        char sf[_MAXDEFSTR];
        va_list ap;
        va_start(ap, s);
        vprintf(s, ap);
        sf[_MAXDEFSTR-1] = 0;
        va_end(ap);
        printf("\n");
        fflush(stdout);
}
