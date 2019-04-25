#include <ccube.h>
#include <ctools.h>
#include <cheaders.h>


void drawicon(float tx, float ty, int x, int y)
{
        glBindTexture(GL_TEXTURE_2D, 5);
        glBegin(GL_QUADS);
        tx /= 192;
        ty /= 192;
        float o = 1/3.0f;
        int s = 120;
        glTexCoord2f(tx,   ty);   glVertex2i(x,   y);
        glTexCoord2f(tx+o, ty);   glVertex2i(x+s, y);
        glTexCoord2f(tx+o, ty+o); glVertex2i(x+s, y+s);
        glTexCoord2f(tx,   ty+o); glVertex2i(x,   y+s);
        glEnd();
        xtraverts += 4;
}
