#include <ccube.h>
#include <ctools.h>
#include <cheaders.h>


void line(int x1, int y1, float z1, int x2, int y2, float z2)
{
        glBegin(GL_POLYGON);
        glVertex3f((float)x1, z1, (float)y1);
        glVertex3f((float)x1, z1, y1+0.01f);
        glVertex3f((float)x2, z2, y2+0.01f);
        glVertex3f((float)x2, z2, (float)y2);
        glEnd();
        xtraverts += 4;
}

