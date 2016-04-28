#include "Resource.hpp"

Resource::Resource() : Building()
{
    mBuilt = true;
}

Resource::Resource(int x, int y) : Building(x,y)
{
    mBuilt = true;
}
