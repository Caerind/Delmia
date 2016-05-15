#ifndef MARKET_HPP
#define MARKET_HPP

#include "Building.hpp"

class Market : public Building
{
    public:
        typedef std::shared_ptr<Market> Ptr;

    public:
        Market();
        Market(Player* player, sf::Vector2i const& coords);

        static std::vector<sf::Vector2i> getTilesBlueprint(sf::Vector2i const& coords);
        std::vector<std::pair<sf::Vector2i,sf::IntRect>> getTiles(sf::Vector2i const& coords);
};

#endif // MARKET_HPP
