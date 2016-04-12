#ifndef FOREST_HPP
#define FOREST_HPP

#include "../Buildings/Building.hpp"
#include "Resources.hpp"

class Forest : public Building
{
    public:
        typedef std::shared_ptr<Forest> Ptr;

    public:
        Forest();
        Forest(int x, int y);
};

#endif // FOREST_HPP
