#include <ccube.h>
#include <ctools.h>
#include <cheaders.h>



void backup(char *name, char *backupname)
{
        remove(backupname);
        rename(name, backupname);
}
