#ifndef GOLDMINE_HPP
#define GOLDMINE_HPP

#include "../Buildings/Building.hpp"
#include "Resources.hpp"

class GoldMine : public Building
{
    public:
        typedef std::shared_ptr<GoldMine> Ptr;

    public:
        GoldMine();
        GoldMine(int x, int y);
};

#endif // GOLDMINE_HPP
