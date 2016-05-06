#ifndef UNIT_HPP
#define UNIT_HPP

#include "../../NodeEngine/Core/SpriteComponent.hpp"
#include "../../NodeEngine/Core/MapUtility.hpp"

#include "../../NodeEngine/Core/PointComponent.hpp"

#include "../Entity.hpp"

#include "Units.hpp"

class Unit : public Entity
{
    public:
        typedef std::shared_ptr<Unit> Ptr;

        enum Direction
        {
            E = 0,
            NE,
            N,
            NW,
            W,
            SW,
            S,
            SE,
            Count,
        };

    public:
        Unit();
        Unit(float x, float y);

        std::size_t getType() const;

        void initSprite(std::string const& texture, sf::Vector2i tileSize, sf::Vector2f origin);

        void setDirection(Direction dir);
        void setDirection(float angle);
        std::size_t getDirection() const;

        void positionOrder(sf::Vector2f const& position);

        void tick(sf::Time dt);

        void load(pugi::xml_node& node);
        void save(pugi::xml_node& node);

    protected:
        std::size_t mType;
        NSpriteComponent mSprite;
        NPointComponent mRoot;
        NPointComponent mNode;
        sf::Vector2i mTileSize;
        std::size_t mDirection;

        float mSpeed;

        sf::Vector2f mPositionOrder;
        bool mPathDone;
        std::vector<sf::Vector2i> mPath;
};

#endif // UNIT_HPP
