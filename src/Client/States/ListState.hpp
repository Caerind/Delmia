#ifndef LISTSTATE_HPP
#define LISTSTATE_HPP

#include "../../NodeEngine/Application/Application.hpp"

class ListState : public ah::State
{
    public:
        ListState(ah::StateManager& manager);

        bool handleEvent(sf::Event const& event);
        bool update(sf::Time dt);
        void render(sf::RenderTarget& target, sf::RenderStates states);
};

#endif // LISTSTATE_HPP
