#include <ccube.h>
#include <ctools.h>
#include <cheaders.h>

void purgetextures();

extern bool hasoverbright;



extern GLUquadricObj *qsphere;

extern int glmaxtexsize;

void gl_init(int w, int h)
{
        //#define fogvalues 0.5f, 0.6f, 0.7f, 1.0f

        glViewport(0, 0, w, h);
        glClearDepth(1.0);
        glDepthFunc(GL_LESS);
        glEnable(GL_DEPTH_TEST);
        glShadeModel(GL_SMOOTH);


        glEnable(GL_FOG);
        glFogi(GL_FOG_MODE, GL_LINEAR);
        glFogf(GL_FOG_DENSITY, 0.25);
        glHint(GL_FOG_HINT, GL_NICEST);


        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        glEnable(GL_POLYGON_OFFSET_LINE);
        glPolygonOffset(-3.0, -3.0);

        glCullFace(GL_FRONT);
        glEnable(GL_CULL_FACE);

        char *exts = (char *)glGetString(GL_EXTENSIONS);

        if(strstr(exts, "GL_EXT_texture_env_combine")) hasoverbright = true;
        else ;//conoutf("WARNING: cannot use overbright lighting, using old lighting model!");

        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glmaxtexsize);

        purgetextures();

        if(!(qsphere = gluNewQuadric())) ;//fatal("glu sphere");
        gluQuadricDrawStyle(qsphere, GLU_FILL);
        gluQuadricOrientation(qsphere, GLU_INSIDE);
        gluQuadricTexture(qsphere, GL_TRUE);
        glNewList(1, GL_COMPILE);
        gluSphere(qsphere, 1, 12, 6);
        glEndList();
}
