#include <ccube.h>
#include <ctools.h>
#include <cheaders.h>



void writeclientinfo(FILE *f)
{
        fprintf(f, "name \"%s\"\nteam \"%s\"\n", player1->name, player1->team);
}
