#ifndef FOREST_HPP
#define FOREST_HPP

#include "Resource.hpp"

class Forest : public Resource
{
    public:
        typedef std::shared_ptr<Forest> Ptr;

    public:
        Forest();
        Forest(sf::Vector2i const& coords);
};

#endif // FOREST_HPP
