#include <ccube.h>
#include <ctools.h>
#include <cheaders.h>

extern int intermission;
extern int sensitivity;
extern int sensitivityscale;
extern int invmouse;
void mousemove(int dx, int dy)
{
	//if(player1->state==CS_DEAD || intermission) return;
        const float SENSF = 33.0f;       // try match quake sens
        player1->yaw += (dx/SENSF)*(sensitivity/(float)sensitivityscale);
        player1->pitch -= (dy/SENSF)*(sensitivity/(float)sensitivityscale)*(invmouse ? -1 : 1);
        fixplayer1range();
};

