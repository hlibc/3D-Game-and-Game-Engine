#include <ccube.h>
#include <ctools.h>
#include <cheaders.h>


void box(block b, float z1, float z2, float z3, float z4)
{
        glBegin(GL_POLYGON);
        glVertex3f((float)b.x,    z1, (float)b.y);
        glVertex3f((float)b.x+b.xs, z2, (float)b.y);
        glVertex3f((float)b.x+b.xs, z3, (float)b.y+b.ys);
        glVertex3f((float)b.x,    z4, (float)b.y+b.ys);
        glEnd();
        xtraverts += 4;
}

