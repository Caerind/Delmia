#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include "../Buildings/Building.hpp"

class Resource : public Building
{
    public:
        typedef std::shared_ptr<Resource> Ptr;

    public:
        Resource();
        Resource(int x, int y);

        sf::Vector2i getCoords() const;

    protected:
        sf::Vector2i mCoords;
};

#endif // RESOURCE_HPP
