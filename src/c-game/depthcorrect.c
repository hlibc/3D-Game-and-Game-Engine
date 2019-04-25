#include <ccube.h>
#include <ctools.h>
#include <cheaders.h>



float depthcorrect(float d)
{
        return (d<=1/256.0f) ? d*256 : d;
}
