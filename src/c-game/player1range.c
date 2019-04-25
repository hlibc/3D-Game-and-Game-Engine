#include <ccube.h>
#include <ctools.h>
#include <cheaders.h>


void fixplayer1range()
{
        const float MAXPITCH = 90.0f;
        if(player1->pitch>MAXPITCH) player1->pitch = MAXPITCH;
        if(player1->pitch<-MAXPITCH) player1->pitch = -MAXPITCH;
        while(player1->yaw<0.0f) player1->yaw += 360.0f;
        while(player1->yaw>=360.0f) player1->yaw -= 360.0f;
}
