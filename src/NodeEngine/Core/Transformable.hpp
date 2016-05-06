#ifndef NTRANSFORMABLE_HPP
#define NTRANSFORMABLE_HPP

#include "../Utils/Vector.hpp"
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Transform.hpp>

class NTransformable
{
    public:
        NTransformable();

        virtual sf::Vector2f getPosition() const;
        virtual void setPosition(sf::Vector2f const& position);
        virtual void setPosition(float x, float y);
        virtual void move(sf::Vector2f const& movement);
        virtual void move(float x, float y);

        virtual sf::Vector2f getOrigin() const;
        virtual void setOrigin(sf::Vector2f const& origin);
        virtual void setOrigin(float x, float y);

        virtual float getPositionZ() const;
        virtual void setPositionZ(float z);
        virtual void moveZ(float z);

        virtual sf::Vector2f getScale() const;
        virtual void setScale(sf::Vector2f const& scale);
        virtual void setScale(float x, float y);

        virtual float getRotation() const;
        virtual void setRotation(float rotation);

        virtual sf::Transform getTransform() const;

        virtual void onMoved();
        virtual void onScaled();
        virtual void onRotated();

    private:
        sf::Transformable mTransformable;
        float mZ;
};

#endif // NTRANSFORMABLE_HPP
