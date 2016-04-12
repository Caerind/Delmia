#ifndef BARRACK_HPP
#define BARRACK_HPP

#include "Building.hpp"

class Barrack : public Building
{
    public:
        typedef std::shared_ptr<Barrack> Ptr;

    public:
        Barrack();
        Barrack(int x, int y);

        std::vector<sf::Vector2i> getTiles(int x, int y);
};

#endif // BARRACK_HPP
