#include <ccube.h>
#include <ctools.h>
#include <cheaders.h>


#define MAXFRAMES 2
extern int mapping[256][MAXFRAMES];                            // ( cube texture, frame ) -> ( opengl id, name )
extern string mapname[256][MAXFRAMES];

void purgetextures()
{
        loopi(256) loop(j,MAXFRAMES) mapping[i][j] = 0;
}



