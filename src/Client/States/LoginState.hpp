#ifndef LOGINSTATE_HPP
#define LOGINSTATE_HPP

#include "../../NodeEngine/Application/Application.hpp"

class LoginState : public ah::State
{
    public:
        LoginState(ah::StateManager& manager);

        bool handleEvent(sf::Event const& event);
        bool update(sf::Time dt);
        void render(sf::RenderTarget& target, sf::RenderStates states);
};
#endif // LOGINSTATE_HPP
