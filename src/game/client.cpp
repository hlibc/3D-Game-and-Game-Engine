// client.cpp, mostly network related client game code 
#include "cube.h" 
int clientnum = -1;		 // our client id in the game
bool c2sinit = false;	   // whether we need to tell the other clients our stats 
int getclientnum()
{
	return clientnum;
}

bool multiplayer()
{ 
};

bool allowedittoggle()
{ 
	return 1;
}; 

void newname(const char *name) { c2sinit = false; strn0cpy(player1->name, name, 16); };
void newteam(const char *name) { c2sinit = false; strn0cpy(player1->team, name, 5); };

COMMANDN(team, newteam, ARG_1STR);
COMMANDN(name, newname, ARG_1STR);

string ctext;

void toserver(char *text) 
{
	conoutf("%s:\f %s", player1->name, text); 
	strn0cpy(ctext, text, 80); 
}

void echo(char *text)
{
	conoutf("%s", text);
}

COMMAND(echo, ARG_VARI);
COMMANDN(say, toserver, ARG_VARI); 
// collect c2s messages conveniently

vector<ivector> messages;

void addmsg(int rel, int num, int type, ...)
{ 
	if(num!=msgsizelookup(type)) 
	{ 
		sprintf_sd(s)("inconsistant msg size for %d (%d != %d)", type, num, msgsizelookup(type)); 
		fatal(s); 
	}

	if(messages.length()==100) 
	{
		conoutf("command flood protection (type %d)", type);
		return; 
	}
	ivector &msg = messages.add();
	msg.add(num);
	msg.add(rel);
	msg.add(type);
	va_list marker;
	va_start(marker, type); 
	loopi(num-1) 
		msg.add(va_arg(marker, int));
	va_end(marker);  
}; 


int lastupdate = 0, lastping = 0;
string toservermap;
bool senditemstoserver = false;	 // after a map change, since server doesn't have map data

string clientpassword;
void password(char *p) { strcpy_s(clientpassword, p); };
COMMAND(password, ARG_1STR); 

// clientextras.cpp: stuff that didn't fit in client.cpp or clientgame.cpp :) 

// render players & monsters
// very messy ad-hoc handling of animation frames, should be made more configurable

//  D	D D D'   D D D D'   A   A'  P   P'  I   I' R,  R'  E L J   J'
int frame[] = { 178, 184, 190, 137, 183, 189, 197, 164, 46, 51, 54, 32, 0,  0, 40, 1,  162, 162, 67, 168 };
int range[] = { 6,   6,   8,   28,  1,   1,   1,   1,   8,  19, 4,  18, 40, 1, 6,  15, 1,   1,   1,  1   };

void renderclient(dynent *d, bool team, char *mdlname, bool hellpig, float scale)
{
	int n = 3;
	float speed = 100.0f;
	float mz = d->o.z-d->eyeheight+1.55f*scale;
	int basetime = -((intptr_t)d&0xFFF);
	if(d->state==CS_DEAD)
	{
		int r;
		if(hellpig) { n = 2; r = range[3]; } else { n = (intptr_t)d%3; r = range[n]; };
		basetime = d->lastaction;
		int t = lastmillis-d->lastaction;
		if(t<0 || t>20000) return;
		if(t>(r-1)*100) { n += 4; if(t>(r+10)*100) { t -= (r+10)*100; mz -= t*t/10000000000.0f*t; }; };
		if(mz<-1000) return;
	}
	else if(d->state==CS_EDITING){ n = 16; }
	else if(d->state==CS_LAGGED){ n = 17; }
	else if(d->monsterstate==M_ATTACKING){ n = 8; }
	else if(d->monsterstate==M_PAIN){ n = 10; } 
	else if((!d->move && !d->strafe) || !d->moving){ n = 12; } 
	else if(!d->onfloor && d->timeinair>100){ n = 18; }
	else{ n = 14; speed = 1200/d->maxspeed*scale; if(hellpig) speed = 300/d->maxspeed; }; 
	if(hellpig) { n++; scale *= 32; mz -= 1.9f; };
	rendermodel(mdlname, frame[n], range[n], 0, 1.5f, d->o.x, mz, d->o.y, d->yaw+90, d->pitch/2, team, scale, speed, 0, basetime);
};

void renderclients()
{
	dynent *d;
	loopv(players)
		 if((d = players[i]) )
			renderclient(d, isteam(player1->team, d->team), "monster/ogro", false, 1.0f);
};

// creation of scoreboard pseudo-menu

bool scoreson = false;

void showscores(bool on)
{
	scoreson = on;
	menuset(((int)on)-1);
};
