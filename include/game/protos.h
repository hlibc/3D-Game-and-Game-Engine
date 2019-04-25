// command
extern int variable(const char *name, int min, int cur, int max, int *storage, void (*fun)(), bool persist);
extern void setvar(const char *name, int i);
extern int getvar(const char *name);
extern bool identexists(char *name);
extern bool addcommand(const char *name, void (*fun)(), int narg);
extern int execute(char *p, bool down = true);
extern void exec(const char *cfgfile);
extern bool execfile(const char *cfgfile);
extern void resetcomplete();
extern void complete(char *s);
extern void alias(char *name, char *action);
extern char *getalias(char *name);
extern void writecfg();

// console
extern void keypress(int code, bool isdown, int cooked);
extern char *getcurcommand();
extern void writebinds(FILE *f);

// menus
extern bool rendermenu();
extern void menuset(int menu);
extern void menumanual(int m, int n, char *text);
extern void sortmenu(int start, int num);
extern bool menukey(int code, bool isdown);
extern void newmenu(char *name);

// serverbrowser
extern void addserver(char *servername);
extern char *getservername(int n);
extern void writeservercfg();

// rendergl 

extern void gl_drawframe(int w, int h, float curfps);
extern bool installtex(int tnum, char *texname, int &xs, int &ys, bool clamp = false);
extern void mipstats(int a, int b, int c);
extern void vertf(float v1, float v2, float v3, sqr *ls, float t1, float t2);
extern void addstrip(int tex, int start, int n);
extern int lookuptexture(int tex, int &xs, int &ys);

// rendercubes
extern void resetcubes();
extern void render_flat(int tex, int x, int y, int size, int h, sqr *l1, sqr *l2, sqr *l3, sqr *l4, bool isceil);
extern void render_flatdelta(int wtex, int x, int y, int size, float h1, float h2, float h3, float h4, sqr *l1, sqr *l2, sqr *l3, sqr *l4, bool isceil);
extern void render_square(int wtex, float floor1, float floor2, float ceil1, float ceil2, int x1, int y1, int x2, int y2, int size, sqr *l1, sqr *l2, bool topleft);
extern void render_tris(int x, int y, int size, bool topleft, sqr *h1, sqr *h2, sqr *s, sqr *t, sqr *u, sqr *v);

extern int renderwater(float hf);
extern void finishstrips(); 

// client
extern void localservertoclient(uchar *buf, int len);
extern void connects(char *servername);
extern void toserver(char *text);
extern void addmsg(int rel, int num, int type, ...);
extern bool multiplayer();
extern bool allowedittoggle();
extern void sendpackettoserv(void *packet);
extern int getclientnum();
extern void changemapserv(const char *name, int mode); 

// clientgame 
extern void updateworld(int millis);
extern void startmap(const char *name);
extern void changemap(const char *name);
extern void spawnplayer(dynent *d);
extern void selfdamage(int damage, int actor, dynent *act);
extern dynent *newdynent();
extern char *getclientmap();
extern const char *modestr(int n);
extern void zapdynent(dynent *&d);
extern dynent *getclient(int cn);
extern void timeupdate(int timeremain); 

// clientextras
extern void renderclients();
extern void renderclient(dynent *d, bool team, char *mdlname, bool hellpig, float scale);
void showscores(bool on);
extern void renderscores();

// world

extern void empty_world(int factor, bool force);
extern void remip(block &b, int level = 0);
extern void remipmore(block &b, int level = 0);
extern int closestent();
extern int findentity(int type, int index = 0);
extern void trigger(int tag, int type, bool savegame);
extern void resettagareas();
extern void settagareas();
extern entity *newentity(int x, int y, int z, char *what, int v1, int v2, int v3, int v4);

// worldlight
extern void calclight();
extern void dodynlight(vec &v, int reach, int strength, dynent *owner);
extern void cleardlights();
extern block *blockcopy(block &b);
extern void blockpaste(block &b);

// worldrender
extern void render_world(float vx, float vy, float vh, int yaw, int pitch, float widef, int w, int h);

// worldocull
extern void computeraytable(float vx, float vy); 

// main
extern void fatal(const char *s); 

// rendertext 
extern void draw_textf(const char *fstr, int left, int top, int gl_num, ...); 

// editing
extern void cursorupdate();
extern void toggleedit();
extern void editdrag(bool isdown);
extern void setvdeltaxy(int delta, block &sel);
extern void editequalisexy(bool isfloor, block &sel);
extern void edittypexy(int type, block &sel);
extern void edittexxy(int type, int t, block &sel);
extern void editheightxy(bool isfloor, int amount, block &sel);
extern bool noteditmode();
extern void pruneundos(int maxremain = 0);

// renderextras 
extern void newsphere(vec &o, float max, int type); 
extern void gl_drawhud(int curfps, int nquads, int curvert, bool underwater);
extern void readdepth(int w, int h); 
extern void damageblend(int n);

// renderparticles
extern void setorient(vec &r, vec &u);
extern void particle_splash(int type, int num, int fade, vec &p);
extern void particle_trail(int type, int fade, vec &from, vec &to);


// worldio
extern void save_world(char *fname);
extern void load_world(const char *mname);
extern void writemap(char *mname, int msize, uchar *mdata);
extern uchar *readmap(char *mname, int *msize);
extern void loadgamerest();
extern void incomingdemodata(uchar *buf, int len, bool extras = false);
extern void demoplaybackstep();
extern void stop();
extern void demodamage(int damage, vec &o);


// physics
extern void moveplayer(dynent *pl, int moveres, bool local);
extern bool collide(dynent *d, bool spawn, float drop, float rise);
extern void entinmap(dynent *d);
extern void setentphysics(int mml, int mmr);
extern void physicsframe();

// sound
extern void playsound(int n, vec *loc = 0);
extern void playsoundc(int n);

extern void cleansound();

// rendermd2
extern void rendermodel(char *mdl, int frame, int range, int tex, float rad, float x, float y, float z, float yaw, float pitch, bool teammate, float scale, float speed, int snap = 0, int basetime = 0);
extern mapmodelinfo &getmminfo(int i);

// server
extern void initserver(bool dedicated, int uprate, char *sdesc, char *ip, char *master, char *passwd, int maxcl);
extern void cleanupserver();
extern void localconnect();
extern void localdisconnect();

extern void serverslice(int seconds, unsigned int timeout);
extern void putint(uchar *&p, int n);
extern int getint(uchar *&p);
extern void sendstring(char *t, uchar *&p);
extern void startintermission();
extern void restoreserverstate(vector<entity> &ents);
extern uchar *retrieveservers(uchar *buf, int buflen);
extern char msgsizelookup(int msg);
extern void serverms(int mode, int numplayers, int minremain, char *smapname, int seconds, bool isfull);
extern void servermsinit(const char *master, char *sdesc, bool listen);
extern void sendmaps(int n, string mapname, int mapsize, uchar *mapdata);


// weapon
extern void selectgun(int a = -1, int b = -1, int c =-1);
extern void shoot(dynent *d, vec &to);
extern void shootv(int gun, vec &from, vec &to, dynent *d = 0, bool local = false);
extern void createrays(vec &from, vec &to);
extern void moveprojectiles(float time);
extern void projreset();
extern char *playerincrosshair();
extern int reloadtime(int gun);

// monster
extern void monsterclear();
extern void restoremonsterstate();
extern void monsterthink();
extern void monsterrender();
extern dvector &getmonsters();
extern void monsterpain(dynent *m, int damage, dynent *d);
extern void endsp(bool allkilled);

// entities
extern void renderents();
extern void putitems(uchar *&p);
extern void checkquad(int time);
extern void checkitems();
extern void realpickup(int n, dynent *d);
extern void renderentities();
extern void resetspawns();
extern void setspawn(uint i, bool on);
extern void teleport(int n, dynent *d); 
extern void baseammo(int gun);
void spawnstate(dynent *d);

extern void neterr(const char *s);
void updatevol(void);

#include "../../include/c-game/cheaders.h"
