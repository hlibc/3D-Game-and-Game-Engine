#include <ccube.h>
#include <ctools.h>
#include <cheaders.h>


void updatepos(dynent *d)
{
        const float r = player1->radius+d->radius;
        const float dx = player1->o.x-d->o.x;
        const float dy = player1->o.y-d->o.y;
        const float dz = player1->o.z-d->o.z;
        const float rz = player1->aboveeye+d->eyeheight;
        const float fx = (float)fabs(dx), fy = (float)fabs(dy), fz = (float)fabs(dz);
        if(fx<r && fy<r && fz<rz && d->state!=CS_DEAD)
        {
                if(fx<fy) d->o.y += dy<0 ? r-fy : -(r-fy);  // push aside
                else      d->o.x += dx<0 ? r-fx : -(r-fx);
        };
        int lagtime = lastmillis-d->lastupdate;
        if(lagtime)
        {
                d->plag = (d->plag*5+lagtime)/6;
                d->lastupdate = lastmillis;
        };
};
