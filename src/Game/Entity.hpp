#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "../NodeEngine/Core/World.hpp"

class World;
class Entity : public NActor
{
    public:
        typedef std::shared_ptr<Entity> Ptr;

    public:
        Entity();

        // Life Properties
        void setLife(float life);
        void setLifeMax(float lifeMax);
        float getLife() const;
        float getLifeMax() const;
        float getLifePercent() const;
        bool isDead() const;
        void inflige(float value);
        void restore(float value);

        // ResourceHolder Properties
        void addResource(std::size_t resourceId, int value);
        void removeResource(std::size_t resourceId, int value);
        int moveResource(std::size_t resourceId);
        int moveResource(std::size_t resourceId, int value);
        bool hasResource(std::size_t resourceId) const;
        bool hasResourceAmount(std::size_t resourceId, int value) const;
        int getResourceAmount(std::size_t resourceId) const;

        void setWorld(World* world);

    protected:
        World* mWorld;

        float mLife;
        float mLifeMax;

        std::map<std::size_t,int> mResources;
};

#endif // ENTITY_HPP
