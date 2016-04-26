#ifndef MARKET_HPP
#define MARKET_HPP

#include "Building.hpp"

class Market : public Building
{
    public:
        typedef std::shared_ptr<Market> Ptr;

    public:
        Market();
        Market(int x, int y);

        static std::vector<sf::Vector2i> getTilesBlueprint(int x, int y);
        virtual std::vector<sf::Vector2i> getTiles(int x, int y);
};

#endif // MARKET_HPP
