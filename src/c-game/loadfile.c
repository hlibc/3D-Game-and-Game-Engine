#include <ccube.h>
#include <ctools.h>
#include <cheaders.h>



char *loadfile(char *fn, int *size)
{
        FILE *f = fopen(fn, "rb");
        if(!f) return NULL;
        fseek(f, 0, SEEK_END);
        size_t len = ftell(f);
        fseek(f, 0, SEEK_SET);
        char *buf = (char *)malloc(len+1);
        if(!buf) return NULL;
        buf[len] = 0;
        size_t rlen = fread(buf, 1, len, f);
        fclose(f);
        if(len!=rlen || len<=0)
        {
                free(buf);
                return NULL;
        };
        if(size!=NULL) *size = len;
        return buf;
}

