#include "InputComponent.hpp"

NInputComponent::NInputComponent()
{
}

void NInputComponent::load(pugi::xml_node& node, std::string const& name)
{
    pugi::xml_node n = node.child(name.c_str());
    for (pugi::xml_attribute attr = n.first_attribute(); attr; attr = attr.next_attribute())
    {
        std::string s = attr.value();
        char t = s[s.size()-1];
        s = s.substr(0,s.size()-2);
        int type = mActions[attr.name()].getType();
        if (t == 'm')
        {
            mActions[attr.name()].setMouseButton(NAction::stringToMouseButton(s),type);
        }
        if (t == 'k')
        {
            mActions[attr.name()].setKeyboardKey(NAction::stringToKey(s),type);
        }
    }
}

void NInputComponent::save(pugi::xml_node& node, std::string const& name)
{
    pugi::xml_node n = node.append_child(name.c_str());
    for (auto itr = mActions.begin(); itr != mActions.end(); itr++)
    {
        if (itr->second.isMouseAction())
        {
            std::string v = NAction::mouseButtonToString(itr->second.getMouseButton()) + ",m";
            n.append_attribute(itr->first.c_str()) = v.c_str();
        }
        if (itr->second.isKeyboardAction())
        {
            std::string v = NAction::keyToString(itr->second.getKeyboardKey()) + ",k";
            n.append_attribute(itr->first.c_str()) = v.c_str();
        }
    }
}
