// server.cpp: little more than enhanced multicaster
// runs dedicated or as client coroutine

#include "cube.h" 

enum { ST_EMPTY, ST_LOCAL, ST_TCPIP }; 
struct client				   // server side version of "dynent" type
{
	int type; 
	string hostname;
	string mapvote;
	string name;
	int modevote;
};

vector<client> clients; 
int maxclients = 8;
string smapname;
struct server_entity			// server side version of "entity" type
{
	bool spawned;
	int spawnsecs;
};

vector<server_entity> sents;

bool notgotitems = true;		// true when map has changed and waiting for clients to send item
int mode = 0;

void restoreserverstate(vector<entity> &ents)   // hack: called from savegame code, only works in SP
{
	loopv(sents)
	{
		sents[i].spawned = ents[i].spawned;
		sents[i].spawnsecs = 0;
	}; 
};

int interm = 0, minremain = 0, mapend = 0;
bool mapreload = false; 
bool isdedicated;

int bsend = 0, brec = 0, laststatus = 0, lastsec = 0; 
#define MAXOBUF 100000 
void resetitems() { sents.setsize(0); notgotitems = true; };

void pickup(uint i, int sec, int sender)		 // server side item pickup, acknowledge first client that gets it
{
	if(i>=(uint)sents.length()) return;
	if(sents[i].spawned)
	{
		sents[i].spawned = false;
		sents[i].spawnsecs = sec; 
	};
};

void resetvotes()
{
	loopv(clients) clients[i].mapvote[0] = 0;
}; 

void send_welcome(int n)
{ 
	uchar *start = (uchar *)malloc(1000); 
	uchar *p = start + 2;
	putint(p, SV_INITS2C);
	putint(p, n); 
	localservertoclient(start, 4); 
}; 


client &addclient()
{
	loopv(clients) if(clients[i].type==ST_EMPTY) return clients[i];
	return clients.add();
};

void checkintermission()
{
	if(!minremain)
	{
		interm = lastsec+10;
		mapend = lastsec+1000;
	};
};

void startintermission() { minremain = 0; checkintermission(); }; 



void localconnect()
{
	client &c = addclient();
	c.type = ST_LOCAL;
	strcpy_s(c.hostname, "local");
	send_welcome(&c-&clients[0]); 
}; 

// all network traffic is in 32bit ints, which are then compressed using the following simple scheme (assumes that most values are small).

void putint(uchar *&p, int n)
{
	if(n<128 && n>-127) { *p++ = n; }
	else if(n<0x8000 && n>=-0x8000) { *p++ = 0x80; *p++ = n; *p++ = n>>8;  }
	else { *p++ = 0x81; *p++ = n; *p++ = n>>8; *p++ = n>>16; *p++ = n>>24; };
};

int getint(uchar *&p)
{
	int c = *((char *)p);
	p++;
	if(c==-128) { int n = *p++; n |= *((char *)p)<<8; p++; return n;}
	else if(c==-127) { int n = *p++; n |= *p++<<8; n |= *p++<<16; return n|(*p++<<24); } 
	else return c;
};

void sendstring(char *t, uchar *&p)
{
	while(*t) putint(p, *t++);
	putint(p, 0);
};

const char *modenames[] =
{
	"SP", "DMSP", "ffa/default", "coopedit", "ffa/duel", "teamplay",
	"instagib", "instagib team", "efficiency", "efficiency team",
	"insta arena", "insta clan arena", "tactics arena", "tactics clan arena",
};
	  
const char *modestr(int n) { return (n>=-2 && n<12) ? modenames[n+2] : "unknown"; };

char msgsizesl[] =  // size inclusive message token, 0 for variable or not-checked sizes
{ 
	SV_INITS2C, 4, SV_INITC2S, 0, SV_POS, 12, SV_TEXT, 0, SV_SOUND, 2, SV_CDIS, 2,
	SV_EDITH, 7, SV_EDITT, 7, SV_EDITS, 6, SV_EDITD, 6, SV_EDITE, 6,
	SV_DIED, 2, SV_DAMAGE, 4, SV_SHOT, 8, SV_FRAGS, 2,
	SV_MAPCHANGE, 0, SV_ITEMSPAWN, 2, SV_ITEMPICKUP, 3, SV_DENIED, 2,
	SV_PING, 2, SV_PONG, 2, SV_CLIENTPING, 2, SV_GAMEMODE, 2,
	SV_TIMEUP, 2, SV_EDITENT, 10, SV_MAPRELOAD, 2, SV_ITEMACC, 2,
	SV_SENDMAP, 0, SV_RECVMAP, 1, SV_SERVMSG, 0, SV_ITEMLIST, 0,
	SV_EXT, 0,
	-1
};

char msgsizelookup(int msg)
{
	for(char *p = msgsizesl; *p>=0; p += 2) if(*p==msg) return p[1];
	return -1;
}; 

