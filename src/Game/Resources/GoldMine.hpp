#ifndef GOLDMINE_HPP
#define GOLDMINE_HPP

#include "Resource.hpp"

class GoldMine : public Resource
{
    public:
        typedef std::shared_ptr<GoldMine> Ptr;

    public:
        GoldMine();
        GoldMine(sf::Vector2i const& coords);
};

#endif // GOLDMINE_HPP
