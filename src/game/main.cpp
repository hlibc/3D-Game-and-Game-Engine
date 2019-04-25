// main.cpp: initialisation & main loop

#include "cube.h"

void cleanup(char *msg)		 // single program exit point;
{
	stop(); 
	writecfg();
	cleangl();
	cleansound();
	SDL_ShowCursor(1);
	if(msg) 
		printf("msg");
	SDL_Quit();
	exit(1);
};

void quit()					 // normal exit
{ 
	cleanup(NULL);
}

void fatal(const char *s)	// failure exit
{ 
	sprintf_sd(msg)("%s (%s)\n", s, SDL_GetError());
	cleanup(msg);
}

int scr_w = 640;
int scr_h = 480;

COMMAND(quit, ARG_NONE); 

VARF(gamespeed, 10, 100, 1000, if(multiplayer()) gamespeed = 100);
VARP(minmillis, 0, 5, 1000);

int islittleendian = 1;
int framesinmap = 0;

int main(int argc, char **argv)
{	

	int fs = SDL_FULLSCREEN;
	int par = 0; 
	static float fps = 30.0f;
	islittleendian = *((char *)&islittleendian); 
	#define log(s) conoutf("init: %s", s)
	log("sdl");
	
	for(int i = 1; i<argc; i++)
	{
		char *a = &argv[i][2];
		if(argv[i][0]=='-') switch(argv[i][1])
		{ 
			case 't': fs	 = 0; break;
			case 'w': scr_w  = atoi(a); break;
			case 'h': scr_h  = atoi(a); break;
			default:  conoutf("unknown commandline option");
		}
		else conoutf("unknown commandline argument");
	}; 

	if(SDL_Init(SDL_INIT_TIMER|SDL_INIT_VIDEO|par)<0) fatal("Unable to initialize SDL");

	log("net"); 
	initclient(); 
	log("world");
	empty_world(7, true);

	log("video: sdl");
	if(SDL_InitSubSystem(SDL_INIT_VIDEO)<0) fatal("Unable to initialize SDL Video");

	log("video: mode");
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	if(SDL_SetVideoMode(scr_w, scr_h, 0, SDL_OPENGL|fs)==NULL) fatal("Unable to create OpenGL screen");

	log("video: misc");
	SDL_WM_SetCaption("cube engine", NULL);
	SDL_WM_GrabInput(SDL_GRAB_ON);
	keyrepeat(false);
	SDL_ShowCursor(0);

	log("gl");
	gl_init(scr_w, scr_h);

	log("basetex");
	int xs, ys;
	if(!installtex(2,  path(newstring("data/newchars.png")), xs, ys) || 
	   !installtex(4,  path(newstring("data/explosion.jpg")), xs, ys) ||
	   !installtex(5,  path(newstring("data/items.png")), xs, ys) ||
	   !installtex(1,  path(newstring("data/crosshair.png")), xs, ys)) 
		fatal("could not find core textures (hint: run cube from the parent of the bin directory)");

	log("sound");
	initsound();

	log("cfg");
	newmenu("frags\tpj\tping\tteam\tname");
	newmenu("ping\tplr\tserver");
	exec("data/keymap.cfg");
	exec("data/menus.cfg");
	exec("data/prefabs.cfg");
	exec("data/sounds.cfg");
	exec("servers.cfg");
	if(!execfile("config.cfg")) 
		execfile("data/defaults.cfg");
	exec("autoexec.cfg");

	log("localconnect");
	localconnect();
	changemapserv("grafland", -2); 
	
	log("mainloop");
	int ignore = 5;
	for(;;)
	{
		int millis = SDL_GetTicks()*gamespeed/100;
		if(millis-lastmillis>200) lastmillis = millis-200;
		else if(millis-lastmillis<1) lastmillis = millis-1;
		if(millis-lastmillis<minmillis) SDL_Delay(minmillis-(millis-lastmillis));
		cleardlights();
		updateworld(millis);
		fps = 30.0f;
		fps = (1000.0f/curtime+fps*50)/51;
		computeraytable(player1->o.x, player1->o.y);
		readdepth(scr_w, scr_h);
		SDL_GL_SwapBuffers(); 
		updatevol(); 
		gl_drawframe(scr_w, scr_h, fps);
		SDL_Event event;
		int lasttype = 0, lastbut = 0;
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					quit();
					break;

				case SDL_KEYDOWN: 
				case SDL_KEYUP: 
					keypress(event.key.keysym.sym, event.key.state==SDL_PRESSED, event.key.keysym.unicode);
					break;

				case SDL_MOUSEMOTION:
					if(ignore) { ignore--; break; };
					mousemove(event.motion.xrel, event.motion.yrel);
					break;

				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
					if(lasttype==event.type && lastbut==event.button.button) 
						break; // why?? get event twice without it
					keypress(-event.button.button, event.button.state!=0, 0);
					lasttype = event.type;
					lastbut = event.button.button;
					break;
			};
		};
	};
	quit();
	return 1;
};


