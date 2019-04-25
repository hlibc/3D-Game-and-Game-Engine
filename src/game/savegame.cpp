// loading and saving of savegames.  dumps the spawn state of all mapents, the full state of all dynents (monsters + player)

#include "cube.h"

extern int islittleendian;

gzFile f = NULL; 
dvector playerhistory; 
void startdemo();

void gzput(int i) { gzputc(f, i); };
void gzputi(int i) { gzwrite(f, &i, sizeof(int)); };
void gzputv(vec &v) { gzwrite(f, &v, sizeof(vec)); };

void gzcheck(int a, int b) { ;/*if(a!=b) fatal("savegame file corrupt (short)");*/ };
int gzget() { char c = gzgetc(f); return c; };
int gzgeti() { int i; gzcheck(gzread(f, &i, sizeof(int)), sizeof(int)); return i; };
void gzgetv(vec &v) { gzcheck(gzread(f, &v, sizeof(vec)), sizeof(vec)); };

void stop()
{
	if(f) 
		gzclose(f);
	
	f = NULL; 
	loopv(playerhistory) zapdynent(playerhistory[i]);
	playerhistory.setsize(0);
}; 

void savestate(char *fn)
{
	stop();
	f = gzopen(fn, "wb9");
	if(!f) { conoutf("could not write %s", fn); return; };
	gzwrite(f, (void *)"CUBESAVE", 8);
	gzputc(f, islittleendian);  
	gzputi(SAVEGAMEVERSION);
	gzputi(sizeof(dynent));
	gzwrite(f, getclientmap(), _MAXDEFSTR);
	gzputi(gamemode);
	gzputi(ents.length());
	loopv(ents) gzputc(f, ents[i].spawned);
	gzwrite(f, player1, sizeof(dynent));
	dvector &monsters = getmonsters();
	gzputi(monsters.length());
	loopv(monsters) gzwrite(f, monsters[i], sizeof(dynent));
	gzputi(players.length());
	loopv(players)
	{
		gzput(players[i]==NULL);
		gzwrite(f, players[i], sizeof(dynent));
	};
};

void savegame(char *name)
{
	if(!m_classicsp) { conoutf("can only save classic sp games"); return; };
	sprintf_sd(fn)("savegames/%s.csgz", name);
	savestate(fn);
	stop();
	conoutf("wrote %s", fn);
};

void loadstate(char *fn)
{
	stop();
	if(multiplayer()) return;
	f = gzopen(fn, "rb9");
	if(!f) { conoutf("could not open %s", fn); return; };
	
	string buf;
	gzread(f, buf, 8);
	if(strncmp(buf, "CUBESAVE", 8)) goto out;
	if(gzgetc(f)!=islittleendian) goto out;	 // not supporting save->load accross incompatible architectures simpifies things a LOT
	if(gzgeti()!=SAVEGAMEVERSION || gzgeti()!=sizeof(dynent)) goto out;
	string mapname;
	gzread(f, mapname, _MAXDEFSTR);
	nextmode = gzgeti();
	changemap(mapname); // continue below once map has been loaded and client & server have updated 
	return;
	out:	
	conoutf("aborting: savegame/demo from a different version of cube or cpu architecture");
	stop();
};

void loadgame(char *name)
{
	sprintf_sd(fn)("savegames/%s.csgz", name);
	loadstate(fn);
};

void loadgameout()
{
	stop();
	conoutf("loadgame incomplete: savegame from a different version of this map");
};

void loadgamerest()
{
	if(gzgeti()!=ents.length()) return loadgameout();
	loopv(ents)
	{
		ents[i].spawned = gzgetc(f)!=0;   
		if(ents[i].type==CARROT && !ents[i].spawned) trigger(ents[i].attr1, ents[i].attr2, true);
	};
	restoreserverstate(ents);
	
	gzread(f, player1, sizeof(dynent));
	player1->lastaction = lastmillis;
	
	int nmonsters = gzgeti();
	dvector &monsters = getmonsters();
	if(nmonsters!=monsters.length()) return loadgameout();
	loopv(monsters)
	{
		gzread(f, monsters[i], sizeof(dynent));
		monsters[i]->enemy = player1;
		monsters[i]->lastaction = monsters[i]->trigger = lastmillis+500;
		if(monsters[i]->state==CS_DEAD) monsters[i]->lastaction = 0;
	};
	restoremonsterstate();
	
	int nplayers = gzgeti();
	loopi(nplayers) if(!gzget())
	{
		dynent *d = getclient(i);
		assert(d);
		gzread(f, d, sizeof(dynent));		
	};
	
	conoutf("savegame restored");

	stop();
}; 

int starttime = 0;
int playbacktime = 0;
int ddamage, bdamage;
vec dorig; 

COMMAND(savegame, ARG_1STR);
COMMAND(loadgame, ARG_1STR);
