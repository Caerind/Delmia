#ifndef HALL_HPP
#define HALL_HPP

#include "Building.hpp"

class Hall : public Building
{
    public:
        typedef std::shared_ptr<Hall> Ptr;

    public:
        Hall();
        Hall(int x, int y);

        static std::vector<sf::Vector2i> getTilesBlueprint(int x, int y);
        virtual std::vector<sf::Vector2i> getTiles(int x, int y);
};

#endif // HALL_HPP
