#include <ccube.h>
#include <ctools.h>
#include <cheaders.h>

extern int hasoverbright;

void setupworld2(void)
{
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        setarraypointers();

        if(hasoverbright)
        {
                glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
                glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
                glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE);
                glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PRIMARY_COLOR_EXT);
       
	}
}

extern char *entnames[];                                // lookup from map entities above to strings

extern sqr *world;
extern int sfactor, ssize, cubicsize, mipsize;

extern header hdr;

void setupworld(int factor)
{
        ssize = 1<<(sfactor = factor);
        cubicsize = ssize*ssize;
        mipsize = cubicsize*134/100;
        sqr *w = world = (sqr *)alloc(mipsize*sizeof(sqr));
        loopi(LARGEST_FACTOR*2) { wmip[i] = w; w += cubicsize>>(i*2); };
};
