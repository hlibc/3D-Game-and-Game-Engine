// renderparticles.cpp

#include "cube.h"

const int MAXPARTICLES = 10500;
const int NUMPARTCUTOFF = 20;
struct particle { vec o, d; int fade, type; int millis; particle *next; };
particle particles[MAXPARTICLES], *parlist = NULL, *parempty = NULL;
bool parinit = false;

VARP(maxparticles, 100, 2000, MAXPARTICLES-500);
VARP(particlesize, 20, 100, 500);

void newparticle(vec &o, vec &d, int fade, int type)
{
	if(!parinit)
	{
		loopi(MAXPARTICLES)
		{
			particles[i].next = parempty;
			parempty = &particles[i];
		};
		parinit = true;
	};
	if(parempty)
	{
		particle *p = parempty;
		parempty = p->next;
		p->o = o;
		p->d = d;
		p->fade = fade;
		p->type = type;
		p->millis = lastmillis;
		p->next = parlist;
		parlist = p;
	};
}; 

vec right, up;
void setorient(vec &r, vec &u) { right = r; up = u; };

void particle_splash(int type, int num, int fade, vec &p)
{
	loopi(num)
	{
		const int radius = type==5 ? 50 : 150;
		int x, y, z;
		do
		{
			x = rnd(radius*2)-radius;
			y = rnd(radius*2)-radius;
			z = rnd(radius*2)-radius;
		}
		while(x*x+y*y+z*z>radius*radius);
		vec d = { (float)x, (float)y, (float)z };
		newparticle(p, d, rnd(fade*3), type);
	};
};

void particle_trail(int type, int fade, vec &s, vec &e)
{
	vdist(d, v, s, e);
	vdiv(v, d*2+0.1f);
	vec p = s;
	loopi((int)d*2)
	{
		vadd(p, v);
		vec d = { float(rnd(11)-5), float(rnd(11)-5), float(rnd(11)-5) };
		newparticle(p, d, rnd(fade)+fade, type);
	};
};

