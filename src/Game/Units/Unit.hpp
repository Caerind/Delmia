#ifndef UNIT_HPP
#define UNIT_HPP

#include "../../NodeEngine/Core/SpriteComponent.hpp"
#include "../../NodeEngine/Core/Isometric.hpp"

#include "../../NodeEngine/Core/PointComponent.hpp"

#include "../Entity.hpp"
#include "../PlayerOwned.hpp"

#include "Units.hpp"

#include <SFML/System/Thread.hpp>
#include <SFML/System/Mutex.hpp>

class Unit : public Entity, public PlayerOwned
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
        Unit(Player* player, sf::Vector2f const& pos);

        std::size_t getType() const;

        void initSprite(std::string const& texture, sf::Vector2i tileSize, sf::Vector2f origin);

        sf::Vector2i getCoords() const;

        void setDirection(Direction dir);
        void setDirection(float angle);
        std::size_t getDirection() const;

        void moveToDest(sf::Vector2f const& dest, sf::Time dt);
        void updateTextureRect();

        void calculatePath();
        void onBuildingAdded(sf::Vector2i const& coords);

        void positionOrder(sf::Vector2f const& position);

        void tick(sf::Time dt);

        void takeResource(Entity* entity, std::size_t resourceId, int amount);
        void giveResource(Entity* entity);

        void load(pugi::xml_node& node);
        void save(pugi::xml_node& node);

    protected:
        std::size_t mType;
        NSpriteComponent mSprite;
        sf::Vector2i mTileSize;
        std::size_t mDirection;

        float mSpeed;
        sf::Time mMovingTime;

        sf::Vector2f mPositionOrder;
        bool mPathDone;
        std::vector<sf::Vector2i> mPath;
        sf::Thread mThread;
        sf::Mutex mMutex;
};

#endif // UNIT_HPP
