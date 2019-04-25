#include <ccube.h>
#include <ctools.h>
#include <cheaders.h>

#define NUMRAYS 512

extern float rdist[NUMRAYS];
extern bool ocull;
extern float odist;
inline float ca(float x, float y) { return x>y ? y/x : 2-x/y; };
inline float ma(float x, float y) { return x==0 ? (y>0 ? 2 : -2) : y/x; };
int isoccluded(float vx, float vy, float cx, float cy, float csize)      // v = viewer, c = cube to test 
{
        if(!ocull) return 0;

        float nx = vx, ny = vy;  // n = point on the border of the cube that is closest to v
        if(nx<cx) nx = cx;
        else if(nx>cx+csize) nx = cx+csize;
        if(ny<cy) ny = cy;
        else if(ny>cy+csize) ny = cy+csize;
        float xdist = (float)fabs(nx-vx);
        float ydist = (float)fabs(ny-vy);
        if(xdist>odist || ydist>odist) return 2;
        float dist = xdist+ydist-1; // 1 needed?

        // ABC
        // D E
        // FGH

        // - check middle cube? BG

        // find highest and lowest angle in the occlusion map that this cube spans, based on its most left and right
        // points on the border from the viewer pov... I see no easier way to do this than this silly code below

        float h, l;
        if(cx<=vx)                        // ABDFG
        {
                if(cx+csize<vx)  // ADF
                {
                        if(cy<=vy)        // AD
                        {
                                if(cy+csize<vy) { h = ca(-(cx-vx), -(cy+csize-vy))+4; l = ca(-(cx+csize-vx), -(cy-vy))+4; }              // A
                                else                    { h = ma(-(cx+csize-vx), -(cy+csize-vy))+4; l =  ma(-(cx+csize-vx), -(cy-vy))+4; }  // D
                        }
                        else                            { h = ca(cy+csize-vy, -(cx+csize-vx))+2; l = ca(cy-vy, -(cx-vx))+2; };                    // F
                }
                else                            // BG
                {
                        if(cy<=vy)
                        {
                                if(cy+csize<vy) { h = ma(-(cy+csize-vy), cx-vx)+6; l = ma(-(cy+csize-vy), cx+csize-vx)+6; }              // B
                                else return 0;
                        }
                        else     { h = ma(cy-vy, -(cx+csize-vx))+2; l = ma(cy-vy, -(cx-vx))+2; };                                                          // G
                };
        }
        else                                    // CEH
        {
                if(cy<=vy)                // CE
                {
                        if(cy+csize<vy) { h = ca(-(cy-vy), cx-vx)+6; l = ca(-(cy+csize-vy), cx+csize-vx)+6; }                              // C
                        else                    { h = ma(cx-vx, cy-vy); l = ma(cx-vx, cy+csize-vy); };                                                            // E
                }
                else                            { h = ca(cx+csize-vx, cy-vy); l = ca(cx-vx, cy+csize-vy); };                                                    // H
        };
        int si = fast_f2nat(h*(NUMRAYS/8))+NUMRAYS;      // get indexes into occlusion map from angles
        int ei = fast_f2nat(l*(NUMRAYS/8))+NUMRAYS+1;
        if(ei<=si) ei += NUMRAYS;

        for(int i = si; i<=ei; i++)
        {
                if(dist<rdist[i&(NUMRAYS-1)]) return 0;  // if any value in this segment of the occlusion map is further away then cube is not occluded
        };

        return 1;                                                                          // cube is entirely occluded
};

