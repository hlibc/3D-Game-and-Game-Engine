#include <ccube.h>
#include <ctools.h>
#include <cheaders.h>

extern bool nosound;
#define MAXCHAN 32
#define SOUNDFREQ 22050 

typedef struct soundloc { vec loc; bool inuse; } soundloc;
soundloc soundlocs[MAXCHAN]; 
#include "SDL_mixer.h"
#define MAXVOL MIX_MAX_VOLUME
extern Mix_Music *mod;
extern void *stream;
extern int soundbufferlen;
extern int soundvol;
extern int musicvol;

void stopsound()
{
        if(nosound)
        return;
        if(mod)
        {
                Mix_HaltMusic();
                Mix_FreeMusic(mod);
                mod = NULL;
        }
        if(stream)
                stream = NULL;
}

void initsound()
{
        memset(soundlocs, 0, sizeof(soundloc)*MAXCHAN);
        if(Mix_OpenAudio(SOUNDFREQ, MIX_DEFAULT_FORMAT, 2, soundbufferlen)<0)
        {
                //conoutf("sound init failed (SDL_mixer): %s", (size_t)Mix_GetError());
                nosound = true;
        };
        Mix_AllocateChannels(MAXCHAN);
}

void music(char *name)
{
        if(nosound) return;
        stopsound();
        if(soundvol && musicvol)
        {
                string sn;
                strcpy_s(sn, "packages/");
                strcat_s(sn, name);

                if((mod = Mix_LoadMUS(path(sn))))
                {
                        Mix_PlayMusic(mod, -1);
                        Mix_VolumeMusic((musicvol*MAXVOL)/255);
                }
        }
}
