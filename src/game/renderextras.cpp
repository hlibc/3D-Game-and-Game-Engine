// renderextras.cpp: misc gl render code and the HUD

#include "cube.h" 

const int MAXSPHERES = 50;
struct sphere { vec o; float size, max; int type; sphere *next; };
sphere spheres[MAXSPHERES], *slist = NULL, *sempty = NULL;
bool sinit = false;

void newsphere(vec &o, float max, int type)
{
	if(!sinit)
	{
		loopi(MAXSPHERES)
		{
			spheres[i].next = sempty;
			sempty = &spheres[i];
		};
		sinit = true;
	};
	if(sempty)
	{
		sphere *p = sempty;
		sempty = p->next;
		p->o = o;
		p->max = max;
		p->size = 1;
		p->type = type;
		p->next = slist;
		slist = p;
	};
};

string closeent;
const char *entnames[] =
{
	"none?", "light", "playerstart",
	"shells", "bullets", "rockets", "riflerounds",
	"health", "healthboost", "greenarmour", "yellowarmour", "quaddamage", 
	"teleport", "teledest", 
	"mapmodel", "monster", "trigger", "jumppad",
	"?", "?", "?", "?", "?", 
};

void renderents()	   // show sparkly thingies for map entities in edit mode
{
	closeent[0] = 0;
	if(!editmode) return;
	loopv(ents)
	{
		entity &e = ents[i];
		if(e.type==NOTUSED) continue;
		vec v = { e.x, e.y, e.z };
		particle_splash(2, 2, 40, v);
	};
	int e = closestent();
	if(e>=0)
	{
		entity &c = ents[e];
		sprintf_s(closeent)("closest entity = %s (%d, %d, %d, %d), selection = (%d, %d)", entnames[c.type], c.attr1, c.attr2, c.attr3, c.attr4, getvar("selxs"), getvar("selys"));
	};
};

void loadsky(char *basename)
{
	static string lastsky = "";
	if(strcmp(lastsky, basename)==0) return;
	const char *side[] = { "ft", "bk", "lf", "rt", "dn", "up" };
	int texnum = 14;
	loopi(6)
	{
		sprintf_sd(name)("packages/%s_%s.jpg", basename, side[i]);
		int xs, ys;
		if(!installtex(texnum+i, path(name), xs, ys, true)) conoutf("could not load sky textures");
	};
	strcpy_s(lastsky, basename);
};

COMMAND(loadsky, ARG_1STR);

float cursordepth = 0.9f;
GLint viewport[4];
GLdouble mm[16], pm[16];
vec worldpos; 


// find out the 3d target of the crosshair in the world easily and very acurately.
// sadly many very old cards and drivers appear to fuck up on glReadPixels() and give false
// coordinates, making shooting and such impossible.
// also hits map entities which is unwanted.
// could be replaced by a more acurate version of monster.cpp los() if needed

void readdepth(int w, int h)
{
	glReadPixels(w/2, h/2, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &cursordepth);
	double worldx = 0, worldy = 0, worldz = 0;
	gluUnProject(w/2, h/2, depthcorrect(cursordepth), mm, pm, viewport, &worldx, &worldz, &worldy);
	worldpos.x = (float)worldx;
	worldpos.y = (float)worldy;
	worldpos.z = (float)worldz;
	vec r = { (float)mm[0], (float)mm[4], (float)mm[8] };
	vec u = { (float)mm[1], (float)mm[5], (float)mm[9] };
	setorient(r, u);
}; 

VARP(crosshairsize, 0, 15, 50);

int dblend = 0;
void damageblend(int n) { dblend += n; };

VAR(hidestats, 0, 0, 1);
VARP(crosshairfx, 0, 1, 1);

void gl_drawhud(int curfps, int nquads, int curvert, bool underwater)
{
	readmatrices();
	if(editmode)
	{
		if(cursordepth==1.0f) worldpos = player1->o;
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		cursorupdate();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	};

	glDisable(GL_DEPTH_TEST);
	invertperspective();
	glPushMatrix();  
	glOrtho(0, VIRTW, VIRTH, 0, -1, 1);
	glEnable(GL_BLEND);

	glDepthMask(GL_FALSE);

	if(dblend || underwater)
	{
		glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
		glBegin(GL_QUADS);
		if(dblend) glColor3d(0.0f, 0.9f, 0.9f);
		else glColor3d(0.9f, 0.5f, 0.0f);
		glVertex2i(0, 0);
		glVertex2i(VIRTW, 0);
		glVertex2i(VIRTW, VIRTH);
		glVertex2i(0, VIRTH);
		glEnd();
		dblend -= curtime/3;
		if(dblend<0) dblend = 0;
	};

	glEnable(GL_TEXTURE_2D);

	char *command = getcurcommand();
	char *player = playerincrosshair();
	if(command) draw_textf("> %s_", 20, 1570, 2, command);
	else if(closeent[0]) draw_text(closeent, 20, 1570, 2);
	else if(player) draw_text(player, 20, 1570, 2);

	//renderscores();
	if(!rendermenu())
	{
		glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D, 1);
		glBegin(GL_QUADS);
		glColor3ub(255,255,255);
		if(crosshairfx)
		{
			if(player1->gunwait) glColor3ub(128,128,128);
			else if(player1->health<=25) glColor3ub(255,0,0);
			else if(player1->health<=50) glColor3ub(255,128,0);
		};
		float chsize = (float)crosshairsize;
		glTexCoord2d(0.0, 0.0); glVertex2f(VIRTW/2 - chsize, VIRTH/2 - chsize);
		glTexCoord2d(1.0, 0.0); glVertex2f(VIRTW/2 + chsize, VIRTH/2 - chsize);
		glTexCoord2d(1.0, 1.0); glVertex2f(VIRTW/2 + chsize, VIRTH/2 + chsize);
		glTexCoord2d(0.0, 1.0); glVertex2f(VIRTW/2 - chsize, VIRTH/2 + chsize);
		glEnd();
	};

	glPopMatrix();

	glPushMatrix();	
	glOrtho(0, VIRTW*4/3, VIRTH*4/3, 0, -1, 1);

	if(!hidestats)
	{
		glPopMatrix();
		glPushMatrix();
		glOrtho(0, VIRTW*3/2, VIRTH*3/2, 0, -1, 1);
		draw_textf("fps %d", 3200, 2390, 2, curfps);
		draw_textf("wqd %d", 3200, 2460, 2, nquads); 
		draw_textf("wvt %d", 3200, 2530, 2, curvert);
		draw_textf("evt %d", 3200, 2600, 2, xtraverts);
	};
	
	glPopMatrix();

	if(player1->state==CS_ALIVE)
	{
		glPushMatrix();
		glOrtho(0, VIRTW/2, VIRTH/2, 0, -1, 1);
		draw_textf("%d",  90, 827, 2, player1->health);
		if(player1->armour) draw_textf("%d", 390, 827, 2, player1->armour);
		draw_textf("%d", 690, 827, 2, player1->ammo[player1->gunselect]);
		glPopMatrix();
		glPushMatrix();
		glOrtho(0, VIRTW, VIRTH, 0, -1, 1);
		glDisable(GL_BLEND);
		drawicon(128, 128, 20, 1650);
		if(player1->armour) drawicon((float)(player1->armourtype*64), 0, 620, 1650); 
		int g = player1->gunselect;
		int r = 64;
		if(g>2) { g -= 3; r = 128; };
		drawicon((float)(g*64), (float)r, 1220, 1650);   
		glPopMatrix();
	};

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
};

