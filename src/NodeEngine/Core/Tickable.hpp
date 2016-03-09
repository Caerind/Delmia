#ifndef NTICKABLE_HPP
#define NTICKABLE_HPP

#include <SFML/System/Time.hpp>

class NTickable
{
    public:
        NTickable();
        ~NTickable();

        virtual void tick(sf::Time dt);
};

#endif // NTICKABLE_HPP
