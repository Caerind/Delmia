#ifndef HALL_HPP
#define HALL_HPP

#include "Building.hpp"

class Hall : public Building
{
    public:
        typedef std::shared_ptr<Hall> Ptr;

    public:
        Hall();
        Hall(sf::Vector2i const& coords);

        static std::vector<sf::Vector2i> getTilesBlueprint(sf::Vector2i const& coords);
        std::vector<std::pair<sf::Vector2i,sf::IntRect>> getTiles(sf::Vector2i const& coords);
};

#endif // HALL_HPP
