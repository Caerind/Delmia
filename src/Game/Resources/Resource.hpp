#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include "../Buildings/Building.hpp"
#include "Resources.hpp"

class Resource : public Building
{
    public:
        typedef std::shared_ptr<Resource> Ptr;

    public:
        Resource();
        Resource(int x, int y);
};

#endif // RESOURCE_HPP
