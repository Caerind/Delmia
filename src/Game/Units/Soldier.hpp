#ifndef SOLDIER_HPP
#define SOLDIER_HPP

#include "Unit.hpp"

class Soldier : public Unit
{
    public:
        typedef std::shared_ptr<Soldier> Ptr;

    public:
        Soldier();
        Soldier(sf::Vector2f const& pos);

    protected:
};

#endif // SOLDIER_HPP
