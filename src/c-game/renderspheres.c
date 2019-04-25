#include <ccube.h>
#include <ctools.h>
#include <cheaders.h>

typedef struct sphere { vec o; float size, max; int type; struct sphere *next; }sphere;
sphere *slist;
sphere *sempty;

void renderspheres(int time)
{
        glDepthMask(GL_FALSE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glBindTexture(GL_TEXTURE_2D, 4);

        for(sphere *p, **pp = &slist; (p = *pp);)
        {
                glPushMatrix();
                float size = p->size/p->max;
                glColor4f(1.0f, 1.0f, 1.0f, 1.0f-size);
                glTranslatef(p->o.x, p->o.z, p->o.y);
                glRotatef(lastmillis/5.0f, 1, 1, 1);
                glScalef(p->size, p->size, p->size);
                glCallList(1);
                glScalef(0.8f, 0.8f, 0.8f);
                glCallList(1);
                glPopMatrix();
                xtraverts += 12*6*2;

                if(p->size>p->max)
                {
                        *pp = p->next;
                        p->next = sempty;
                        sempty = p;
                }
                else
                {
                        p->size += time/100.0f;
                        pp = &p->next;
                };
        };

        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);
}
