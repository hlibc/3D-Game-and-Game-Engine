#include <ccube.h>
#include <ctools.h>
#include <cheaders.h>

extern GLUquadricObj *qsphere;
void cleangl()
{
        if(qsphere) gluDeleteQuadric(qsphere);
}
