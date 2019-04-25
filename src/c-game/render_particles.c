#include <ccube.h>
#include <ctools.h>
#include <cheaders.h>

#define MAXPARTICLES 10500
#define NUMPARTCUTOFF  20
extern vec right, up;
typedef struct particle { vec o, d; int fade, type; int millis; struct particle *next; }particle;
extern particle particles[MAXPARTICLES], *parlist, *parempty;

extern int particlesize;
extern int maxparticles;
void render_particles(int time)
{ 

        glDepthMask(GL_FALSE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);
        glDisable(GL_FOG);
        
        typedef struct parttype { float r, g, b; int gr, tex; float sz; } parttype;
	parttype parttypes[] =
        {
                { 0.7f, 0.6f, 0.3f, 2,  3, 0.06f }, // yellow: sparks 
                { 0.5f, 0.5f, 0.5f, 20, 7, 0.15f }, // grey:   small smoke
                { 0.2f, 0.2f, 1.0f, 20, 3, 0.08f }, // blue:   edit mode entities
                { 1.0f, 0.1f, 0.1f, 1,  7, 0.06f }, // red:     blood spats
                { 1.0f, 0.8f, 0.8f, 20, 6, 1.2f  }, // yellow: fireball1
                { 0.5f, 0.5f, 0.5f, 20, 7, 0.6f  }, // grey:   big smoke   
                { 1.0f, 1.0f, 1.0f, 20, 8, 1.2f  }, // blue:   fireball2
                { 1.0f, 1.0f, 1.0f, 20, 9, 1.2f  }, // green:  fireball3
                { 1.0f, 0.1f, 0.1f, 0,  7, 0.2f  }, // red:     demotrack
        };
        
        int numrender = 0;
        
        for(particle *p, **pp = &parlist; (p = *pp);)
        {          
                parttype *pt = &parttypes[p->type];

                glBindTexture(GL_TEXTURE_2D, pt->tex);  
                glBegin(GL_QUADS);
                
                glColor3d(pt->r, pt->g, pt->b);
                float sz = pt->sz*particlesize/100.0f; 
                // perf varray?
                glTexCoord2f(0.0, 1.0); glVertex3d(p->o.x+(-right.x+up.x)*sz, p->o.z+(-right.y+up.y)*sz, p->o.y+(-right.z+up.z)*sz);
                glTexCoord2f(1.0, 1.0); glVertex3d(p->o.x+( right.x+up.x)*sz, p->o.z+( right.y+up.y)*sz, p->o.y+( right.z+up.z)*sz);
                glTexCoord2f(1.0, 0.0); glVertex3d(p->o.x+( right.x-up.x)*sz, p->o.z+( right.y-up.y)*sz, p->o.y+( right.z-up.z)*sz);
                glTexCoord2f(0.0, 0.0); glVertex3d(p->o.x+(-right.x-up.x)*sz, p->o.z+(-right.y-up.y)*sz, p->o.y+(-right.z-up.z)*sz);
                glEnd();
                xtraverts += 4;

                if(numrender++>maxparticles || (p->fade -= time)<0)
                {
                        *pp = p->next;
                        p->next = parempty;
                        parempty = p;
                }
                else
                {
                        if(pt->gr) p->o.z -= ((lastmillis-p->millis)/3.0f)*curtime/(pt->gr*10000);
                        vec a = p->d;
                        vmul(a,time);
                        vdiv(a,20000.0f);
                        vadd(p->o, a);
                        pp = &p->next;
                };
        };

        glEnable(GL_FOG);
        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);
};
