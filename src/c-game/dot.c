#include <ccube.h>
#include <ctools.h>
#include <cheaders.h>


void dot(int x, int y, float z)
{
        const float DOF = 0.1f;
        glBegin(GL_POLYGON);
        glVertex3f(x-DOF, (float)z, y-DOF);
        glVertex3f(x+DOF, (float)z, y-DOF);
        glVertex3f(x+DOF, (float)z, y+DOF);
        glVertex3f(x-DOF, (float)z, y+DOF);
        glEnd();
        xtraverts += 4;
}
