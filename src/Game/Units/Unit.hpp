#ifndef UNIT_HPP
#define UNIT_HPP

#include "../../NodeEngine/Core/SpriteComponent.hpp"

#include "../Entity.hpp"

class Unit : public Entity
{
    public:
        typedef std::shared_ptr<Entity> Ptr;

    public:
        Unit();

        void load(pugi::xml_node& node);
        void save(pugi::xml_node& node);

    protected:
        NSpriteComponent mSprite;
};

#endif // UNIT_HPP
