#ifndef QUARRY_HPP
#define QUARRY_HPP

#include "Resource.hpp"

class Quarry : public Resource
{
    public:
        typedef std::shared_ptr<Quarry> Ptr;

    public:
        Quarry();
        Quarry(sf::Vector2i const& coords);
};

#endif // QUARRY_HPP
