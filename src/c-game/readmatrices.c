#include <ccube.h>
#include <ctools.h>
#include <cheaders.h>


extern GLint viewport[4];
extern GLdouble mm[16], pm[16];


void readmatrices()
{
        glGetIntegerv(GL_VIEWPORT, viewport);
        glGetDoublev(GL_MODELVIEW_MATRIX, mm);
        glGetDoublev(GL_PROJECTION_MATRIX, pm);
}


