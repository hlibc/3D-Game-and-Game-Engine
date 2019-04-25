#include <ccube.h>
#include <ctools.h>
#include <cheaders.h>
extern string clientmap;
extern string ctext;
extern string toservermap;
extern string clientpassword;

void initclient()
{
	clientmap[0] = 0;
        ctext[0] = 0;
        toservermap[0] = 0;
        clientpassword[0] = 0;
        //newname("unnamed");
        //newteam("red");
}
