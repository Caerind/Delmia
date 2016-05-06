#ifndef NTICKABLE_HPP
#define NTICKABLE_HPP

#include <SFML/System/Time.hpp>

class NTickable
{
    public:
        NTickable();
        ~NTickable();

        virtual void tick(sf::Time dt);

        bool hasTicked() const;
        void ticked();
        void unticked();

    private:
        bool mTicked;
};

#endif // NTICKABLE_HPP
