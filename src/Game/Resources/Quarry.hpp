#ifndef QUARRY_HPP
#define QUARRY_HPP

#include "../Buildings/Building.hpp"
#include "Resources.hpp"

class Quarry : public Building
{
    public:
        typedef std::shared_ptr<Quarry> Ptr;

    public:
        Quarry();
        Quarry(int x, int y);
};

#endif // QUARRY_HPP
