#ifndef BARRACK_HPP
#define BARRACK_HPP

#include "Building.hpp"

class Barrack : public Building
{
    public:
        typedef std::shared_ptr<Barrack> Ptr;

    public:
        Barrack();
        Barrack(Player* player, sf::Vector2i const& coords);

        static std::vector<sf::Vector2i> getTilesBlueprint(sf::Vector2i const& coords);
        std::vector<std::pair<sf::Vector2i,sf::IntRect>> getTiles(sf::Vector2i const& coords);
};

#endif // BARRACK_HPP
