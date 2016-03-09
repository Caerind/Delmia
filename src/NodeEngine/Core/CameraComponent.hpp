#ifndef NCAMERACOMPONENT_HPP
#define NCAMERACOMPONENT_HPP

#include "Component.hpp"
#include <SFML/Graphics/View.hpp>

class NCameraComponent : public NComponent
{
    public:
        NCameraComponent();
        ~NCameraComponent();

        sf::View& getView();

    private:
        sf::View mView;
};

#endif // NCAMERACOMPONENT_HPP
