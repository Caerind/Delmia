#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include "../Entity.hpp"
#include "../IsometricBase.hpp"

#include "../Buildings/Buildings.hpp"
#include "Resources.hpp"

class Resource : public Entity, public IsometricBase
{
    public:
        typedef std::shared_ptr<Resource> Ptr;

    public:
        Resource();
        Resource(sf::Vector2i const& coords);

        std::size_t getType() const;

    protected:
        std::size_t mType;
};

#endif // RESOURCE_HPP
