// worldocull.cpp: occlusion map and occlusion test

#include "cube.h"

#define NUMRAYS 512

float rdist[NUMRAYS];
bool ocull = true;
float odist = 256;

void toggleocull() { ocull = !ocull; };

COMMAND(toggleocull, ARG_NONE);

// constructs occlusion map: cast rays in all directions on the 2d plane and record distance.
// done exactly once per frame.

void computeraytable(float vx, float vy)
{
	if(!ocull) return;

	odist = getvar("fog")*1.5f;

	float apitch = (float)fabs(player1->pitch);
	float af = getvar("fov")/2+apitch/1.5f+3;
	float byaw = (player1->yaw-90+af)/360*PI2;
	float syaw = (player1->yaw-90-af)/360*PI2;

	loopi(NUMRAYS)
	{
		float angle = i*PI2/NUMRAYS;
		if((apitch>45 // must be bigger if fov>120
		|| (angle<byaw && angle>syaw)
		|| (angle<byaw-PI2 && angle>syaw-PI2)
		|| (angle<byaw+PI2 && angle>syaw+PI2))
		&& !OUTBORD(vx, vy)
		&& !SOLID(S(fast_f2nat(vx), fast_f2nat(vy))))	   // try to avoid tracing ray if outside of frustrum
		{
			float ray = i*8/(float)NUMRAYS;
			float dx, dy;
			if(ray>1 && ray<3) { dx = -(ray-2); dy = 1; }
			else if(ray>=3 && ray<5) { dx = -1; dy = -(ray-4); }
			else if(ray>=5 && ray<7) { dx = ray-6; dy = -1; }
			else { dx = 1; dy = ray>4 ? ray-8 : ray; };
			float sx = vx;
			float sy = vy;
			for(;;)
			{
				sx += dx;
				sy += dy;
				if(SOLID(S(fast_f2nat(sx), fast_f2nat(sy))))	// 90% of time spend in this function is on this line
				{
					rdist[i] = (float)(fabs(sx-vx)+fabs(sy-vy));
					break;
				};
			};
		}
		else
		{
			rdist[i] = 2;
		};
	};
};
