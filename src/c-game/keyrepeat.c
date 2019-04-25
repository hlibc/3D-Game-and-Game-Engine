#include <ccube.h>
#include <ctools.h>
#include <cheaders.h>

void keyrepeat(bool on)
{
        SDL_EnableKeyRepeat(on ? SDL_DEFAULT_REPEAT_DELAY : 0, SDL_DEFAULT_REPEAT_INTERVAL);
}

