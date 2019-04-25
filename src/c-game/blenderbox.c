#include <ccube.h>
#include <ctools.h>
#include <cheaders.h>


void blendbox(int x1, int y1, int x2, int y2, bool border)
{
        glDepthMask(GL_FALSE);
        glDisable(GL_TEXTURE_2D);
        glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
        glBegin(GL_QUADS);
        if(border) glColor3d(0.5, 0.3, 0.4);
        else glColor3d(1.0, 1.0, 1.0);
        glVertex2i(x1, y1);
        glVertex2i(x2, y1);
        glVertex2i(x2, y2);
        glVertex2i(x1, y2);
        glEnd();
        glDisable(GL_BLEND);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBegin(GL_POLYGON);
        glColor3d(0.2, 0.7, 0.4);
        glVertex2i(x1, y1);
        glVertex2i(x2, y1);
        glVertex2i(x2, y2);
        glVertex2i(x1, y2);
        glEnd();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        xtraverts += 8;
        glEnable(GL_BLEND);
        glEnable(GL_TEXTURE_2D);
        glDepthMask(GL_TRUE);
}
