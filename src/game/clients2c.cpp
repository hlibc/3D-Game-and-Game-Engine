// client processing of the incoming network stream

#include "cube.h"

extern int clientnum;
extern bool c2sinit, senditemstoserver;
extern string toservermap;
extern string clientpassword; 

void changemapserv(const char *name, int mode)	// forced map change from the server
{
	gamemode = mode;
	load_world(name);
};

void changemap(const char *name)  // request map change, server may ignore
{
	strcpy_s(toservermap, name);
}; 

void localservertoclient(uchar *buf, int len)   // processes any updates from the server
{ 

	uchar *end = buf+len;
	uchar *p = buf+2;
	char text[MAXTRANS];
	int cn = -1, type;
	dynent *d = NULL;
	bool mapchanged = false;

	while(p<end) switch(type = getint(p))
	{
		case SV_INITS2C:// welcome messsage from the server
		{
			cn = getint(p); 
			toservermap[0] = 0;
			clientnum = cn;	 // we are now fully connected
			if(!getint(p)) strcpy_s(toservermap, getclientmap());   // we are the first client on this server, set map
			sgetstr(); 
			break;
		};

		case SV_POS:	// position of another client
		{
			cn = getint(p);
			d = getclient(cn);
			if(!d) return;
			d->o.x   = getint(p)/DMF;
			d->o.y   = getint(p)/DMF;
			d->o.z   = getint(p)/DMF;
			d->yaw   = getint(p)/DAF;
			d->pitch = getint(p)/DAF;
			d->roll  = getint(p)/DAF;
			d->vel.x = getint(p)/DVF;
			d->vel.y = getint(p)/DVF;
			d->vel.z = getint(p)/DVF;
			int f = getint(p);
			d->strafe = (f&3)==3 ? -1 : f&3;
			f >>= 2; 
			d->move = (f&3)==3 ? -1 : f&3;
			d->onfloor = (f>>2)&1;
			int state = f>>3;
			if(state==CS_DEAD && d->state!=CS_DEAD) d->lastaction = lastmillis;
			d->state = state;
			
			break;
		};

		case SV_SOUND:
			playsound(getint(p), &d->o);
			break;

		case SV_TEXT:
			sgetstr();
			conoutf("%s:\f %s", d->name, text); 
			break;

		case SV_MAPCHANGE:	 
			sgetstr();
			changemapserv(text, getint(p));
			mapchanged = true;
			break;
		
		case SV_ITEMLIST:
		{
			int n;
			if(mapchanged) { senditemstoserver = false; resetspawns(); };
			while((n = getint(p))!=-1) if(mapchanged) setspawn(n, true);
			break;
		};

		case SV_MAPRELOAD:  // server requests next map
		{
			getint(p);
			sprintf_sd(nextmapalias)("nextmap_%s", getclientmap());
			char *map = getalias(nextmapalias);	 // look up map in the cycle
			changemap(map ? map : getclientmap());
			break;
		};

		case SV_INITC2S:	// another client either connected or changed name/team
		{
			sgetstr();
			if(d->name[0])	  // already connected
			{
				if(strcmp(d->name, text))
					conoutf("%s is now known as %s", d->name, text);
			}
			else	// new client
			{
				c2sinit = false;// send new players my info again 
				conoutf("connected: %s", text);
			}; 
			strcpy_s(d->name, text);
			sgetstr();
			strcpy_s(d->team, text);
			d->lifesequence = getint(p);
			break;
		};

		case SV_CDIS:
			cn = getint(p);
			if(!(d = getclient(cn))) break;
			conoutf("player %s disconnected", d->name[0] ? d->name : "[incompatible client]"); 
			zapdynent(players[cn]);
			break;

		case SV_SHOT:
		{
			int gun = getint(p);
			vec s, e;
			s.x = getint(p)/DMF;
			s.y = getint(p)/DMF;
			s.z = getint(p)/DMF;
			e.x = getint(p)/DMF;
			e.y = getint(p)/DMF;
			e.z = getint(p)/DMF;
			if(gun==GUN_SG) createrays(s, e);
			shootv(gun, s, e, d);
			break;
		};

		case SV_DAMAGE:			 
		{
			int target = getint(p);
			int damage = getint(p);
			int ls = getint(p);
			if(target==clientnum) { if(ls==player1->lifesequence) selfdamage(damage, cn, d); }
			else playsound(S_PAIN1+rnd(5), &getclient(target)->o);
			break;
		};

		case SV_DIED:
		{
			int actor = getint(p);
			if(actor==cn)
			{
				conoutf("%s suicided", d->name);
			}
			else if(actor==clientnum)
			{
				int frags;
				if(isteam(player1->team, d->team))
				{
					frags = -1;
					conoutf("you fragged a teammate (%s)", d->name);
				}
				else
				{
					frags = 1;
					conoutf("you fragged %s", d->name);
				};
				addmsg(1, 2, SV_FRAGS, player1->frags += frags);
			}
			else
			{
				dynent *a = getclient(actor);
				if(a)
				{
					if(isteam(a->team, d->name))
					{
						conoutf("%s fragged his teammate (%s)", a->name, d->name);
					}
					else
					{
						conoutf("%s fragged %s", a->name, d->name);
					};
				};
			};
			playsound(S_DIE1+rnd(2), &d->o);
			d->lifesequence++;
			break;
		};

		case SV_FRAGS:
			players[cn]->frags = getint(p);
			break;

		case SV_ITEMPICKUP:
			setspawn(getint(p), false);
			getint(p);
			break;

		case SV_ITEMSPAWN:
		{
			uint i = getint(p);
			setspawn(i, true);
			if(i>=(uint)ents.length()) break;
			vec v = { ents[i].x, ents[i].y, ents[i].z };
			playsound(S_ITEMSPAWN, &v); 
			break;
		};

		case SV_ITEMACC:// server acknowledges that I picked up this item
			realpickup(getint(p), player1);
			break;

		case SV_EDITH:  // coop editing messages, should be extended to include all possible editing ops
		case SV_EDITT:
		case SV_EDITS:
		case SV_EDITD:
		case SV_EDITE:
		{
			int x  = getint(p);
			int y  = getint(p);
			int xs = getint(p);
			int ys = getint(p);
			int v  = getint(p);
			block b = { x, y, xs, ys };
			switch(type)
			{
				case SV_EDITH: editheightxy(v!=0, getint(p), b); break;
				case SV_EDITT: edittexxy(v, getint(p), b); break;
				case SV_EDITS: edittypexy(v, b); break;
				case SV_EDITD: setvdeltaxy(v, b); break;
				case SV_EDITE: editequalisexy(v!=0, b); break;
			};
			break;
		};

		case SV_EDITENT:// coop edit of ent
		{
			uint i = getint(p);
			while((uint)ents.length()<=i) ents.add().type = NOTUSED;
			int to = ents[i].type;
			ents[i].type = getint(p);
			ents[i].x = getint(p);
			ents[i].y = getint(p);
			ents[i].z = getint(p);
			ents[i].attr1 = getint(p);
			ents[i].attr2 = getint(p);
			ents[i].attr3 = getint(p);
			ents[i].attr4 = getint(p);
			ents[i].spawned = false;
			if(ents[i].type==LIGHT || to==LIGHT) calclight();
			break;
		};

		case SV_PING:
			getint(p);
			break;

		case SV_PONG: 
			addmsg(0, 2, SV_CLIENTPING, player1->ping = (player1->ping*5+lastmillis-getint(p))/6);
			break;

		case SV_CLIENTPING:
			players[cn]->ping = getint(p);
			break;

		case SV_GAMEMODE:
			nextmode = getint(p);
			break;

		case SV_TIMEUP:
			timeupdate(getint(p));
			break;

		case SV_RECVMAP:
		{
			sgetstr();
			conoutf("received map \"%s\" from server, reloading..", text);
			int mapsize = getint(p);
			writemap(text, mapsize, p);
			p += mapsize;
			changemapserv(text, gamemode);
			break;
		};
		
		case SV_SERVMSG:
			sgetstr();
			conoutf("%s", text);
			break;

		case SV_EXT:	// so we can messages without breaking previous clients/servers, if necessary
		{
			for(int n = getint(p); n; n--) getint(p);
			break;
		};

		default:
			return;
	};
};
