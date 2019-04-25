// console.cpp: the console buffer, its display, and command line control

#include "cube.h"
#include <ctype.h>
#include <X11/Xlib.h>
#include <SDL_syswm.h>

bool saycommandon = false;
string commandbuf; 

void conoutf(const char *s, ...)
{ 
	char sf[_MAXDEFSTR];
	va_list ap;
	va_start(ap, s);
	vprintf(s, ap);
        sf[_MAXDEFSTR-1] = 0;
	va_end(ap);
	printf("\n");
	fflush(stdout); 
}
// keymap is defined externally in keymap.cfg

struct keym { int code; char *name; char *action; } keyms[256];
int numkm = 0;									 

void keymap(char *code, char *key, char *action)
{
	keyms[numkm].code = atoi(code);
	keyms[numkm].name = newstring(key);
	keyms[numkm++].action = newstringbuf(action);
};

COMMAND(keymap, ARG_3STR);

void bindkey(char *key, char *action)
{
	for(char *x = key; *x; x++) *x = toupper(*x);
	loopi(numkm) if(strcmp(keyms[i].name, key)==0)
	{
		strcpy_s(keyms[i].action, action);
		return;
	};
	conoutf("unknown key \"%s\"", key);   
};

COMMANDN(bind, bindkey, ARG_2STR);

void saycommand(char *init) // turns input to the command line on or off
{
	SDL_EnableUNICODE(saycommandon = (init!=NULL));
	if(!editmode) keyrepeat(saycommandon);
	if(!init) init = "";
	strcpy_s(commandbuf, init);
};

void mapmsg(char *s) { strn0cpy(hdr.maptitle, s, 128); };

COMMAND(saycommand, ARG_VARI);
COMMAND(mapmsg, ARG_1STR);

void pasteconsole()
{ 
	SDL_SysWMinfo wminfo;
	SDL_VERSION(&wminfo.version); 
	wminfo.subsystem = SDL_SYSWM_X11;
	if(!SDL_GetWMInfo(&wminfo)) return;
	int cbsize;
	char *cb = XFetchBytes(wminfo.info.x11.display, &cbsize);
	if(!cb || !cbsize) return;
	int commandlen = strlen(commandbuf);
	for(char *cbline = cb, *cbend; commandlen + 1 < _MAXDEFSTR && cbline < &cb[cbsize]; cbline = cbend + 1)
	{
		cbend = (char *)memchr(cbline, '\0', &cb[cbsize] - cbline);
		if(!cbend) cbend = &cb[cbsize];
		if(commandlen + cbend - cbline + 1 > _MAXDEFSTR) cbend = cbline + _MAXDEFSTR - commandlen - 1;
		memcpy(&commandbuf[commandlen], cbline, cbend - cbline);
		commandlen += cbend - cbline;
		commandbuf[commandlen] = '\n';
		if(commandlen + 1 < _MAXDEFSTR && cbend < &cb[cbsize]) ++commandlen;
		commandbuf[commandlen] = '\0';
	};
	XFree(cb);
	
};

cvector vhistory;
int histpos = 0;

void history(int n)
{
	static bool rec = false;
	if(!rec && n>=0 && n<vhistory.length())
	{
		rec = true;
		execute(vhistory[vhistory.length()-n-1]);
		rec = false;
	};
};

COMMAND(history, ARG_1INT);

void keypress(int code, bool isdown, int cooked)
{
	if(saycommandon)// keystrokes go to commandline
	{
		if(isdown)
		{
			switch(code)
			{
				case SDLK_RETURN:
					break;

				case SDLK_BACKSPACE:
				case SDLK_LEFT:
				{
					for(int i = 0; commandbuf[i]; i++) if(!commandbuf[i+1]) commandbuf[i] = 0;
					resetcomplete();
					break;
				};
					
				case SDLK_UP:
					if(histpos) strcpy_s(commandbuf, vhistory[--histpos]);
					break;
				
				case SDLK_DOWN:
					if(histpos<vhistory.length()) strcpy_s(commandbuf, vhistory[histpos++]);
					break;
					
				case SDLK_TAB:
					complete(commandbuf);
					break;

				case SDLK_v:
					if(SDL_GetModState()&(KMOD_LCTRL|KMOD_RCTRL)) { pasteconsole(); return; };

				default:
					resetcomplete();
					if(cooked) { char add[] = { cooked, 0 }; strcat_s(commandbuf, add); };
			};
		}
		else
		{
			if(code==SDLK_RETURN)
			{
				if(commandbuf[0])
				{
					if(vhistory.empty() || strcmp(vhistory.last(), commandbuf))
					{
						vhistory.add(newstring(commandbuf));  // cap this?
					};
					histpos = vhistory.length();
					if(commandbuf[0]=='/') execute(commandbuf, true);
					else toserver(commandbuf);
				};
				saycommand(NULL);
			}
			else if(code==SDLK_ESCAPE)
			{
				saycommand(NULL);
			};
		};
	}
	else if(!menukey(code, isdown))	 // keystrokes go to menu
	{
		loopi(numkm) if(keyms[i].code==code)	// keystrokes go to game, lookup in keymap and execute
		{
			string temp;
			strcpy_s(temp, keyms[i].action);
			execute(temp, isdown); 
			return;
		};
	};
};

char *getcurcommand()
{
	return saycommandon ? commandbuf : NULL;
};

void writebinds(FILE *f)
{
	loopi(numkm)
	{
		if(*keyms[i].action) fprintf(f, "bind \"%s\" [%s]\n", keyms[i].name, keyms[i].action);
	};
};

void draw_textf(const char *fstr, int left, int top, int gl_num, ...)
{
        sprintf_sdlv(str, gl_num, fstr);
        draw_text(str, left, top, gl_num);
};
