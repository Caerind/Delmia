#include "Resource.hpp"

Resource::Resource() : Building()
{
    mType = Resources::DefaultResource;
    mBuilt = true;
}

Resource::Resource(int x, int y) : Building(x,y)
{
    mType = Resources::DefaultResource;
    mBuilt = true;
}
