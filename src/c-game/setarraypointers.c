#include <ccube.h>
#include <ctools.h>
#include <cheaders.h>

extern vertex *verts;
extern int curvert;
extern int curmaxverts;
void setarraypointers()
{
        glVertexPointer(3, GL_FLOAT, sizeof(vertex), &verts[0].x);
        glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(vertex), &verts[0].r);
        glTexCoordPointer(2, GL_FLOAT, sizeof(vertex), &verts[0].u);
}
