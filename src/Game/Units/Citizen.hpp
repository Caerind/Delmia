#ifndef CITIZEN_HPP
#define CITIZEN_HPP

#include "Unit.hpp"

class Citizen : public Unit
{
    public:
        typedef std::shared_ptr<Citizen> Ptr;

    public:
        Citizen();
        Citizen(float x, float y);

    private:
};

#endif // CITIZEN_HPP
