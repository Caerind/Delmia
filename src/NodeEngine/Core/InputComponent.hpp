#ifndef NINPUTCOMPONENT_HPP
#define NINPUTCOMPONENT_HPP

#include "Component.hpp"
#include "ActionTarget.hpp"

class NInputComponent : public NComponent, public NActionTarget
{
    public:
        NInputComponent();

        virtual void load(pugi::xml_node& node, std::string const& name = "InputComponent");
        virtual void save(pugi::xml_node& node, std::string const& name = "InputComponent");
};

#endif // NINPUTCOMPONENT_HPP
