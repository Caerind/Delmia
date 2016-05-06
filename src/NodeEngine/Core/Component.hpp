#ifndef NCOMPONENT_HPP
#define NCOMPONENT_HPP

#include "../Utils/Container.hpp"
#include "../Utils/Pugixml.hpp"
#include "../Utils/String.hpp"

#include "Tickable.hpp"
#include "Transformable.hpp"

class NComponent : public NTickable, public NTransformable
{
    public:
        NComponent();

        void attachComponent(NComponent* component);
        void detachComponent(NComponent* component);

        float getFinalZ() const;
        sf::Vector2f getFinalPosition() const;
        sf::Transform getFinalTransform() const;

        sf::Vector2f getActorPosition() const;
        sf::Vector2f getActorScale() const;
        float getActorRotation() const;

        void setActorPosition(sf::Vector2f const& position);
        void setActorScale(sf::Vector2f const& scale);
        void setActorRotation(float rotation);

        virtual void onMoved();
        virtual void onMovedThis();
        virtual void onMovedChildren();
        virtual void onScaled();
        virtual void onScaledThis();
        virtual void onScaledChildren();
        virtual void onRotated();
        virtual void onRotatedThis();
        virtual void onRotatedChildren();

        virtual void load(pugi::xml_node& node, std::string const& name = "Component");
        virtual void save(pugi::xml_node& node, std::string const& name = "Component");

    protected:
        std::vector<NComponent*> mComponents;
        NComponent* mParent;
};

#endif // NCOMPONENT_HPP
