#include <ccube.h>
#include <ctools.h>
#include <cheaders.h>
extern int wx1, wy1, wx2, wy2;
void addwaterquad(int x, int y, int size)          // update bounding rect that contains water
{
        int x2 = x+size;
        int y2 = y+size;
        if(wx1<0)
        {
                wx1 = x;
                wy1 = y;
                wx2 = x2;
                wy2 = y2;
        }
        else
        {
                if(x<wx1) wx1 = x;
                if(y<wy1) wy1 = y;
                if(x2>wx2) wx2 = x2;
                if(y2>wy2) wy2 = y2;
        };
};

