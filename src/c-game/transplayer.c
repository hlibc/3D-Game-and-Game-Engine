#include <ccube.h>
#include <ctools.h>
#include <cheaders.h>


void transplayer()
{
        glLoadIdentity();

        glRotated(player1->roll,0.0,0.0,1.0);
        glRotated(player1->pitch,-1.0,0.0,0.0);
        glRotated(player1->yaw,0.0,1.0,0.0);

        glTranslated(-player1->o.x, (player1->state==CS_DEAD ? player1->eyeheight-0.2f : 0)-player1->o.z, -player1->o.y);
};


