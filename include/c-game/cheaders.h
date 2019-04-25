#ifdef __cplusplus
extern "C"{
#endif
float perlinnoise_2D(float x, float y, float pers); 
void perlinarea(block b, int scale, int seed); 
extern void linestyle(float width, int r, int g, int b);
float depthcorrect(float d);
uchar *stripheader(uchar *b); 
int text_width(char *str);
void draw_text(char *str, int left, int top, int gl_num); 
void draw_envbox_aux(float s0, float t0, int x0, int y0, int z0,
                                         float s1, float t1, int x1, int y1, int z1,
                                         float s2, float t2, int x2, int y2, int z2,
                                         float s3, float t3, int x3, int y3, int z3,
int texture);
void draw_envbox(int t, int w); 
void updatepos(dynent *d);
extern void fixplayer1range();
void endianswap(void *memory, int stride, int length);
char *path(char *s);
char *loadfile(char *fn, int *size);
void backup(char *name, char *backupname); 
extern void writeclientinfo(FILE *f); 
extern void line(int x1, int y1, float z1, int x2, int y2, float z2);
extern void dot(int x, int y, float z);
extern void blendbox(int x1, int y1, int x2, int y2, bool border); 
extern void renderspheres(int time);
void invertperspective();
void drawicon(float tx, float ty, int x, int y);
void readmatrices();
int listlen(char *a);
extern void keyrepeat(bool on);
extern void cleangl();
void transplayer();
void purgetextures();
void gl_init(int w, int h);
void setarraypointers();
void reallocv();
extern int isoccluded(float vx, float vy, float cx, float cy, float csize);
extern void box(block b, float z1, float z2, float z3, float z4);
extern void render_particles(int time);
void resetmovement(dynent *d);
extern void setupworld2();
extern void addwaterquad(int x, int y, int size);
void stopsound(void);
void initsound(void);
void music(char *);
extern void mousemove(int dx, int dy);
extern void setupworld(int);
extern void *alloc(int);
extern void initclient();
void conoutf(const char *, ...);
#ifdef __cplusplus
}
#endif

