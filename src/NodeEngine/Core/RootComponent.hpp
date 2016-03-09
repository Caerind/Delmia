#ifndef NROOTCOMPONENT_HPP
#define NROOTCOMPONENT_HPP

#include "Component.hpp"

class NActor;
class NRootComponent : public NComponent
{
    public:
        NRootComponent(NActor* actor);

        virtual void load(pugi::xml_node& node, std::string const& name = "");
        virtual void save(pugi::xml_node& node, std::string const& name = "");

    protected:
        NActor* mActor;
};

#endif // NROOTCOMPONENT_HPP
