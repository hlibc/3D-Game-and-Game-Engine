#include <ccube.h>
#include <ctools.h>
#include <cheaders.h>

extern vertex *verts;
extern int curvert;
extern int curmaxverts;

void setarraypointers();
void reallocv()
{
        verts = (vertex *)realloc(verts, (curmaxverts *= 2)*sizeof(vertex));
        curmaxverts -= 10;
        if(!verts) 
		;// need an error here
        setarraypointers();
}


