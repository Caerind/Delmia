#ifndef NTRANSFORMABLE_HPP
#define NTRANSFORMABLE_HPP

#include "../Utils/Vector.hpp"
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Transform.hpp>

class NTransformable
{
    public:
        NTransformable();

        virtual NVector getPosition() const;
        virtual void setPosition(NVector const& position);
        virtual void setPosition(sf::Vector2f const& position, float z = 0.f);
        virtual void setPosition(float x, float y, float z = 0.f);
        virtual void setPositionZ(float z);
        void move(NVector const& movement);
        void move(float x, float y, float z = 0.f);

        virtual NVector getScale() const;
        virtual void setScale(NVector const& scale);
        virtual void setScale(sf::Vector2f const& position);
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
